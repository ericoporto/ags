//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2025 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#include <stdio.h>
#include <math.h>
#include "core/platform.h"
#include "media/audio/audio.h"
#include "ac/audiocliptype.h"
#include "ac/gamesetupstruct.h"
#include "ac/dynobj/cc_audioclip.h"
#include "ac/dynobj/cc_audiochannel.h"
#include "ac/gamestate.h"
#include "script/script_runtime.h"
#include "ac/audiochannel.h"
#include "ac/audioclip.h"
#include "ac/game.h"
#include "ac/gamesetup.h"
#include "ac/path_helper.h"
#include "ac/view.h"
#include "media/audio/sound.h"
#include "debug/debug_log.h"
#include "debug/debugger.h"
#include "ac/common.h"
#include "ac/file.h"
#include "ac/global_audio.h"
#include <math.h>
#include "util/stream.h"
#include "core/assetmanager.h"
#include "ac/timer.h"
#include "main/game_run.h"
#include "media/audio/audio_core.h"
#include "platform/base/sys_main.h"
#include "ac/dynobj/dynobj_manager.h"

using namespace AGS::Common;
using namespace AGS::Engine;

//-----------------------
//sound channel management

static std::array<std::unique_ptr<SoundClip>, TOTAL_AUDIO_CHANNELS> _channels;

SoundClip *AudioChans::GetChannel(int index)
{
    return _channels[index].get();
}

SoundClip *AudioChans::GetChannelIfPlaying(int index)
{
    auto *ch = _channels[index].get();
    return (ch != nullptr && ch->is_ready()) ? ch : nullptr;
}

SoundClip *AudioChans::SetChannel(int index, std::unique_ptr<SoundClip> &&ch)
{
    if ((ch != nullptr) && (_channels[index] != nullptr))
        Debug::Printf(kDbgMsg_Warn, "WARNING: channel %d - clip overwritten", index);
    _channels[index] = std::move(ch);
    return _channels[index].get();
}

SoundClip *AudioChans::MoveChannel(int to, int from)
{
    return SetChannel(to, std::move(_channels[from]));
}

void AudioChans::DeleteClipOnChannel(int index)
{
    _channels[index].reset();
}

extern GameSetupStruct game;
extern RoomStruct thisroom;
extern CharacterInfo*playerchar;
extern CCAudioChannel ccDynamicAudio;

extern volatile int switching_away_from_game;
extern int frames_per_second; // for queue "hack"

ScriptAudioChannel scrAudioChannel[MAX_GAME_CHANNELS];
int reserved_channel_count = 0;

void calculate_reserved_channel_count()
{
    int reservedChannels = 0;
    for (size_t i = 0; i < game.audioClipTypes.size(); i++)
    {
        reservedChannels += game.audioClipTypes[i].reservedChannels;
    }
    reserved_channel_count = reservedChannels;
}

void update_clip_default_volume(ScriptAudioClip *audioClip)
{
    if (play.default_audio_type_volumes[audioClip->type] >= 0) 
    {
        audioClip->defaultVolume = play.default_audio_type_volumes[audioClip->type];
    }
}

void start_fading_in_new_track_if_applicable(int fadeInChannel, ScriptAudioClip *newSound)
{
    int crossfadeSpeed = game.audioClipTypes[newSound->type].crossfadeSpeed;
    if (crossfadeSpeed > 0)
    {
        update_clip_default_volume(newSound);
        play.crossfade_in_volume_per_step = crossfadeSpeed;
        play.crossfade_final_volume_in = newSound->defaultVolume;
        play.crossfading_in_channel = fadeInChannel;
    }
}

static void move_track_to_crossfade_channel(int currentChannel, int crossfadeSpeed, int fadeInChannel, ScriptAudioClip *newSound)
{
    stop_and_destroy_channel(SPECIAL_CROSSFADE_CHANNEL);
    auto *cfade_clip = AudioChans::MoveChannel(SPECIAL_CROSSFADE_CHANNEL, currentChannel);
    if (!cfade_clip)
        return;

    play.crossfading_out_channel = SPECIAL_CROSSFADE_CHANNEL;
    play.crossfade_step = 0;
    play.crossfade_initial_volume_out = cfade_clip->get_volume100();
    play.crossfade_out_volume_per_step = crossfadeSpeed;

    play.crossfading_in_channel = fadeInChannel;
    if (newSound != nullptr)
    {
        start_fading_in_new_track_if_applicable(fadeInChannel, newSound);
    }
}

