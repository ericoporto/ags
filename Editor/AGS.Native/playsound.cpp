

#include "SDL.h"
#include "SDL_sound.h"


// We need to implement this in some way, interface picked from IrrklangPlayer.cs
// The editor currently can play sounds simultaneously
// So we need to be able to generate this object and dispose of it per AudioClip that is loaded there
class SoundPlayer
{
    void Play()
    {
        // editor currently can't loop playback so this is really just a play until finished
    }

    bool IsPlaying()
    {
    }

    int GetPositionMs()
    {
    }

    int GetLengthMs()
    {
    }

    void Pause()
    {
    }

    void Resume()
    {
    }

    void Stop()
    {
    }

    void Poll()
    {
    }

    void Dispose()
    {
    }
};

// we need to create some c-like interface to be able to interact
// then this interface has to be available in AGS Native
// and later IrrklangPlayer.cs has to be rewritten as AudioClipPlayer.cs
// with calls to AGS Native