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
//
// AGS Character functions
//
//=============================================================================

#include "ac/global_character.h"
#include "ac/common.h"
#include "ac/view.h"
#include "ac/character.h"
#include "ac/display.h"
#include "ac/draw.h"
#include "ac/event.h"
#include "ac/gamesetupstruct.h"
#include "ac/gamestate.h"
#include "ac/global_overlay.h"
#include "ac/global_translation.h"
#include "ac/gui.h"
#include "ac/object.h"
#include "ac/overlay.h"
#include "ac/properties.h"
#include "ac/screenoverlay.h"
#include "ac/string.h"
#include "ac/dynobj/cc_character.h"
#include "debug/debug_log.h"
#include "game/roomstruct.h"
#include "main/game_run.h"
#include "script/script.h"

using namespace AGS::Common;
using namespace AGS::Engine;


extern GameSetupStruct game;
extern std::vector<ViewStruct> views;
extern RoomObject*objs;
extern RoomStruct thisroom;
extern ScriptObject scrObj[MAX_ROOM_OBJECTS];
extern ScriptInvItem scrInv[MAX_INV];
extern CCCharacter ccDynamicCharacter;

// defined in character unit
extern CharacterInfo*playerchar;
extern int32_t _sc_PlayerCharPtr;
extern CharacterInfo*playerchar;


void StopMoving(int chaa) {

    Character_StopMoving(&game.chars[chaa]);
}

void ReleaseCharacterView(int chat) {
    if (!is_valid_character(chat))
        quit("!ReleaseCahracterView: invalid character supplied");

    Character_UnlockView(&game.chars[chat]);
}

void MoveToWalkableArea(int charid) {
    if (!is_valid_character(charid))
        quit("!MoveToWalkableArea: invalid character specified");

    Character_PlaceOnWalkableArea(&game.chars[charid]);
}

void FaceLocation(int cha, int xx, int yy) {
    if (!is_valid_character(cha))
        quit("!FaceLocation: Invalid character specified");

    Character_FaceLocation(&game.chars[cha], xx, yy, BLOCKING);
}

void FaceCharacter(int cha,int toface) {
    if (!is_valid_character(cha))
        quit("!FaceCharacter: Invalid character specified");
    if (!is_valid_character(toface)) 
        quit("!FaceCharacter: invalid character specified");

    Character_FaceCharacter(&game.chars[cha], &game.chars[toface], BLOCKING);
}


void SetCharacterIdle(int who, int iview, int itime) {
    if (!is_valid_character(who))
        quit("!SetCharacterIdle: Invalid character specified");

    Character_SetIdleView(&game.chars[who], iview, itime);
}



int GetCharacterWidth(int ww) {
    CharacterInfo *char1 = &game.chars[ww];

    if (charextra[ww].width < 1)
    {
        if ((char1->view < 0) ||
            (char1->loop >= views[char1->view].numLoops) ||
            (char1->frame >= views[char1->view].loops[char1->loop].numFrames))
        {
            debug_script_warn("GetCharacterWidth: Character %s has invalid frame: view %d, loop %d, frame %d",
                char1->scrname, char1->view + 1, char1->loop, char1->frame);
            return data_to_game_coord(4);
        }

        return game.SpriteInfos[views[char1->view].loops[char1->loop].frames[char1->frame].pic].Width;
    }
    else 
        return charextra[ww].width;
}

int GetCharacterHeight(int charid) {
    CharacterInfo *char1 = &game.chars[charid];

    if (charextra[charid].height < 1)
    {
        if ((char1->view < 0) ||
            (char1->loop >= views[char1->view].numLoops) ||
            (char1->frame >= views[char1->view].loops[char1->loop].numFrames))
        {
            debug_script_warn("GetCharacterHeight: Character %s has invalid frame: view %d, loop %d, frame %d",
                char1->scrname, char1->view + 1, char1->loop, char1->frame);
            return data_to_game_coord(2);
        }

        return game.SpriteInfos[views[char1->view].loops[char1->loop].frames[char1->frame].pic].Height;
    }
    else
        return charextra[charid].height;
}



