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
#ifndef __AGS_EE_AC__CHARACTER_H
#define __AGS_EE_AC__CHARACTER_H

#include "ac/characterinfo.h"
#include "ac/characterextras.h"
#include "ac/display.h"
#include "ac/dynobj/scriptobject.h"
#include "ac/dynobj/scriptinvitem.h"
#include "ac/dynobj/scriptoverlay.h"
#include "game/viewport.h"
#include "util/geometry.h"

// **** CHARACTER: FUNCTIONS ****

bool    is_valid_character(int char_id);
// Asserts the character ID is valid,
// if not then prints a warning to the log; returns assertion result
bool    AssertCharacter(const char *apiname, int char_id);

void    Character_AddInventory(CharacterInfo *chaa, ScriptInvItem *invi, int addIndex);
void    Character_AddWaypoint(CharacterInfo *chaa, int x, int y);
void    Character_Animate(CharacterInfo *chaa, int loop, int delay, int repeat,
                          int blocking, int direction, int sframe = 0, int volume = 100);
void    Character_Animate5(CharacterInfo *chaa, int loop, int delay, int repeat, int blocking, int direction);
void    Character_ChangeRoomAutoPosition(CharacterInfo *chaa, int room, int newPos);
void    Character_ChangeRoom(CharacterInfo *chaa, int room, int x, int y);
void    Character_ChangeRoomSetLoop(CharacterInfo *chaa, int room, int x, int y, int direction);
void    Character_ChangeView(CharacterInfo *chap, int vii);
void    Character_FaceDirection(CharacterInfo *char1, int direction, int blockingStyle);
void    Character_FaceCharacter(CharacterInfo *char1, CharacterInfo *char2, int blockingStyle);
void    Character_FaceLocation(CharacterInfo *char1, int xx, int yy, int blockingStyle);
void    Character_FaceObject(CharacterInfo *char1, ScriptObject *obj, int blockingStyle);
void    Character_FollowCharacter(CharacterInfo *chaa, CharacterInfo *tofollow, int distaway, int eagerness);
int     Character_IsCollidingWithChar(CharacterInfo *char1, CharacterInfo *char2);
int     Character_IsCollidingWithObject(CharacterInfo *chin, ScriptObject *objid);
bool    Character_IsInteractionAvailable(CharacterInfo *cchar, int mood);
void    Character_LockView(CharacterInfo *chap, int vii);
void    Character_LockViewEx(CharacterInfo *chap, int vii, int stopMoving);
void    Character_LockViewAligned(CharacterInfo *chap, int vii, int loop, int align);
void    Character_LockViewAlignedEx(CharacterInfo *chap, int vii, int loop, int align, int stopMoving);
void    Character_LockViewFrame(CharacterInfo *chaa, int view, int loop, int frame);
void    Character_LockViewFrameEx(CharacterInfo *chaa, int view, int loop, int frame, int stopMoving);
void    Character_LockViewOffset(CharacterInfo *chap, int vii, int xoffs, int yoffs);
void    Character_LockViewOffsetEx(CharacterInfo *chap, int vii, int xoffs, int yoffs, int stopMoving);
void    Character_LoseInventory(CharacterInfo *chap, ScriptInvItem *invi);
void    Character_PlaceOnWalkableArea(CharacterInfo *chap);
void    Character_RemoveTint(CharacterInfo *chaa);
int     Character_GetHasExplicitTint(CharacterInfo *chaa);
void    Character_Say(CharacterInfo *chaa, const char *text);
void    Character_SayAt(CharacterInfo *chaa, int x, int y, int width, const char *texx);
ScriptOverlay* Character_SayBackground(CharacterInfo *chaa, const char *texx);
void    Character_SetAsPlayer(CharacterInfo *chaa);
void    Character_SetIdleView(CharacterInfo *chaa, int iview, int itime);
void    Character_SetOption(CharacterInfo *chaa, int flag, int yesorno);
void    Character_SetSpeed(CharacterInfo *chaa, int xspeed, int yspeed);
void    Character_StopMoving(CharacterInfo *charp);
void    Character_StopMovingEx(CharacterInfo *charp, bool force_walkable_area);
void    Character_Tint(CharacterInfo *chaa, int red, int green, int blue, int opacity, int luminance);
void    Character_Think(CharacterInfo *chaa, const char *text);
void    Character_UnlockView(CharacterInfo *chaa);
void    Character_UnlockViewEx(CharacterInfo *chaa, int stopMoving);
void    Character_Walk(CharacterInfo *chaa, int x, int y, int blocking, int ignwal);
void    Character_Move(CharacterInfo *chaa, int x, int y, int blocking, int ignwal);
void    Character_WalkStraight(CharacterInfo *chaa, int xx, int yy, int blocking);

