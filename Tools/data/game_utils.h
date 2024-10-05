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
#ifndef __AGS_TOOL_DATA__GAMEUTIL_H
#define __AGS_TOOL_DATA__GAMEUTIL_H

#include <vector>
#include "util/string.h"

namespace AGS
{
namespace DataUtil
{

using AGS::Common::String;

// EntityRef is a parent struct for a game object data;
// contains common fields such as a numeric ID (aka index) and script name.
struct EntityRef
{
    String TypeName; // name of type, for the reference when necessary
    int ID = -1;
    String ScriptName;
};

typedef EntityRef CharacterRef;

// DialogRef contains only Dialog data strictly necessary for generating scripts.
// NOTE: replace with full Dialog struct later if appears necessary
struct DialogRef : EntityRef
{
    int OptionCount = 0;
};

// GUIRef contains only GUI data strictly necessary for generating scripts.
// NOTE: replace with full GUI struct later if appears necessary
struct GUIRef : EntityRef
{
    std::vector<EntityRef> Controls;
};

// Game variable (for variables defined in the game project)
struct Variable
{
    String Type;
    String Name;
    String Value;
};

// Game settings
struct GameSettings
{
    bool AllowRelativeAssetResolutions;
    bool AlwaysDisplayTextAsSpeech;
    int AndroidAppVersionCode;
    String AndroidAppVersionName;
    String AndroidApplicationId;
    String AndroidBuildFormat;
    bool AntiAliasFonts;
    bool AntiGlideMode;
    bool AttachDataToExe;
    int AudioIndexer;
    bool AutoMoveInWalkMode;
    bool BackwardsText;
    String BuildTargets;
    bool ClipGUIControls;
    String TrueColor;
    String CompressSpritesType;
    String CustomDataDir;
    String CustomResolution;
    bool DebugMode;
    int DefaultRoomMaskResolution;
    String Description;
    String DeveloperName;
    String DeveloperURL;
    bool DialogOptionsBackwards;
    int DialogOptionsBullet;
    int DialogOptionsGUI;
    int DialogOptionsGap;
    String DialogScriptNarrateFunction; // Custom narrate function name
    String DialogScriptSayFunction; // Custom speech function name
    bool DisplayMultipleInventory;
    bool EnforceNewAudio;
    bool EnforceNewStrings;
    bool EnforceObjectBasedScript;
    String GUIAlphaStyle;
    String GUIDAsString;
    String GameFileName;
    String GameName;
    String GameTextEncoding;
    String Genre;
    int GlobalSpeechAnimationDelay;
    bool HandleInvClicksInScript;
    bool InventoryCursors;
    unsigned int InventoryHotspotMarkerCrosshairColor;
    unsigned int InventoryHotspotMarkerDotColor;
    int InventoryHotspotMarkerSprite;
    String InventoryHotspotMarkerStyle;
    bool LeftToRightPrecedence;
    bool LetterboxMode;
    int MaximumScore;
    bool MouseWheelEnabled;
    String NumberDialogOptions;
    bool OptimizeSpriteStorage;
    bool PixelPerfect;
    int PlaySoundOnScore;
    String ReleaseDate;
    String RenderAtScreenResolution;
    String RoomTransition;
    bool RunGameLoopsWhileDialogOptionsDisplayed;
    String SaveGameFileExtension;
    String SaveGameFolderName;
    bool SaveScreenshots;
    bool ScaleCharacterSpriteOffsets;
    bool ScaleMovementSpeedWithMaskResolution;
    String ScriptAPIVersion;
    String ScriptCompatLevel;
    String SkipSpeech;
    String SpeechPortraitSide;
    String SpeechStyle;
    String SplitResources;
    String SpriteAlphaStyle;
    String TTFHeightDefinedBy;
    String TTFMetricsFixup;
    int TextWindowGUI;
    int ThoughtGUI;
    bool TurnBeforeFacing;
    bool TurnBeforeWalking;
    int UniqueID;
    bool UseGlobalSpeechAnimationDelay;
    bool UseLowResCoordinatesInScript;
    bool UseOldCustomDialogOptionsAPI;
    bool UseOldKeyboardHandling;
    String Version;
    bool WalkInLookMode;
    String WhenInterfaceDisabled;
};

// GameRef contains only game data strictly necessary for generating scripts.
// NOTE: replace with full Game struct later if appears necessary
struct GameRef
{
    std::vector<EntityRef> AudioClips;
    std::vector<EntityRef> AudioTypes;
    std::vector<CharacterRef> Characters;
    std::vector<EntityRef> Cursors;
    std::vector<DialogRef> Dialogs;
    std::vector<EntityRef> Fonts;
    std::vector<GUIRef>    GUI;
    std::vector<EntityRef> Inventory;
    std::vector<EntityRef> Views;

    std::vector<Variable>  GlobalVars;

    GameSettings           Settings;
};

} // namespace DataUtil
} // namespace AGS

#endif // __AGS_TOOL_DATA__GAMEUTIL_H
