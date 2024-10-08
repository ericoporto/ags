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
// AGS::AGF namespace contains utilities for reading and parsing AGS game
// project format (AGF), which is essentially an XML.
//
// AGFReader opens the file and makes basic check for validity.
//
// EntityListParser and its subclasses parse lists of game entities and return
// collection of their document nodes.
//
// EntityParser and its subclasses parse entity itself. There's a number of
// common properties which may be accessed using EntityParser interface, but
// most of the data is only retrieved by a specific parser subtype.
//
// All the above classes use DocElem: a simple copyable type which references
// particular AGF node. DocElem instances are allocated by AGFReader internally
// and persist as long as AGFReader object does. They do not have to be deleted
// by the user.
//
//=============================================================================
#ifndef __AGS_TOOL_DATA__AGFREADER_H
#define __AGS_TOOL_DATA__AGFREADER_H

#include <memory>
#include "data/game_utils.h"
#include "util/error.h"

namespace tinyxml2 { class XMLDocument; class XMLElement; }

namespace AGS
{
namespace AGF
{

using AGS::Common::String;
typedef tinyxml2::XMLDocument Document;
typedef tinyxml2::XMLElement* DocElem;

// AGFReader opens AGF document and checks it for validity.
// Provides a Game data root node.
class AGFReader
{
public:
    AGFReader();
    ~AGFReader();

    AGS::Common::HError Open(const char *filename);
    void Close();

    inline DocElem GetGameRoot() const { return _gameRoot; }

private:
    std::unique_ptr<Document> _doc;
    DocElem _gameRoot;
};

// Helper class, providing static methods for reading values from a doc element
class ValueParser
{
protected:
    ValueParser() = default;
    ~ValueParser() = default;

    // Helper functions for reading certain field;
    // all of them return default value if the field cannot be read
    static const char *ReadString(DocElem elem, const char *field, const char *def_value = "");
    static int ReadInt(DocElem elem, const char *field, int def_value = 0);
    static bool ReadBool(DocElem elem, const char *field, bool def_value = false);
};

// EntityParser: parent class meant for parsing a game entity
class EntityParser : public ValueParser
{
public:
    EntityParser() = default;
    virtual ~EntityParser() = default;

    // Read name of entity's type
    virtual String ReadType(DocElem elem) = 0;
    // Read entity's numeric ID (aka index)
    virtual int ReadID(DocElem elem) = 0;
    // Read entity's script name (may be null)
    virtual String ReadScriptName(DocElem elem) = 0;
};

// EntityListParser: parent class meant for parsing a list of game entities
// and returning a collection of their document nodes.
// In AGF document lists may contain subfolders with (theoretically) unlimited
// nesting. List parser takes care of that. The returned nodes are all pointing
// directly to the object data, skipping folders.
class EntityListParser
{
public:
    EntityListParser() = default;
    virtual ~EntityListParser() = default;

    // Returns a collection of nodes referencing object data
    virtual void GetAll(DocElem root, std::vector<DocElem> &elems) = 0;

protected:
    // Parses a list or tree of nodes;
    // folder_elem - name of the folder element; pass null if entity list
    //     does not support subfolders;
    // list_elem - name of the list element, cannot be null;
    // type_elem - name of the object element; pass null if exact name should
    //     be ignored: this is useful if the list contains multiple types;
    void GetAllElems(DocElem root, std::vector<DocElem> &elems,
        const char *folder_elem, const char *list_elem, const char *type_elem);
    // Special case of above: when the root elem and list elem have different names;
    // root_elem - name of the topmost list element, cannot be null;
    void GetAllElems(DocElem root, std::vector<DocElem> &elems,
        const char *root_elem, const char *folder_elem, const char *list_elem, const char *type_elem);

private:
    // Internal implementation of recursive subfolder parsing
    void GetElemsRecursive(DocElem folder, std::vector<DocElem> &elems,
        const char *folder_elem, const char *list_elem, const char *type_elem);
};


// AudioClip data parser
class AudioClip : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "AudioClip"; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "ScriptName"); }
};

// AudioType data parser
class AudioType : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return ""; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "TypeID"); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }
};

// Character data parser
class Character : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "Character"; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "ScriptName");  }
};

// Cursor data parser
class Cursor : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return ""; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }
};

// Dialog data parser
class Dialog : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "Dialog"; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }

    int ReadOptionCount(DocElem elem);
    String ReadScript(DocElem elem) { return ReadString(elem, "Script"); }
};