void SetCharacterBaseline (int obn, int basel) {
    if (!is_valid_character(obn)) quit("!SetCharacterBaseline: invalid object number specified");

    Character_SetBaseline(&game.chars[obn], basel);
}

// pass trans=0 for fully solid, trans=100 for fully transparent
void SetCharacterTransparency(int obn,int trans) {
    if (!is_valid_character(obn))
        quit("!SetCharTransparent: invalid character number specified");

    Character_SetTransparency(&game.chars[obn], trans);
}

void AnimateCharacter4(int chh, int loopn, int sppd, int rept) {
    AnimateCharacter6(chh, loopn, sppd, rept, FORWARDS, IN_BACKGROUND);
}

void AnimateCharacter6(int chh, int loopn, int sppd, int rept, int direction, int blocking) {
    if (!is_valid_character(chh))
        quit("AnimateCharacter: invalid character");

    Character_Animate5(&game.chars[chh], loopn, sppd, rept, blocking, direction);
}

void SetPlayerCharacter(int newchar) {
    if (!is_valid_character(newchar))
        quit("!SetPlayerCharacter: Invalid character specified");

    Character_SetAsPlayer(&game.chars[newchar]);
}

void FollowCharacterEx(int who, int tofollow, int distaway, int eagerness) {
    if (!is_valid_character(who))
        quit("!FollowCharacter: Invalid character specified");
    CharacterInfo *chtofollow = nullptr;
    if (tofollow != -1)
    {
        if (!is_valid_character(tofollow))
            quit("!FollowCharacterEx: invalid character to follow");
        else
            chtofollow = &game.chars[tofollow];
    }

    Character_FollowCharacter(&game.chars[who], chtofollow, distaway, eagerness);
}

void FollowCharacter(int who, int tofollow) {
    FollowCharacterEx(who,tofollow,10,97);
}

void SetCharacterIgnoreLight (int who, int yesorno) {
    if (!is_valid_character(who))
        quit("!SetCharacterIgnoreLight: Invalid character specified");

    Character_SetIgnoreLighting(&game.chars[who], yesorno);
}

void MoveCharacter(int cc,int x, int y)
{
    if (!is_valid_character(cc))
        quit("!MoveCharacter: invalid character specified");

    Character_DoMove(&game.chars[cc], "MoveCharacter", x, y,
                     false /* not straight */, IN_BACKGROUND, WALKABLE_AREAS, true /* animate */);
}

void MoveCharacterDirect(int cc, int x, int y)
{
    if (!is_valid_character(cc))
        quit("!MoveCharacterDirect: invalid character specified");

    Character_DoMove(&game.chars[cc], "MoveCharacterDirect", x, y,
                     false /* not straight */, IN_BACKGROUND, ANYWHERE, true /* animate */);
}

void MoveCharacterStraight(int cc,int x, int y)
{
    if (!is_valid_character(cc))
        quit("!MoveCharacterStraight: invalid character specified");

    Character_DoMove(&game.chars[cc], "MoveCharacterStraight", x, y,
                     true /* straight */, IN_BACKGROUND, WALKABLE_AREAS, true /* animate */);
}

// Append to character path
void MoveCharacterPath(int chac, int tox, int toy)
{
    if (!is_valid_character(chac))
        quit("!MoveCharacterPath: invalid character specified");

    Character_AddWaypoint(&game.chars[chac], tox, toy);
}


int GetPlayerCharacter() {
    return game.playercharacter;
}

void SetCharacterSpeedEx(int chaa, int xspeed, int yspeed) {
    if (!is_valid_character(chaa))
        quit("!SetCharacterSpeedEx: invalid character");

    Character_SetSpeed(&game.chars[chaa], xspeed, yspeed);

}

void SetCharacterSpeed(int chaa,int nspeed) {
    SetCharacterSpeedEx(chaa, nspeed, nspeed);
}

void SetTalkingColor(int chaa,int ncol) {
    if (!is_valid_character(chaa)) quit("!SetTalkingColor: invalid character");

    Character_SetSpeechColor(&game.chars[chaa], ncol);
}