// NOTE: this function assumes one of the user channels
void stop_or_fade_out_channel(int fadeOutChannel, int fadeInChannel, ScriptAudioClip *newSound)
{
    ScriptAudioClip *sourceClip = AudioChannel_GetPlayingClip(&scrAudioChannel[fadeOutChannel]);
    if ((play.fast_forward == 0) && // don't crossfade if skipping a cutscene
        (sourceClip != nullptr) && (game.audioClipTypes[sourceClip->type].crossfadeSpeed > 0))
    {
        move_track_to_crossfade_channel(fadeOutChannel, game.audioClipTypes[sourceClip->type].crossfadeSpeed, fadeInChannel, newSound);
    }
    else
    {
        stop_and_destroy_channel(fadeOutChannel);
    }
}

static int find_free_audio_channel(ScriptAudioClip *clip, int priority, bool interruptEqualPriority,
    bool for_queue = true)
{
    int lowestPrioritySoFar = 9999999;
    int lowestPriorityID = -1;
    int channelToUse = -1;

    if (!interruptEqualPriority)
        priority--;

    int startAtChannel = reserved_channel_count;
    int endBeforeChannel = game.numGameChannels;

    if (game.audioClipTypes[clip->type].reservedChannels > 0)
    {
        startAtChannel = 0;
        for (int i = 0; i < clip->type; i++)
        {
            startAtChannel += game.audioClipTypes[i].reservedChannels;
        }
        // NOTE: we allow to place sound on a crossfade channel for backward compatibility,
        // but ONLY for the case of audio type with reserved channels (weird quirk).
        endBeforeChannel = std::min(game.numCompatGameChannels,
            startAtChannel + game.audioClipTypes[clip->type].reservedChannels);
    }

    for (int i = startAtChannel; i < endBeforeChannel; i++)
    {
        auto* ch = AudioChans::GetChannelIfPlaying(i);
        if (ch == nullptr)
        {
            channelToUse = i;
            stop_and_destroy_channel(i);
            break;
        }
        if ((ch->priority < lowestPrioritySoFar) &&
            (ch->sourceClipType == clip->type))
        {
            lowestPrioritySoFar = ch->priority;
            lowestPriorityID = i;
        }
        // NOTE: This is a "hack" for starting queued clips;
        // since having a new audio system (3.6.0 onwards), the audio timing
        // changed a little, and queued sounds have to start bit earlier
        // if we want them to sound seamless with the previous clips.
        // TODO: investigate better solutions? may require reimplementation of the sound queue.
        if (for_queue && (ch->sourceClipType == clip->type))
        { // try to start queued sounds 1 frame earlier
            const float trigger_pos = (1000.f / frames_per_second) * 1.f;
            if (ch->get_pos_ms() >= (ch->get_length_ms() - trigger_pos))
            {
                lowestPrioritySoFar = priority;
                lowestPriorityID = i;
            }
        }
    }

    if ((channelToUse < 0) && (lowestPriorityID >= 0) &&
        (lowestPrioritySoFar <= priority))
    {
        stop_or_fade_out_channel(lowestPriorityID, lowestPriorityID, clip);
        channelToUse = lowestPriorityID;
    }
    else if ((channelToUse >= 0) && (play.crossfading_in_channel < 1))
    {
        start_fading_in_new_track_if_applicable(channelToUse, clip);
    }
    return channelToUse;
}

bool is_audiotype_allowed_to_play(AudioFileType /*type*/)
{ // TODO: this is a remnant of an old audio logic, think this function over
    return usetup.AudioEnabled;
}

std::unique_ptr<SoundClip> load_sound_clip(ScriptAudioClip *audioClip, bool repeat)
{
    if (!is_audiotype_allowed_to_play((AudioFileType)audioClip->fileType))
    {
        return nullptr;
    }

    update_clip_default_volume(audioClip);

    AssetPath asset_name = get_audio_clip_assetpath(audioClip->bundlingType, audioClip->fileName);
    const char *ext = "";
    switch (audioClip->fileType)
    {
    case eAudioFileOGG:
        ext = "ogg"; break;
    case eAudioFileMP3:
        ext = "mp3"; break;
    case eAudioFileWAV:
    case eAudioFileVOC:
        ext = "wav"; break;
    case eAudioFileMIDI:
        ext = "mid"; break;
    case eAudioFileMOD:
        ext = "mod"; break;
    default:
        quitprintf("AudioClip.Play: invalid audio file type encountered: %d", audioClip->fileType);
    }

    std::unique_ptr<SoundClip> soundClip = load_sound_clip(asset_name, ext, repeat);
    if (soundClip != nullptr)
    {
        soundClip->set_volume100(audioClip->defaultVolume);
        soundClip->sourceClipID = audioClip->id;
        soundClip->sourceClipType = audioClip->type;
    }
    return soundClip;
}