// Font data parser
class Font : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return ""; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }
};

// GUI Control data parser
class GUIControl : public EntityParser
{
public:
    String ReadType(DocElem elem) override;
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }
    void ReadAllData(DocElem elem, DataUtil::GUIControlData& data);
    void ReadButtonData(DocElem elem, DataUtil::GUIButtonData& data);
    void ReadLabelData(DocElem elem, DataUtil::GUILabelData& data);
    void ReadSliderData(DocElem elem, DataUtil::GUISliderData& data);
    void ReadInventoryData(DocElem elem, DataUtil::GUIInventoryData& data);
    void ReadTextBoxData(DocElem elem, DataUtil::GUITextBoxData& data);
    void ReadListBoxData(DocElem elem, DataUtil::GUIListBoxData& data);
};

// GUI data parser
class GUIMain : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "GUI"; }
    int ReadID(DocElem elem) override;
    String ReadScriptName(DocElem elem) override;
    void ReadAllData(DocElem elem, DataUtil::GUIData& gui_data);

private:
    DocElem GetNormalGUI(DocElem elem);
    DocElem GetTextWindow(DocElem elem);
};

// Inventory Item data parser
class InventoryItem : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "InventoryItem"; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }
};

// View data parser
class View : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return ""; }
    int ReadID(DocElem elem) override { return ReadInt(elem, "ID", -1); }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }
};


// Audio Clip list parser
class AudioClips : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "AudioClipFolder", "AudioClips", "AudioClip");
    }
};

// Audio Type list parser
class AudioTypes : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, nullptr, "AudioClipTypes", "AudioClipType");
    }
};

// Character list parser
class Characters : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "CharacterFolder", "Characters", "Character");
    }
};

// Cursor list parser
class Cursors : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, nullptr, "Cursors", "MouseCursor");
    }
};

// Dialog list parser
class Dialogs : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "DialogFolder", "Dialogs", "Dialog");
    }
};

// Font list parser
class Fonts : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, nullptr, "Fonts", "Font");
    }
};

// GUI list parser
class GUIs : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "GUIFolder", "GUIs", "GUIMain");
    }
};

// GUI Control list parser
class GUIControls : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, nullptr, "Controls", nullptr);
    }
};

// Inventory Item list parser
class Inventory : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "InventoryItemFolder", "InventoryItems", "InventoryItem");
    }
};

// View list parser
class Views : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "ViewFolder", "Views", "View");
    }
};

class GlobalVariable : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return ReadString(elem, "Type"); }
    int ReadID(DocElem elem) override { return -1; }
    String ReadScriptName(DocElem elem) override { return ReadString(elem, "Name"); }

    String ReadDefaultValue(DocElem elem) { return ReadString(elem, "DefaultValue"); }
};

class GlobalVariables : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override;
};

class Game : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "Game"; }
    int    ReadID(DocElem elem) override { return -1; }
    String ReadScriptName(DocElem elem) override { return ""; }

    DocElem GetSettings(DocElem elem);
};

class GameSettings : public EntityParser
{
public:
    String ReadType(DocElem elem) override { return "GameSettings"; }
    int    ReadID(DocElem elem) override { return -1; }
    String ReadScriptName(DocElem elem) override { return ""; }