void    Character_RunInteraction(CharacterInfo *chaa, int mood);

// **** CHARACTER: PROPERTIES ****

int Character_GetProperty(CharacterInfo *chaa, const char *property);
void Character_GetPropertyText(CharacterInfo *chaa, const char *property, char *bufer);
const char* Character_GetTextProperty(CharacterInfo *chaa, const char *property);

ScriptInvItem* Character_GetActiveInventory(CharacterInfo *chaa);
void    Character_SetActiveInventory(CharacterInfo *chaa, ScriptInvItem* iit);
int     Character_GetAnimating(CharacterInfo *chaa);
int     Character_GetAnimationSpeed(CharacterInfo *chaa);
void    Character_SetAnimationSpeed(CharacterInfo *chaa, int newval);
int     Character_GetBaseline(CharacterInfo *chaa);
void    Character_SetBaseline(CharacterInfo *chaa, int basel);
int     Character_GetBlinkInterval(CharacterInfo *chaa);
void    Character_SetBlinkInterval(CharacterInfo *chaa, int interval);
int     Character_GetBlinkView(CharacterInfo *chaa);
void    Character_SetBlinkView(CharacterInfo *chaa, int vii);
int     Character_GetBlinkWhileThinking(CharacterInfo *chaa);
void    Character_SetBlinkWhileThinking(CharacterInfo *chaa, int yesOrNo);
int     Character_GetBlockingHeight(CharacterInfo *chaa);
void    Character_SetBlockingHeight(CharacterInfo *chaa, int hit);
int     Character_GetBlockingWidth(CharacterInfo *chaa);
void    Character_SetBlockingWidth(CharacterInfo *chaa, int wid);
int     Character_GetDiagonalWalking(CharacterInfo *chaa);
void    Character_SetDiagonalWalking(CharacterInfo *chaa, int yesorno);
int     Character_GetClickable(CharacterInfo *chaa);
void    Character_SetClickable(CharacterInfo *chaa, int clik);
int     Character_GetID(CharacterInfo *chaa);
int     Character_GetFrame(CharacterInfo *chaa);
void    Character_SetFrame(CharacterInfo *chaa, int newval);
int     Character_GetIdleView(CharacterInfo *chaa);
int     Character_GetIInventoryQuantity(CharacterInfo *chaa, int index);
int     Character_HasInventory(CharacterInfo *chaa, ScriptInvItem *invi);
void    Character_SetIInventoryQuantity(CharacterInfo *chaa, int index, int quant);
int     Character_GetIgnoreLighting(CharacterInfo *chaa);
void    Character_SetIgnoreLighting(CharacterInfo *chaa, int yesorno);
int     Character_GetIgnoreScaling(CharacterInfo *chaa);
void    Character_SetIgnoreScaling(CharacterInfo *chaa, int yesorno);
void    Character_SetManualScaling(CharacterInfo *chaa, int yesorno);
int     Character_GetIgnoreWalkbehinds(CharacterInfo *chaa);
void    Character_SetIgnoreWalkbehinds(CharacterInfo *chaa, int yesorno);
int     Character_GetMovementLinkedToAnimation(CharacterInfo *chaa);
void    Character_SetMovementLinkedToAnimation(CharacterInfo *chaa, int yesorno);
int     Character_GetLoop(CharacterInfo *chaa);
void    Character_SetLoop(CharacterInfo *chaa, int newval);
int     Character_GetMoving(CharacterInfo *chaa);
const char* Character_GetName(CharacterInfo *chaa);
void    Character_SetName(CharacterInfo *chaa, const char *newName);
int     Character_GetNormalView(CharacterInfo *chaa);
int     Character_GetPreviousRoom(CharacterInfo *chaa);
int     Character_GetRoom(CharacterInfo *chaa);
int     Character_GetScaleMoveSpeed(CharacterInfo *chaa);
void    Character_SetScaleMoveSpeed(CharacterInfo *chaa, int yesorno);
int     Character_GetScaleVolume(CharacterInfo *chaa);
void    Character_SetScaleVolume(CharacterInfo *chaa, int yesorno);
int     Character_GetScaling(CharacterInfo *chaa);
void    Character_SetScaling(CharacterInfo *chaa, int zoomlevel);
int     Character_GetSolid(CharacterInfo *chaa);
void    Character_SetSolid(CharacterInfo *chaa, int yesorno);
int     Character_GetSpeaking(CharacterInfo *chaa);
int     Character_GetSpeechColor(CharacterInfo *chaa);
void    Character_SetSpeechColor(CharacterInfo *chaa, int ncol);
void    Character_SetSpeechAnimationDelay(CharacterInfo *chaa, int newDelay);
int     Character_GetSpeechView(CharacterInfo *chaa);
void    Character_SetSpeechView(CharacterInfo *chaa, int vii);
int     Character_GetThinkView(CharacterInfo *chaa);
void    Character_SetThinkView(CharacterInfo *chaa, int vii);
int     Character_GetTransparency(CharacterInfo *chaa);
void    Character_SetTransparency(CharacterInfo *chaa, int trans);
int     Character_GetTurnBeforeWalking(CharacterInfo *chaa);
void    Character_SetTurnBeforeWalking(CharacterInfo *chaa, int yesorno);
int     Character_GetView(CharacterInfo *chaa);
int     Character_GetWalkSpeedX(CharacterInfo *chaa);
int     Character_GetWalkSpeedY(CharacterInfo *chaa);
int     Character_GetX(CharacterInfo *chaa);
void    Character_SetX(CharacterInfo *chaa, int newval);
int     Character_GetY(CharacterInfo *chaa);
void    Character_SetY(CharacterInfo *chaa, int newval);
int     Character_GetZ(CharacterInfo *chaa);
void    Character_SetZ(CharacterInfo *chaa, int newval);
int     Character_GetSpeakingFrame(CharacterInfo *chaa);