static void audio_update_polled_stuff()
{
    ///////////////////////////////////////////////////////////////////////////
    // Do crossfade
    play.crossfade_step++;

    if (play.crossfading_out_channel > 0 && !AudioChans::GetChannelIfPlaying(play.crossfading_out_channel))
        play.crossfading_out_channel = 0;

    if (play.crossfading_out_channel > 0)
    {
        SoundClip* ch = AudioChans::GetChannel(play.crossfading_out_channel);
        int newVolume = ch ? ch->get_volume100() - play.crossfade_out_volume_per_step : 0;
        if (newVolume > 0)
        {
            ch->set_volume100(newVolume);
        }
        else
        {
            stop_and_destroy_channel(play.crossfading_out_channel);
            play.crossfading_out_channel = 0;
        }
    }

    if (play.crossfading_in_channel > 0 && !AudioChans::GetChannelIfPlaying(play.crossfading_in_channel))
        play.crossfading_in_channel = 0;

    if (play.crossfading_in_channel > 0)
    {
        SoundClip* ch = AudioChans::GetChannel(play.crossfading_in_channel);
        int newVolume = ch ? ch->get_volume100() + play.crossfade_in_volume_per_step : 0;
        if (newVolume > play.crossfade_final_volume_in)
        {
            newVolume = play.crossfade_final_volume_in;
        }

        ch->set_volume100(newVolume);

        if (newVolume >= play.crossfade_final_volume_in)
        {
            play.crossfading_in_channel = 0;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Do audio queue
    if (play.new_music_queue_size > 0)
    {
        for (int i = 0; i < play.new_music_queue_size; i++)
        {
            ScriptAudioClip *clip = &game.audioClips[play.new_music_queue[i].audioClipIndex];
            int channel = find_free_audio_channel(clip, clip->defaultPriority, false, true);
            if (channel >= 0)
            {
                QueuedAudioItem itemToPlay = std::move(play.new_music_queue[i]);

                play.new_music_queue_size--;
                for (int j = i; j < play.new_music_queue_size; j++)
                {
                    play.new_music_queue[j] = std::move(play.new_music_queue[j + 1]);
                }

                play_audio_clip_on_channel(channel, clip, itemToPlay.priority, itemToPlay.repeat, 0, std::move(itemToPlay.cachedClip));
                i--;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Do non-blocking voice speech
    // NOTE: there's only one speech channel, therefore it's either blocking
    // or non-blocking at any given time. If it's changed, we'd need to keep
    // record of every channel, or keep a count of active channels.
    if (play.IsNonBlockingVoiceSpeech())
    {
        if (!AudioChans::ChannelIsPlaying(SCHAN_SPEECH))
        {
            stop_voice_nonblocking();
        }
    }
}

// Applies a volume drop modifier to the clip, in accordance to its audio type
static void apply_volume_drop_to_clip(SoundClip *clip)
{
    int audiotype = clip->sourceClipType;
    clip->apply_volume_modifier(-(game.audioClipTypes[audiotype].volume_reduction_while_speech_playing * 255 / 100));
}

static void queue_audio_clip_to_play(ScriptAudioClip *clip, int priority, int repeat)
{
    if (play.new_music_queue_size >= MAX_QUEUED_MUSIC) {
        debug_script_log("Too many queued music, cannot add %s", clip->scriptName.GetCStr());
        return;
    }

    std::unique_ptr<SoundClip> cachedClip = load_sound_clip(clip, (repeat != 0));
    if (cachedClip != nullptr) 
    {
        play.new_music_queue[play.new_music_queue_size].audioClipIndex = clip->id;
        play.new_music_queue[play.new_music_queue_size].priority = priority;
        play.new_music_queue[play.new_music_queue_size].repeat = (repeat != 0);
        play.new_music_queue[play.new_music_queue_size].cachedClip = std::move(cachedClip);
        play.new_music_queue_size++;
    }
}

ScriptAudioChannel* play_audio_clip_on_channel(int channel, ScriptAudioClip *clip, int priority, int repeat, int fromOffset, std::unique_ptr<SoundClip> &&soundfx)
{
    if (soundfx == nullptr)
    {
        soundfx = load_sound_clip(clip, (repeat) ? true : false);
    }
    if (soundfx == nullptr)
    {
        debug_script_log("AudioClip.Play: unable to load sound file");
        if (play.crossfading_in_channel == channel)
        {
            play.crossfading_in_channel = 0;
        }
        return nullptr;
    }
    soundfx->priority = priority;

    if (play.crossfading_in_channel == channel)
    {
        soundfx->set_volume100(0);
    }

    // Mute the audio clip if fast-forwarding the cutscene
    if (play.fast_forward) 
    {
        soundfx->set_mute(true);

        // CHECKME!!
        // [IKM] According to the 3.2.1 logic the clip will restore
        // its value after cutscene, but only if originalVolAsPercentage
        // is not zeroed. Something I am not sure about: why does it
        // disable the clip under condition that there's more than one
        // channel for this audio type? It does not even check if
        // anything of this type is currently playing.
        if (game.audioClipTypes[clip->type].reservedChannels != 1)
            soundfx->set_volume100(0);
    }

    if (soundfx->play_from(fromOffset) == 0)
    {
        debug_script_log("AudioClip.Play: failed to play sound file");
        return nullptr;
    }

    // Apply volume drop if any speech voice-over is currently playing
    // NOTE: there is a confusing logic in sound clip classes, that they do not use
    // any modifiers when begin playing, therefore we must apply this only after
    // playback was started.
    if (!play.fast_forward && play.speech_has_voice)
        apply_volume_drop_to_clip(soundfx.get());

    AudioChans::SetChannel(channel, std::move(soundfx));
    return &scrAudioChannel[channel];
}

void remove_clips_of_type_from_queue(int audioType) 
{
    int aa;
    for (aa = 0; aa < play.new_music_queue_size; aa++)
    {
        ScriptAudioClip *clip = &game.audioClips[play.new_music_queue[aa].audioClipIndex];
        if ((audioType == SCR_NO_VALUE) || (clip->type == audioType))
        {
            play.new_music_queue_size--;
            for (int bb = aa; bb < play.new_music_queue_size; bb++)
                play.new_music_queue[bb] = std::move(play.new_music_queue[bb + 1]);
            aa--;
        }
    }
}

void update_queued_clips_volume(int audioType, int new_vol)
{
    for (int i = 0; i < play.new_music_queue_size; ++i)
    {
        // NOTE: if clip is uncached, the volume will be set from defaults when it is loaded
        SoundClip *sndclip = play.new_music_queue[i].cachedClip.get();
        if (sndclip)
        {
            ScriptAudioClip *clip = &game.audioClips[play.new_music_queue[i].audioClipIndex];
            if (clip->type == audioType)
                sndclip->set_volume100(new_vol);
        }
    }
}

ScriptAudioChannel* play_audio_clip(ScriptAudioClip *clip, int priority, int repeat, int fromOffset, bool queueIfNoChannel)
{
    if (!queueIfNoChannel)
        remove_clips_of_type_from_queue(clip->type);

    if (priority == SCR_NO_VALUE)
        priority = clip->defaultPriority;
    if (repeat == SCR_NO_VALUE)
        repeat = clip->defaultRepeat;

    int channel = find_free_audio_channel(clip, priority, !queueIfNoChannel, queueIfNoChannel);
    if (channel < 0)
    {
        if (queueIfNoChannel)
            queue_audio_clip_to_play(clip, priority, repeat);
        else
            debug_script_log("AudioClip.Play: no channels available to interrupt PRI:%d TYPE:%d", priority, clip->type);

        return nullptr;
    }

    return play_audio_clip_on_channel(channel, clip, priority, repeat, fromOffset);
}

ScriptAudioChannel* play_audio_clip_by_index(int audioClipIndex)
{
    if ((audioClipIndex >= 0) && ((size_t)audioClipIndex < game.audioClips.size()))
        return AudioClip_Play(&game.audioClips[audioClipIndex], SCR_NO_VALUE, SCR_NO_VALUE);
    else 
        return nullptr;
}

void stop_and_destroy_channel_ex(int chid, bool resetLegacyMusicSettings)
{
    if ((chid < 0) || (chid >= TOTAL_AUDIO_CHANNELS))
        quit("!StopChannel: invalid channel ID");

    AudioChans::DeleteClipOnChannel(chid);

    if (play.crossfading_in_channel == chid)
        play.crossfading_in_channel = 0;
    if (play.crossfading_out_channel == chid)
        play.crossfading_out_channel = 0;
    // don't update 'crossFading' here as it is updated in all the cross-fading functions.

    // destroyed an ambient sound channel
    if (chid < game.numGameChannels)
    {
        if (ambient[chid].channel > 0)
            ambient[chid].channel = 0;
    }

    if ((chid == SCHAN_MUSIC) && (resetLegacyMusicSettings))
    {
        play.cur_music_number = -1;
        current_music_type = 0;
    }
}

void stop_and_destroy_channel(int chid)
{
    stop_and_destroy_channel_ex(chid, true);
}

void export_missing_audiochans()
{
    for (int i = 0; i < game.numCompatGameChannels; ++i)
    {
        int h = ccGetObjectHandleFromAddress(&scrAudioChannel[i]);
        if (h <= 0)
            ccRegisterManagedObject(&scrAudioChannel[i], &ccDynamicAudio);
    }
}



// ***** BACKWARDS COMPATIBILITY WITH OLD AUDIO SYSTEM ***** //

int get_old_style_number_for_sound(int sound_number)
{
    // In the legacy audio system treat sound_number as an old style number
    if (game.IsLegacyAudioSystem())
    {
        return sound_number;
    }

    // Treat sound_number as a real clip index
    if (sound_number >= 0)
    {
        int old_style_number = 0;
        if (sscanf(game.audioClips[sound_number].scriptName.GetCStr(), "aSound%d", &old_style_number) == 1)
            return old_style_number;    
    }
    return 0;
}

std::unique_ptr<SoundClip> load_sound_clip_from_old_style_number(bool isMusic, int indexNumber, bool repeat)
{
    ScriptAudioClip* audioClip = GetAudioClipForOldStyleNumber(game, isMusic, indexNumber);

    if (audioClip != nullptr)
    {
        return load_sound_clip(audioClip, repeat);
    }

    return nullptr;
}

//=============================================================================

std::array<AmbientSound, MAX_GAME_CHANNELS> ambient;

int get_volume_adjusted_for_distance(int volume, int sndX, int sndY, int sndMaxDist)
{
    int distx = playerchar->x - sndX;
    int disty = playerchar->y - sndY;
    // it uses Allegro's "fix" sqrt without the ::
    int dist = (int)::sqrt((double)(distx*distx + disty*disty));

    // if they're quite close, full volume
    int wantvol = volume;

    if (dist >= AMBIENCE_FULL_DIST)
    {
        // get the relative volume
        wantvol = ((dist - AMBIENCE_FULL_DIST) * volume) / sndMaxDist;
        // closer is louder
        wantvol = volume - wantvol;
    }

    return wantvol;
}

void update_directional_sound_vol()
{
    for (int chnum = NUM_SPEECH_CHANS; chnum < game.numGameChannels; chnum++)
    {
        auto* ch = AudioChans::GetChannelIfPlaying(chnum);
        if ((ch != nullptr) && (ch->xSource >= 0)) 
        {
            ch->apply_directional_modifier(
                get_volume_adjusted_for_distance(ch->get_volume255(), 
                    ch->xSource,
                    ch->ySource,
                    ch->maximumPossibleDistanceAway) -
                ch->get_volume255());
        }
    }
}

void update_ambient_sound_vol ()
{
    for (int chan = NUM_SPEECH_CHANS; chan < game.numGameChannels; chan++) {

        AmbientSound *thisSound = &ambient[chan];

        if (thisSound->channel == 0)
            continue;

        int sourceVolume = thisSound->vol;

        if (play.speech_has_voice) {
            // Negative value means set exactly; positive means drop that amount
            if (play.speech_music_drop < 0)
                sourceVolume = -play.speech_music_drop;
            else
                sourceVolume -= play.speech_music_drop;

            if (sourceVolume < 0)
                sourceVolume = 0;
            if (sourceVolume > 255)
                sourceVolume = 255;
        }

        // Adjust ambient volume so it maxes out at overall sound volume
        int ambientvol = (sourceVolume * play.sound_volume) / 255;

        int wantvol;

        if ((thisSound->x == 0) && (thisSound->y == 0)) {
            wantvol = ambientvol;
        }
        else {
            wantvol = get_volume_adjusted_for_distance(ambientvol, thisSound->x, thisSound->y, thisSound->maxdist);
        }

        auto *ch = AudioChans::GetChannelIfPlaying(thisSound->channel);
        if (ch)
            ch->set_volume255(wantvol);
    }
}

std::unique_ptr<SoundClip> load_sound_and_play(ScriptAudioClip *aclip, bool repeat)
{
    std::unique_ptr<SoundClip> soundfx = load_sound_clip(aclip, repeat);
    if (!soundfx)
        return nullptr;

    if (soundfx->play() == 0)
    {
        return nullptr;
    }

    return soundfx;
}

void stop_all_sound_and_music() 
{
    stopmusic();
    stop_voice_nonblocking();
    // make sure it doesn't start crossfading when it comes back
    crossFading = 0;
    // any ambient sound will be aborted
    for (int i = 0; i < TOTAL_AUDIO_CHANNELS; ++i)
        stop_and_destroy_channel(i);
    // Sound queues
    cachedQueuedMusic.reset();
    for (int i = 0; i < MAX_QUEUED_MUSIC; ++i)
        play.new_music_queue[i] = QueuedAudioItem();
}

void shutdown_sound() 
{
    stop_all_sound_and_music(); // game logic
    audio_core_shutdown(); // audio core system
    soundcache_clear(); // clear cached data
    sys_audio_shutdown(); // backend; NOTE: sys_main will know if it's required
    usetup.AudioEnabled = false;
}

// the sound will only be played if there is a free channel or
// it has a priority >= an existing sound to override
static int play_sound_priority (int val1, int priority) {
    int lowest_pri = 9999, lowest_pri_id = -1;

    // find a free channel to play it on
    for (int i = SCHAN_NORMAL; i < game.numGameChannels; i++) {
        auto* ch = AudioChans::GetChannelIfPlaying(i);
        if (val1 < 0) {
            // Playing sound -1 means iterate through and stop all sound
            if (ch)
                stop_and_destroy_channel (i);
        }
        else if (ch == nullptr || !ch->is_ready()) {
            // PlaySoundEx will destroy the previous channel value.
            const int usechan = PlaySoundEx(val1, i);
            if (usechan >= 0)
            { // channel will hold a different clip here
                assert(usechan == i);
                ch = AudioChans::GetChannel(usechan);
                if (ch)
                    ch->priority = priority;
            }
            return usechan;
        }
        else if (ch->priority < lowest_pri) {
            lowest_pri = ch->priority;
            lowest_pri_id = i;
        }

    }
    if (val1 < 0)
        return -1;

    // no free channels, see if we have a high enough priority
    // to override one
    if (priority >= lowest_pri) {
        const int usechan = PlaySoundEx(val1, lowest_pri_id);
        if (usechan >= 0) {
            assert(usechan == lowest_pri_id);
            auto *ch = AudioChans::GetChannel(usechan);
            if (ch)
                ch->priority = priority;
            return usechan;
        }
    }

    return -1;
}

int play_sound(int val1) {
    return play_sound_priority(val1, 10);
}


//=============================================================================


// This is an indicator of a music played by an old audio system
// (to distinguish from the new system API)
int current_music_type = 0;
// crossFading is >0 (channel number of new track), or -1 (old
// track fading out, no new track)
int crossFading = 0, crossFadeVolumePerStep = 0, crossFadeStep = 0;
int crossFadeVolumeAtStart = 0;
std::unique_ptr<SoundClip> cachedQueuedMusic;

//=============================================================================
// Music update is scheduled when the voice speech stops;
// we do a small delay before reverting any volume adjustments
static bool music_update_scheduled = false;
static auto music_update_at = Clock::now();

void cancel_scheduled_music_update() {
    music_update_scheduled = false;
}

void schedule_music_update_at(Clock::time_point at) {
    music_update_scheduled = true;
    music_update_at = at;
}

void postpone_scheduled_music_update_by(std::chrono::milliseconds duration) {
    if (!music_update_scheduled) { return; }
    music_update_at += duration;
}

void process_scheduled_music_update() {
    if (!music_update_scheduled) { return; }
    if (music_update_at > Clock::now()) { return; }
    cancel_scheduled_music_update();
    update_music_volume();
    apply_volume_drop_modifier(false);
    update_ambient_sound_vol();
}
// end scheduled music update functions
//=============================================================================

void clear_music_cache()
{
    cachedQueuedMusic = nullptr;
}

static void play_new_music(int mnum, std::unique_ptr<SoundClip> &&music);

void play_next_queued() {
    // check if there's a queued one to play
    if (play.music_queue_size > 0) {

        int tuneToPlay = play.music_queue[0];

        if (tuneToPlay >= QUEUED_MUSIC_REPEAT) {
            // Loop it!
            play.music_repeat++;
            play_new_music(tuneToPlay - QUEUED_MUSIC_REPEAT, std::move(cachedQueuedMusic));
            play.music_repeat--;
        }
        else {
            // Don't loop it!
            int repeatWas = play.music_repeat;
            play.music_repeat = 0;
            play_new_music(tuneToPlay, std::move(cachedQueuedMusic));
            play.music_repeat = repeatWas;
        }

        play.music_queue_size--;
        for (int i = 0; i < play.music_queue_size; i++)
            play.music_queue[i] = play.music_queue[i + 1];

        if (play.music_queue_size > 0)
            cachedQueuedMusic = load_music_from_disk(play.music_queue[0], 0);
    }

}

int calculate_max_volume() {
    // quieter so that sounds can be heard better
    int newvol=play.music_master_volume + ((int)thisroom.Options.MusicVolume) * LegacyRoomVolumeFactor;
    if (newvol>255) newvol=255;
    if (newvol<0) newvol=0;

    if (play.fast_forward)
        newvol = 0;

    return newvol;
}

// add/remove the volume drop to the audio channels while speech is playing
void apply_volume_drop_modifier(bool applyModifier)
{
    for (int i = NUM_SPEECH_CHANS; i < game.numGameChannels; i++)
    {
        auto* ch = AudioChans::GetChannelIfPlaying(i);
        if (ch && ch->sourceClipID >= 0)
        {
            if (applyModifier)
                apply_volume_drop_to_clip(ch);
            else
                ch->apply_volume_modifier(0); // reset modifier
        }
    }
}

// Checks if speech voice-over is currently playing, and reapply volume drop to all other active clips
void update_volume_drop_if_voiceover()
{
    apply_volume_drop_modifier(play.speech_has_voice);
}

// Sync logical game channels with the audio backend:
// startup new assigned clips, apply changed parameters.
void sync_audio_playback()
{
    for (int i = 0; i < TOTAL_AUDIO_CHANNELS; ++i)
    { // update the playing channels, and dispose the finished / invalid ones
        auto *ch = AudioChans::GetChannelIfPlaying(i);
        if (ch && !ch->update())
        {
            AudioChans::DeleteClipOnChannel(i);
        }
    }
}

// Update the music, and advance the crossfade on a step
// (this should only be called once per game loop)
void update_audio_system_on_game_loop ()
{
    update_polled_stuff();

    // Sync logical game channels with the audio backend
    // NOTE: we update twice, first time here - because we need to know
    // which clips are still playing before updating the sound transitions
    // and queues, then second time later - because we need to apply any
    // changes to channels / parameters.
    // TODO: investigate options for optimizing this.
    sync_audio_playback();

    process_scheduled_music_update();

    audio_update_polled_stuff();

    if (crossFading) {
        crossFadeStep++;
        update_music_volume();
    }

    // Check if the current music has finished playing
    if ((play.cur_music_number >= 0) && (play.fast_forward == 0)) {
        if (IsMusicPlaying() == 0) {
            // The current music has finished
            play.cur_music_number = -1;
            play_next_queued();
        }
        else if ((game.options[OPT_CROSSFADEMUSIC] > 0) &&
            (play.music_queue_size > 0) && (!crossFading)) {
                // want to crossfade, and new tune in the queue
                auto *ch = AudioChans::GetChannel(SCHAN_MUSIC);
                if (ch) {
                    int curpos = ch->get_pos_ms();
                    int muslen = ch->get_length_ms();
                    if ((curpos > 0) && (muslen > 0)) {
                        // we want to crossfade, and we know how far through
                        // the tune we are
                        int takesSteps = calculate_max_volume() / game.options[OPT_CROSSFADEMUSIC];
                        int takesMs = ::lround(takesSteps * 1000.0f / get_game_fps());
                        if (curpos >= muslen - takesMs)
                            play_next_queued();
                    }
                }
        }
    }

    if (get_loop_counter() % 5 == 0) // TODO: investigate why we do this each 5 frames?
    {
        update_ambient_sound_vol();
        update_directional_sound_vol();
    }

    // Sync logical game channels with the audio backend again
    sync_audio_playback();
}

void stopmusic()
{
    if (crossFading > 0) {
        // stop in the middle of a new track fading in
        // Abort the new track, and let the old one finish fading out
        stop_and_destroy_channel (crossFading);
        crossFading = -1;
    }
    else if (crossFading < 0) {
        // the music is already fading out
        if (game.options[OPT_CROSSFADEMUSIC] <= 0) {
            // If they have since disabled crossfading, stop the fadeout
            stop_and_destroy_channel(SCHAN_MUSIC);
            crossFading = 0;
            crossFadeStep = 0;
            update_music_volume();
        }
    }
    else if ((game.options[OPT_CROSSFADEMUSIC] > 0)
        && (AudioChans::GetChannelIfPlaying(SCHAN_MUSIC) != nullptr)
        && (current_music_type != 0)
        && (current_music_type != MUS_MIDI)
        && (current_music_type != MUS_MOD)) {

        crossFading = -1;
        crossFadeStep = 0;
        crossFadeVolumePerStep = game.options[OPT_CROSSFADEMUSIC];
        crossFadeVolumeAtStart = calculate_max_volume();
    }
    else
        stop_and_destroy_channel (SCHAN_MUSIC);

    play.cur_music_number = -1;
    current_music_type = 0;
}

void update_music_volume()
{
    if ((current_music_type) || (crossFading < 0)) 
    {
        // targetVol is the maximum volume we're fading in to
        // newvol is the starting volume that we faded out from
        int targetVol = calculate_max_volume();
        int newvol;
        if (crossFading)
            newvol = crossFadeVolumeAtStart;
        else
            newvol = targetVol;

        // fading out old track, target volume is silence
        if (crossFading < 0)
            targetVol = 0;

        if (crossFading) {
            int curvol = crossFadeVolumePerStep * crossFadeStep;

            if ((curvol > targetVol) && (curvol > newvol)) {
                // it has fully faded to the new track
                newvol = targetVol;
                stop_and_destroy_channel_ex(SCHAN_MUSIC, false);
                if (crossFading > 0) {
                    AudioChans::MoveChannel(SCHAN_MUSIC, crossFading);
                }
                crossFading = 0;
            }
            else {
                if (crossFading > 0)
                {
                    auto *ch = AudioChans::GetChannel(crossFading);
                    if (ch)
                        ch->set_volume255((curvol > targetVol) ? targetVol : curvol);
                }

                newvol -= curvol;
                if (newvol < 0)
                    newvol = 0;
            }
        }
        auto *ch = AudioChans::GetChannel(SCHAN_MUSIC);
        if (ch)
            ch->set_volume255(newvol);
    }
}

// Ensures crossfader is stable after loading (or failing to load) new music
// NOTE: part of the legacy audio logic
void post_new_music_check()
{
    if ((crossFading > 0) && (AudioChans::GetChannel(crossFading) == nullptr)) {
        crossFading = 0;
        // Was fading out but then they played invalid music, continue to fade out
        if (AudioChans::GetChannel(SCHAN_MUSIC) != nullptr)
            crossFading = -1;
    }

}

int prepare_for_new_music ()
{
    int useChannel = SCHAN_MUSIC;

    if ((game.options[OPT_CROSSFADEMUSIC] > 0)
        && (AudioChans::GetChannelIfPlaying(SCHAN_MUSIC) != nullptr)
        && (current_music_type != MUS_MIDI)
        && (current_music_type != MUS_MOD)) {

            if (crossFading > 0) {
                // It's still crossfading to the previous track
                stop_and_destroy_channel_ex(SCHAN_MUSIC, false);
                AudioChans::MoveChannel(SCHAN_MUSIC, crossFading);
                crossFading = 0;
                update_music_volume();
            }
            else if (crossFading < 0) {
                // an old track is still fading out, no new music yet
                // Do nothing, and keep the current crossfade step
            }
            else {
                // start crossfading
                crossFadeStep = 0;
                crossFadeVolumePerStep = game.options[OPT_CROSSFADEMUSIC];
                crossFadeVolumeAtStart = calculate_max_volume();
            }
            useChannel = SPECIAL_CROSSFADE_CHANNEL;
            crossFading = useChannel;
    }
    else {
        // crossfading is now turned off
        stopmusic();
        // ensure that any traces of old tunes fading are eliminated
        // (otherwise the new track will be faded out)
        crossFading = 0;
    }

    // Just make sure, because it will be overwritten in a sec
    if (AudioChans::GetChannel(useChannel) != nullptr)
        stop_and_destroy_channel (useChannel);

    return useChannel;
}

ScriptAudioClip *get_audio_clip_for_music(int mnum)
{
    if (mnum >= QUEUED_MUSIC_REPEAT)
        mnum -= QUEUED_MUSIC_REPEAT;
    return GetAudioClipForOldStyleNumber(game, true, mnum);
}

std::unique_ptr<SoundClip> load_music_from_disk(int mnum, bool doRepeat)
{
    if (mnum >= QUEUED_MUSIC_REPEAT) {
        mnum -= QUEUED_MUSIC_REPEAT;
        doRepeat = true;
    }

    std::unique_ptr<SoundClip> loaded = load_sound_clip_from_old_style_number(true, mnum, doRepeat);

    if ((loaded == nullptr) && (mnum > 0)) 
    {
        debug_script_warn("Warning: failed to load music %d", mnum);
    }

    return loaded;
}

static void play_new_music(int mnum, std::unique_ptr<SoundClip> &&music)
{
    if (debug_flags & DBG_NOMUSIC)
        return;

    if ((play.cur_music_number == mnum) && (music == nullptr)) {
        debug_script_log("PlayMusic %d but already playing", mnum);
        return;  // don't play the music if it's already playing
    }

    ScriptAudioClip *aclip = get_audio_clip_for_music(mnum);
    if (aclip && !is_audiotype_allowed_to_play((AudioFileType)aclip->fileType))
        return;

    int useChannel = SCHAN_MUSIC;
    debug_script_log("Playing music %d", mnum);

    if (mnum<0) {
        stopmusic();
        return;
    }

    if (play.fast_forward) {
        // while skipping cutscene, don't change the music
        play.end_cutscene_music = mnum;
        return;
    }

    useChannel = prepare_for_new_music();
    play.cur_music_number = mnum;
    current_music_type = 0;

    play.current_music_repeating = play.music_repeat;
    // now that all the previous music is unloaded, load in the new one

    std::unique_ptr<SoundClip> new_clip;
    if (music != nullptr)
        new_clip = std::move(music);
    else
        new_clip = load_music_from_disk(mnum, (play.music_repeat > 0));

    if (new_clip && new_clip->play())
    {
        current_music_type = new_clip->get_sound_type();
        AudioChans::SetChannel(useChannel, std::move(new_clip));
    }
    else
    { // previous behavior was to set channel[] to null on error, so continue to do that here.
        AudioChans::DeleteClipOnChannel(useChannel);
    }

    post_new_music_check();
    update_music_volume();
}

void newmusic(int mnum)
{
    play_new_music(mnum, nullptr);
}