    void ReadGameSettings(DocElem elem, DataUtil::GameSettings& s)
    {
        s.AllowRelativeAssetResolutions = ReadBool(elem, "AllowRelativeAssetResolutions");
        s.AlwaysDisplayTextAsSpeech = ReadBool(elem, "AlwaysDisplayTextAsSpeech");
        s.AndroidAppVersionCode = ReadInt(elem, "AndroidAppVersionCode");
        s.AndroidAppVersionName = ReadString(elem, "AndroidAppVersionName");
        s.AndroidApplicationId = ReadString(elem, "AndroidApplicationId");
        s.AndroidBuildFormat = ReadString(elem, "AndroidBuildFormat");
        s.AntiAliasFonts = ReadBool(elem, "AntiAliasFonts");
        s.AntiGlideMode = ReadBool(elem, "AntiGlideMode");
        s.AttachDataToExe = ReadBool(elem, "AttachDataToExe");
        s.AudioIndexer = ReadInt(elem, "AudioIndexer");
        s.AutoMoveInWalkMode = ReadBool(elem, "AutoMoveInWalkMode");
        s.BackwardsText = ReadBool(elem, "BackwardsText");
        s.BuildTargets = ReadString(elem, "BuildTargets");
        s.ClipGUIControls = ReadBool(elem, "ClipGUIControls");
        s.TrueColor = ReadString(elem, "TrueColor");
        s.CompressSpritesType = ReadString(elem, "CompressSpritesType");
        s.CustomDataDir = ReadString(elem, "CustomDataDir");
        s.CustomResolution = ReadString(elem, "CustomResolution");
        s.DebugMode = ReadBool(elem, "DebugMode");
        s.DefaultRoomMaskResolution = ReadInt(elem, "DefaultRoomMaskResolution");
        s.Description = ReadString(elem, "Description");
        s.DeveloperName = ReadString(elem, "DeveloperName");
        s.DeveloperURL = ReadString(elem, "DeveloperURL");
        s.DialogOptionsBackwards = ReadBool(elem, "DialogOptionsBackwards");
        s.DialogOptionsBullet = ReadInt(elem, "DialogOptionsBullet");
        s.DialogOptionsGUI = ReadInt(elem, "DialogOptionsGUI");
        s.DialogOptionsGap = ReadInt(elem, "DialogOptionsGap");
        s.DialogScriptNarrateFunction = ReadString(elem, "DialogScriptNarrateFunction");
        s.DialogScriptSayFunction = ReadString(elem, "DialogScriptSayFunction");
        s.DisplayMultipleInventory = ReadBool(elem, "DisplayMultipleInventory");
        s.EnforceNewAudio = ReadBool(elem, "EnforceNewAudio");
        s.EnforceNewStrings = ReadBool(elem, "EnforceNewStrings");
        s.EnforceObjectBasedScript = ReadBool(elem, "EnforceObjectBasedScript");
        s.GUIAlphaStyle = ReadString(elem, "GUIAlphaStyle");
        s.GUIDAsString = ReadString(elem, "GUIDAsString");
        s.GameFileName = ReadString(elem, "GameFileName");
        s.GameName = ReadString(elem, "GameName");
        s.GameTextEncoding = ReadString(elem, "GameTextEncoding");
        s.Genre = ReadString(elem, "Genre");
        s.GlobalSpeechAnimationDelay = ReadInt(elem, "GlobalSpeechAnimationDelay");
        s.HandleInvClicksInScript = ReadBool(elem, "HandleInvClicksInScript");
        s.InventoryCursors = ReadBool(elem, "InventoryCursors");
        s.InventoryHotspotMarkerCrosshairColor = ReadInt(elem, "InventoryHotspotMarkerCrosshairColor");
        s.InventoryHotspotMarkerDotColor = ReadInt(elem, "InventoryHotspotMarkerDotColor");
        s.InventoryHotspotMarkerSprite = ReadInt(elem, "InventoryHotspotMarkerSprite");
        s.InventoryHotspotMarkerStyle = ReadString(elem, "InventoryHotspotMarkerStyle");
        s.LeftToRightPrecedence = ReadBool(elem, "LeftToRightPrecedence");
        s.LetterboxMode = ReadBool(elem, "LetterboxMode");
        s.MaximumScore = ReadInt(elem, "MaximumScore");
        s.MouseWheelEnabled = ReadBool(elem, "MouseWheelEnabled");
        s.NumberDialogOptions = ReadString(elem, "NumberDialogOptions");
        s.OptimizeSpriteStorage = ReadBool(elem, "OptimizeSpriteStorage");
        s.PixelPerfect = ReadBool(elem, "PixelPerfect");
        s.PlaySoundOnScore = ReadInt(elem, "PlaySoundOnScore");
        s.ReleaseDate = ReadString(elem, "ReleaseDate");
        s.RenderAtScreenResolution = ReadString(elem, "RenderAtScreenResolution");
        s.RoomTransition = ReadString(elem, "RoomTransition");
        s.RunGameLoopsWhileDialogOptionsDisplayed = ReadBool(elem, "RunGameLoopsWhileDialogOptionsDisplayed");
        s.SaveGameFileExtension = ReadString(elem, "SaveGameFileExtension");
        s.SaveGameFolderName = ReadString(elem, "SaveGameFolderName");
        s.SaveScreenshots = ReadBool(elem, "SaveScreenshots");
        s.ScaleCharacterSpriteOffsets = ReadBool(elem, "ScaleCharacterSpriteOffsets");
        s.ScaleMovementSpeedWithMaskResolution = ReadBool(elem, "ScaleMovementSpeedWithMaskResolution");
        s.ScriptAPIVersion = ReadString(elem, "ScriptAPIVersion");
        s.ScriptCompatLevel = ReadString(elem, "ScriptCompatLevel");
        s.SkipSpeech = ReadString(elem, "SkipSpeech");
        s.SpeechPortraitSide = ReadString(elem, "SpeechPortraitSide");
        s.SpeechStyle = ReadString(elem, "SpeechStyle");
        s.SplitResources = ReadString(elem, "SplitResources");
        s.SpriteAlphaStyle = ReadString(elem, "SpriteAlphaStyle");
        s.TTFHeightDefinedBy = ReadString(elem, "TTFHeightDefinedBy");
        s.TTFMetricsFixup = ReadString(elem, "TTFMetricsFixup");
        s.TextWindowGUI = ReadInt(elem, "TextWindowGUI");
        s.ThoughtGUI = ReadInt(elem, "ThoughtGUI");
        s.TurnBeforeFacing = ReadBool(elem, "TurnBeforeFacing");
        s.TurnBeforeWalking = ReadBool(elem, "TurnBeforeWalking");
        s.UniqueID = ReadInt(elem, "UniqueID");
        s.UseGlobalSpeechAnimationDelay = ReadBool(elem, "UseGlobalSpeechAnimationDelay");
        s.UseLowResCoordinatesInScript = ReadBool(elem, "UseLowResCoordinatesInScript");
        s.UseOldCustomDialogOptionsAPI = ReadBool(elem, "UseOldCustomDialogOptionsAPI");
        s.UseOldKeyboardHandling = ReadBool(elem, "UseOldKeyboardHandling");
        s.Version = ReadString(elem, "Version");
        s.WalkInLookMode = ReadBool(elem, "WalkInLookMode");
        s.WhenInterfaceDisabled = ReadString(elem, "WhenInterfaceDisabled");
    }
};