//=============================================================================

class MoveList;
namespace AGS { namespace Common { class Bitmap; } }
using namespace AGS; // FIXME later

// Configures and starts character animation.
void animate_character(CharacterInfo *chap, int loopn, int sppd, int rept,
    int direction = 0, int sframe = 0, int volume = 100);
// Clears up animation parameters
void stop_character_anim(CharacterInfo *chap);
int  find_looporder_index (int curloop);
// returns 0 to use diagonal, 1 to not
int  useDiagonal (CharacterInfo *char1);
// returns 1 normally, or 0 if they only have horizontal animations
int  hasUpDownLoops(CharacterInfo *char1);
void start_character_turning (CharacterInfo *chinf, int useloop, int no_diagonal);
void fix_player_sprite(MoveList*cmls,CharacterInfo*chinf);
// Check whether two characters have walked into each other
int  has_hit_another_character(int sourceChar);
int  doNextCharMoveStep(CharacterInfo *chi, CharacterExtras *chex);
// Tells if character is currently moving, in eWalkableAreas mode
bool is_char_walking_ndirect(CharacterInfo *chi);
bool FindNearestWalkableAreaForCharacter(const Point &src, Point &dst, bool force_move);
void FindReasonableLoopForCharacter(CharacterInfo *chap);
// Start character walk or move; calculate path using destination and optionally "ignore walls" flag
void move_character(CharacterInfo *chaa, int tox, int toy, bool ignwal, bool walk_anim);
// Start character walk or move along the straight line until any non-passable area is met
void move_character_straight(CharacterInfo *chaa, int x, int y, bool walk_anim);
// Start character walk; calculate path using destination and optionally "ignore walls" flag
void walk_character(CharacterInfo *chaa, int tox, int toy, bool ignwal);
// Start character walk the straight line until any non-passable area is met
void walk_character_straight(CharacterInfo *chaa, int tox, int toy);
int  wantMoveNow (CharacterInfo *chi, CharacterExtras *chex);
void setup_player_character(int charid);
Common::Bitmap *GetCharacterImage(int charid, bool *is_original = nullptr);
CharacterInfo *GetCharacterAtScreen(int xx, int yy);
// Deduces room object's scale, accounting for both manual scaling and the room region effects;
// calculates resulting sprite size.
void update_character_scale(int charid);
// Get character ID at the given room coordinates
int is_pos_on_character(int xx,int yy);
void get_char_blocking_rect(int charid, int *x1, int *y1, int *width, int *y2);
// Check whether the source char is standing inside otherChar's blocking rectangle
int is_char_in_blocking_rect(int sourceChar, int otherChar, int *fromxptr, int *cwidptr);
int my_getpixel(Common::Bitmap *blk, int x, int y);
int check_click_on_character(int xx, int yy,int mood);
int is_pos_on_character(int xx,int yy);
void DisplaySpeechCore(int chid, const char *displbuf);
void DisplayThoughtCore(int chid, const char *displbuf);
// Displays character speech or thought message.
// When auto-position flag is on, passing negative coordinates will request
// auto-position on the respective axis. The nature of auto-pos will depend
// on a speech style. E.g. LA speech aligns above character's head.
void display_speech(const char *texx, int aschar, int xx, int yy, int widd, bool auto_position, bool is_thought);
int get_character_currently_talking();
void DisplaySpeech(const char*texx, int aschar);
int update_lip_sync(int talkview, int talkloop, int *talkframeptr);