void SetCharacterSpeechView (int chaa, int vii) {
    if (!is_valid_character(chaa))
        quit("!SetCharacterSpeechView: invalid character specified");

    Character_SetSpeechView(&game.chars[chaa], vii);
}

void SetCharacterBlinkView (int chaa, int vii, int intrv) {
    if (!is_valid_character(chaa))
        quit("!SetCharacterBlinkView: invalid character specified");

    Character_SetBlinkView(&game.chars[chaa], vii);
    Character_SetBlinkInterval(&game.chars[chaa], intrv);
}

void SetCharacterView(int chaa,int vii) {
    if (!is_valid_character(chaa))
        quit("!SetCharacterView: invalid character specified");

    Character_LockView(&game.chars[chaa], vii);
}

void SetCharacterFrame(int chaa, int view, int loop, int frame) {

    Character_LockViewFrame(&game.chars[chaa], view, loop, frame);
}

// similar to SetCharView, but aligns the frame to make it line up
void SetCharacterViewEx (int chaa, int vii, int loop, int align) {

    Character_LockViewAligned(&game.chars[chaa], vii, loop, ConvertLegacyScriptAlignment((LegacyScriptAlignment)align));
}

void SetCharacterViewOffset (int chaa, int vii, int xoffs, int yoffs) {

    Character_LockViewOffset(&game.chars[chaa], vii, xoffs, yoffs);
}


void ChangeCharacterView(int chaa,int vii) {
    if (!is_valid_character(chaa))
        quit("!ChangeCharacterView: invalid character specified");

    Character_ChangeView(&game.chars[chaa], vii);
}

void SetCharacterClickable (int cha, int clik) {
    if (!is_valid_character(cha))
        quit("!SetCharacterClickable: Invalid character specified");
    // make the character clicklabe (reset "No interaction" bit)
    game.chars[cha].flags&=~CHF_NOINTERACT;
    // if they don't want it clickable, set the relevant bit
    if (clik == 0)
        game.chars[cha].flags|=CHF_NOINTERACT;
}

void SetCharacterIgnoreWalkbehinds (int cha, int clik) {
    if (!is_valid_character(cha))
        quit("!SetCharacterIgnoreWalkbehinds: Invalid character specified");

    Character_SetIgnoreWalkbehinds(&game.chars[cha], clik);
}


void MoveCharacterToObject(int chaa,int obbj)
{
    // invalid object, do nothing
    // this allows MoveCharacterToObject(EGO, GetObjectAt(...));
    if (!is_valid_object(obbj))
        return;

    // NOTE: yep, it was using a hardcoded +5,+6 relative position...
    Character_DoMove(&game.chars[chaa], "MoveCharacterToObject", objs[obbj].x + 5, objs[obbj].y + 6,
                     false /* not straight */, BLOCKING, WALKABLE_AREAS, true /* animate */);
}

void MoveCharacterToHotspot(int chaa, int hotsp)
{
    if ((hotsp<0) || (hotsp>=MAX_ROOM_HOTSPOTS))
        quit("!MovecharacterToHotspot: invalid hotspot");
    if (thisroom.Hotspots[hotsp].WalkTo.X < 1)
        return;

    Character_DoMove(&game.chars[chaa], "MoveCharacterToObject", thisroom.Hotspots[hotsp].WalkTo.X, thisroom.Hotspots[hotsp].WalkTo.Y,
                     false /* not straight */, BLOCKING, WALKABLE_AREAS, true /* animate */);
}

int MoveCharacterBlocking(int chaa, int x, int y, int ignwal)
{
    if (!is_valid_character (chaa))
        quit("!MoveCharacterBlocking: invalid character");

    Character_DoMove(&game.chars[chaa], "MoveCharacterBlocking", x, y,
                     false /* not straight */, BLOCKING, ignwal, true /* animate */);

    return -1; // replicates legacy engine effect
}