// Parses a description of an individual script file (header or body)
class ScriptElem : public ValueParser
{
public:
    bool   IsHeader(DocElem elem) { return ReadBool(elem, "IsHeader"); }
    String ReadFilename(DocElem elem) { return ReadString(elem, "FileName"); }
};

// Parses a description of a script module with header
class ScriptWithHeader : public ValueParser
{
public:
    DocElem GetHeader(DocElem elem);
    DocElem GetBody(DocElem elem);
};

// Parses a list of script modules
class ScriptModules : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "Scripts", "ScriptFolder", "ScriptAndHeaders", "ScriptAndHeader");
    }
};

// Parses a description of a room
class Room : public ValueParser
{
public:
    int ReadNumber(DocElem elem) { return ReadInt(elem, "Number", -1); }
    String ReadDescription(DocElem elem) { return ReadString(elem, "Description"); }
};

// Parses a list of rooms
class Rooms : public EntityListParser
{
public:
    void GetAll(DocElem root, std::vector<DocElem> &elems) override
    {
        GetAllElems(root, elems, "Rooms", "UnloadedRoomFolder", "UnloadedRooms", "UnloadedRoom");
    }
};


//
// Helper functions
//

// Read entity reference data using given parser from the given doc element
void ReadEntityRef(DataUtil::EntityRef &ent, EntityParser &parser, DocElem elem);
// Reads a group of entity ref data using given list and entity parsers
// from the given doc root element.
void ReadAllEntityRefs(std::vector<DataUtil::EntityRef> &ents, EntityListParser &list_parser,
    EntityParser &parser, DocElem root);
// Reads global variables defined inside the game project from the given doc root element.
void ReadGlobalVariables(std::vector<DataUtil::Variable> &vars, DocElem root);
// Reads game settings from the given doc root element.
void ReadGameSettings(DataUtil::GameSettings &opt, DocElem root);
// Reads full game reference data using AGFReader
void ReadGameRef(DataUtil::GameRef &game, AGFReader &reader);
// Reads an ordered list of script module names (their order determines dependency).
void ReadScriptList(std::vector<String> &script_list, DocElem root);
// Reads an ordered list of script header module names (their order determines dependency).
void ReadScriptHeaderList(std::vector<String> &script_list, DocElem root);
// Reads a list of room ID and descriptions found in the game document.
void ReadRoomList(std::vector<std::pair<int, String>> &room_list, DocElem root);

} // namespace AGF
} // namespace AGS

#endif // __AGS_TOOL_DATA__AGFREADER_H