// Recalculate dynamic character properties, e.g. after restoring a game save
void restore_characters();

// Calculates character's bounding box in room coordinates (takes only in-room transform into account)
// use_frame_0 optionally tells to use frame 0 of current loop instead of current frame.
Rect GetCharacterRoomBBox(int charid, bool use_frame_0 = false);
// Find a closest viewport given character is to. Checks viewports in their order in game's array,
// and returns either first viewport character's bounding box intersects with (or rather with its camera),
// or the one that is least far away from its camera; calculated as a perpendicular distance between two AABBs.
PViewport FindNearestViewport(int charid);

// Character_DoMove converts and validates script parameters, and calls corresponding internal character move function
void Character_DoMove(CharacterInfo *chaa, const char *api_name,
    int x, int y, bool walk_straight, int blocking, int ignwal, bool walk_anim);

//
// Character update functions
// TODO: move these into a runtime Character class, when there will be a proper one,
// merging CharacterInfo and CharacterExtras.
//
void UpdateCharacterMoveAndAnim(CharacterInfo *chi, CharacterExtras *chex, std::vector<int> &followingAsSheep);
void UpdateFollowingExactlyCharacter(CharacterInfo *chi);
bool UpdateCharacterTurning(CharacterInfo *chi, CharacterExtras *chex);
void UpdateCharacterMoving(CharacterInfo *chi, CharacterExtras *chex, int &doing_nothing);
bool UpdateCharacterAnimating(CharacterInfo *chi, CharacterExtras *chex, int &doing_nothing);
void UpdateCharacterIdle(CharacterInfo *chi, CharacterExtras *chex, int &doing_nothing);
void UpdateCharacterFollower(CharacterInfo *chi, std::vector<int> &followingAsSheep, int &doing_nothing);

extern CharacterInfo*playerchar;
extern int32_t _sc_PlayerCharPtr;

// order of loops to turn character in circle from down to down
extern const int turnlooporder[8];

#endif // __AGS_EE_AC__CHARACTER_H