int GetCharacterSpeechAnimationDelay(CharacterInfo *cha)
{
	if ((loaded_game_file_version < kGameVersion_312) && (game.options[OPT_SPEECHTYPE] != kSpeechStyle_LucasArts))
	{
		// legacy versions of AGS assigned a fixed delay to Sierra-style speech only
		return 5;
	}
	if (game.options[OPT_GLOBALTALKANIMSPD] != 0)
		return play.talkanim_speed;
    else
        return cha->speech_anim_speed;
}

void RunCharacterInteraction (int cc, int mood) {
    if (!is_valid_character(cc))
        quit("!RunCharacterInteraction: invalid character");

    // convert cursor mode to event index (in character event table)
    // TODO: probably move this conversion table elsewhere? should be a global info
    int evnt;
    switch (mood)
    {
    case MODE_LOOK: evnt = 0; break;
    case MODE_HAND: evnt = 1; break;
    case MODE_TALK: evnt = 2; break;
    case MODE_USE: evnt = 3; break;
    case MODE_PICKUP: evnt = 5; break;
    case MODE_CUSTOM1: evnt = 6; break;
    case MODE_CUSTOM2: evnt = 7; break;
    default: evnt = -1; break;
    }
    const int anyclick_evt = 4; // TODO: make global constant (character any-click evt)

    // For USE verb: remember active inventory
    if (mood == MODE_USE)
    {
        play.usedinv = playerchar->activeinv;
    }

    const auto obj_evt = ObjectEvent(kScTypeGame, "character%d", cc,
        RuntimeScriptValue().SetScriptObject(&game.chars[cc], &ccDynamicCharacter), mood);
    if (loaded_game_file_version > kGameVersion_272)
    {
        if ((evnt >= 0) &&
                run_interaction_script(obj_evt, game.charScripts[cc].get(), evnt, anyclick_evt) < 0)
            return; // game state changed, don't do "any click"
        run_interaction_script(obj_evt, game.charScripts[cc].get(), anyclick_evt);  // any click on char
    }
    else 
    {
        if ((evnt >= 0) &&
                run_interaction_event(obj_evt, game.intrChar[cc].get(), evnt, anyclick_evt, (mood == MODE_USE)) < 0)
            return; // game state changed, don't do "any click"
        run_interaction_event(obj_evt, game.intrChar[cc].get(), anyclick_evt);  // any click on char
    }
}

int AreCharObjColliding(int charid,int objid) {
    if (!is_valid_character(charid))
        quit("!AreCharObjColliding: invalid character");
    if (!is_valid_object(objid))
        quit("!AreCharObjColliding: invalid object number");

    return Character_IsCollidingWithObject(&game.chars[charid], &scrObj[objid]);
}

int AreCharactersColliding(int cchar1,int cchar2) {
    if (!is_valid_character(cchar1))
        quit("!AreCharactersColliding: invalid char1");
    if (!is_valid_character(cchar2))
        quit("!AreCharactersColliding: invalid char2");

    return Character_IsCollidingWithChar(&game.chars[cchar1], &game.chars[cchar2]);
}

int GetCharacterProperty (int cha, const char *property) {
    if (!is_valid_character(cha))
        quit("!GetCharacterProperty: invalid character");
    return get_int_property (game.charProps[cha], play.charProps[cha], property);
}

void SetCharacterProperty (int who, int flag, int yesorno) {
    if (!is_valid_character(who))
        quit("!SetCharacterProperty: Invalid character specified");

    Character_SetOption(&game.chars[who], flag, yesorno);
}

void GetCharacterPropertyText (int item, const char *property, char *bufer) {
    get_text_property (game.charProps[item], play.charProps[item], property, bufer);
}

int GetCharIDAtScreen(int xx, int yy) {
    VpPoint vpt = play.ScreenToRoomDivDown(xx, yy);
    if (vpt.second < 0)
        return -1;
    return is_pos_on_character(vpt.first.X, vpt.first.Y);
}

void SetActiveInventory(int iit) {

    ScriptInvItem *tosend = nullptr;
    if ((iit > 0) && (iit < game.numinvitems))
        tosend = &scrInv[iit];
    else if (iit != -1)
        quitprintf("!SetActiveInventory: invalid inventory number %d", iit);

    Character_SetActiveInventory(playerchar, tosend);
}

void update_invorder() {
    for (int cc = 0; cc < game.numcharacters; cc++) {
        charextra[cc].invorder_count = 0;
        int ff, howmany;
        // Iterate through all inv items, adding them once (or multiple
        // times if requested) to the list.
        for (ff=0;ff < game.numinvitems;ff++) {
            howmany = game.chars[cc].inv[ff];
            if ((game.options[OPT_DUPLICATEINV] == 0) && (howmany > 1))
                howmany = 1;

            for (int ts = 0; ts < howmany; ts++) {
                if (charextra[cc].invorder_count >= MAX_INVORDER)
                    quit("!Too many inventory items to display: 500 max");

                charextra[cc].invorder[charextra[cc].invorder_count] = ff;
                charextra[cc].invorder_count++;
            }
        }
    }
    // backwards compatibility
    play.inv_numorder = charextra[game.playercharacter].invorder_count;
    GUIE::MarkInventoryForUpdate(game.playercharacter, true);
}

void add_inventory(int inum) {
    if ((inum < 0) || (inum >= MAX_INV))
        quit("!AddInventory: invalid inventory number");

    Character_AddInventory(playerchar, &scrInv[inum], SCR_NO_VALUE);

    play.inv_numorder = charextra[game.playercharacter].invorder_count;
}

void lose_inventory(int inum) {
    if ((inum < 0) || (inum >= MAX_INV))
        quit("!LoseInventory: invalid inventory number");

    Character_LoseInventory(playerchar, &scrInv[inum]);

    play.inv_numorder = charextra[game.playercharacter].invorder_count;
}

void AddInventoryToCharacter(int charid, int inum) {
    if (!is_valid_character(charid))
        quit("!AddInventoryToCharacter: invalid character specified");
    if ((inum < 1) || (inum >= game.numinvitems))
        quit("!AddInventory: invalid inv item specified");

    Character_AddInventory(&game.chars[charid], &scrInv[inum], SCR_NO_VALUE);
}

void LoseInventoryFromCharacter(int charid, int inum) {
    if (!is_valid_character(charid))
        quit("!LoseInventoryFromCharacter: invalid character specified");
    if ((inum < 1) || (inum >= game.numinvitems))
        quit("!AddInventory: invalid inv item specified");

    Character_LoseInventory(&game.chars[charid], &scrInv[inum]);
}

void DisplayThought(int chid, const char *text) {
    if ((chid < 0) || (chid >= game.numcharacters))
        quit("!DisplayThought: invalid character specified");

    DisplayThoughtCore(chid, text);
}

void __sc_displayspeech(int chid, const char *text) {
    if ((chid<0) || (chid>=game.numcharacters))
        quit("!DisplaySpeech: invalid character specified");

    DisplaySpeechCore(chid, text);
}

void DisplaySpeechAt(int xx, int yy, int wii, int aschar, const char*spch) {
    data_to_game_coords(&xx, &yy);
    wii = data_to_game_coord(wii);
    display_speech(get_translation(spch), aschar, xx, yy, wii, true /*auto-pos*/, false /* not thought */);
}

int DisplaySpeechBackground(int charid, const char*speel) {
    // remove any previous background speech for this character
    // TODO: have a map character -> bg speech over?
    const auto &overs = get_overlays();
    for (size_t i = 0; i < overs.size(); ++i)
    {
        if (overs[i].bgSpeechForChar == charid)
        {
            remove_screen_overlay(i);
            break;
        }
    }

    int ovrl=CreateTextOverlay(OVR_AUTOPLACE,charid,play.GetUIViewport().GetWidth()/2,FONT_SPEECH,
        game.chars[charid].talkcolor, get_translation(speel), kDisplayTextStyle_Overchar);

    auto *over = get_overlay(ovrl);
    over->bgSpeechForChar = charid;
    over->timeout = GetTextDisplayTime(speel, 1);
    return ovrl;
}
