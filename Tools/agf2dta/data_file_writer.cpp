//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2026 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#include "data_file_writer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ac/characterinfo.h"
#include "ac/dynobj/scriptaudioclip.h"
#include "ac/game_version.h"
#include "ac/gamesetupstructbase.h"
#include "ac/inventoryiteminfo.h"
#include "ac/mousecursor.h"
#include "game/customproperties.h"
#include "gui/guidefines.h"
#include "util/string_utils.h"

using namespace AGS::Common;

namespace
{

static void WriteNullTerminatedString(Stream *out, const String &text)
{
    if (!out)
        return;
    if (!text.IsEmpty())
        out->Write(text.GetCStr(), text.GetLength());
    out->WriteByte(0);
}

static void WriteCountedText(Stream *out, const String &text)
{
    if (!out)
        return;
    out->WriteInt32(static_cast<int32_t>(text.GetLength()));
    if (!text.IsEmpty())
        out->Write(text.GetCStr(), text.GetLength());
}

static void WriteFixedText(Stream *out, const String &text, size_t length)
{
    if (!out || length == 0)
        return;

    String tmp = text.Left(static_cast<int>(length));
    if (!tmp.IsEmpty())
        out->Write(tmp.GetCStr(), tmp.GetLength());
    for (size_t i = tmp.GetLength(); i < length; ++i)
        out->WriteByte(0);
}

static bool ParseResolution(const String &text, int &width, int &height)
{
    width = 320;
    height = 200;
    if (text.IsEmpty())
        return false;

    int parsed_width = 0;
    int parsed_height = 0;
    if (std::sscanf(text.GetCStr(), "%d x %d", &parsed_width, &parsed_height) == 2 ||
        std::sscanf(text.GetCStr(), "%dx%d", &parsed_width, &parsed_height) == 2 ||
        std::sscanf(text.GetCStr(), "%d,%d", &parsed_width, &parsed_height) == 2)
    {
        if (parsed_width > 0 && parsed_height > 0)
        {
            width = parsed_width;
            height = parsed_height;
            return true;
        }
    }
    return false;
}

static void WritePropertySchemaBlock(Stream *out,
    const std::vector<DataUtil::CustomPropertySchemaItem> &schema)
{
    out->WriteInt32(AGS::Common::kPropertyVersion_Current);
    out->WriteInt32(static_cast<int32_t>(schema.size()));
    for (const auto &item : schema)
    {
        WriteCountedText(out, item.Name);
        out->WriteInt32(static_cast<int32_t>(item.Type));
        WriteCountedText(out, item.Description);
        WriteCountedText(out, item.DefaultValue);
    }
}

static void WriteEmptyPropertyValues(Stream *out)
{
    out->WriteInt32(AGS::Common::kPropertyVersion_Current);
    out->WriteInt32(0);
}

static void WriteDefaultFontInfo(Stream *out)
{
    out->WriteInt32(0);   // flags
    out->WriteInt32(1);   // size
    out->WriteInt32(-1);  // outline
    out->WriteInt32(0);   // yoffset
    out->WriteInt32(0);   // linespacing
}

static void WriteEmptyInteractionEvents(Stream *out)
{
    out->WriteInt32(0);
}

static void WriteDefaultView(Stream *out)
{
    out->WriteInt16(0);
}

static void WriteDefaultCharacter(Stream *out, const DataUtil::EntityRef &ref, int index)
{
    CharacterInfo chinfo;
    CharacterInfo2 chinfo2;

    chinfo.index_id = ref.ID >= 0 ? ref.ID : index;
    chinfo.on = 1;
    chinfo.defview = -1;
    chinfo.talkview = -1;
    chinfo.view = -1;
    chinfo.room = -1;
    chinfo.prevroom = -1;
    chinfo.idleview = -1;
    chinfo.thinkview = -1;
    chinfo.blinkview = -1;
    chinfo.activeinv = -1;

    const String &name = ref.ScriptName.IsEmpty() ? ref.TypeName : ref.ScriptName;
    std::snprintf(chinfo.name, LEGACY_MAX_CHAR_NAME_LEN, "%s", name.GetCStr());
    std::snprintf(chinfo.scrname, LEGACY_MAX_SCRIPT_NAME_LEN, "%s", name.GetCStr());
    chinfo2.name_new = name;
    chinfo2.scrname_new = name;

    chinfo.WriteToFile(chinfo2, out);
}

static void WriteInventoryItem(Stream *out, const DataUtil::InventoryItemData &ref)
{
    InventoryItemInfo item{};
    item.name = ref.Description;
    item.pic = ref.Image;
    item.cursorPic = ref.CursorImage;
    item.hotx = ref.HotspotX;
    item.hoty = ref.HotspotY;
    item.flags = ref.PlayerStartsWith ? IFLG_STARTWITH : 0;
    item.WriteToFile(out);
}

static void WriteCursor(Stream *out, const DataUtil::CursorData &ref)
{
    MouseCursor cur;
    const String &name = ref.ScriptName.IsEmpty() ? ref.TypeName : ref.ScriptName;
    std::snprintf(cur.legacy_name, LEGACY_MAX_CURSOR_NAME_LENGTH, "%s", name.GetCStr());
    cur.name = name;
    cur.pic = ref.Image;
    cur.hotx = static_cast<short>(ref.HotspotX);
    cur.hoty = static_cast<short>(ref.HotspotY);
    cur.view = static_cast<short>(ref.View > 0 ? ref.View - 1 : -1);
    cur.flags = ref.Animate ? MCF_ANIMMOVE : 0;
    cur.WriteToFile(out);
}

static void WriteGui(Stream *out, const DataUtil::GUIData &ref, int index)
{
    const String &name = ref.ScriptName.IsEmpty() ? ref.TypeName : ref.ScriptName;
    WriteCountedText(out, name);
    WriteCountedText(out, ref.OnClick);
    out->WriteInt32(ref.Left);
    out->WriteInt32(ref.Top);
    out->WriteInt32(ref.Width);
    out->WriteInt32(ref.Height);
    // Control records are written in the type-specific lists below; these are
    // still empty until that serialization is implemented.
    out->WriteInt32(0);
    out->WriteInt32(ref.PopupStyle.CompareNoCase("MouseYPos") == 0 ? kGUIPopupMouseY : kGUIPopupNormal);
    out->WriteInt32(ref.PopupYPos);
    out->WriteInt32(ref.BackgroundColor);
    out->WriteInt32(ref.BackgroundImage);
    out->WriteInt32(ref.BorderColor);
    int flags = kGUIMain_DefFlags;
    if (!ref.Clickable) flags &= ~kGUIMain_Clickable;
    if (!ref.Visible) flags &= ~kGUIMain_Visible;
    out->WriteInt32(flags);
    out->WriteInt32(ref.Transparency);
    out->WriteInt32(ref.ZOrder);
    out->WriteInt32(ref.ID >= 0 ? ref.ID : index);
    out->WriteInt32(TEXTWINDOW_PADDING_DEFAULT);
}

static void WriteEmptyGuiControlList(Stream *out)
{
    out->WriteInt32(0);
}

static void WriteDefaultAudioType(Stream *out, bool speech)
{
    out->WriteInt32(speech ? 0 : 1);
    out->WriteInt32(speech ? 1 : 0);
    out->WriteInt32(0);
    out->WriteInt32(0);
    out->WriteInt32(0);
}

static void WriteDefaultAudioClip(Stream *out, const DataUtil::EntityRef &ref, int index)
{
    const String &name = ref.ScriptName.IsEmpty() ? ref.TypeName : ref.ScriptName;
    out->WriteInt32(ref.ID >= 0 ? ref.ID : index);
    String script_name = name;
    if (script_name.GetLength() > 29)
        script_name = script_name.Left(29);
    String file_name;
    WriteFixedText(out, script_name, 30);
    WriteFixedText(out, file_name, 15);
    out->WriteInt8(0); // bundlingType
    out->WriteInt8(0); // type
    out->WriteInt8(0); // fileType
    out->WriteInt8(0); // defaultRepeat
    out->WriteInt16(0); // alignment padding to int16
    out->WriteInt16(0); // defaultPriority
    out->WriteInt16(0); // defaultVolume
    out->WriteInt16(0); // alignment padding to int32
    out->WriteInt32(0); // reserved
}

static void WriteRoomName(Stream *out, int number, const String &description)
{
    out->WriteInt32(number);
    WriteNullTerminatedString(out, description);
}

static int ParseGameTextEncodingCodePage(const String &value)
{
    if (value.CompareNoCase("UTF-8") == 0 || value.CompareNoCase("utf-8") == 0)
        return 65001;
    if (value.CompareNoCase("ANSI") == 0 || value.CompareNoCase("ansi") == 0)
        return 1252;
    return 65001;
}

static int ParseScriptApiVersion(const String &value)
{
    if (value.CompareNoCase("Highest") == 0)
        return kScriptAPI_Current;

    static const CstrArr<14> kScriptApiNames = {{
        "v321", "v330", "v334", "v335", "v340",
        "v341", "v350", "v3507", "v351", "v360",
        "v36026", "v361", "v362", "v363"
    }};
    switch (StrUtil::ParseEnum(value, kScriptApiNames, 13))
    {
    case 0: return kScriptAPI_v321;
    case 1: return kScriptAPI_v330;
    case 2: return kScriptAPI_v334;
    case 3: return kScriptAPI_v335;
    case 4: return kScriptAPI_v340;
    case 5: return kScriptAPI_v341;
    case 6: return kScriptAPI_v350;
    case 7: return kScriptAPI_v3507;
    case 8: return kScriptAPI_v351;
    case 9: return kScriptAPI_v360;
    case 10: return kScriptAPI_v36026;
    case 11: return kScriptAPI_v361;
    case 12: return kScriptAPI_v362;
    case 13:
    default: return kScriptAPI_v363;
    }
}

static int ParseSplitResources(const String &value)
{
    return StrUtil::StringToInt(value, 0);
}

static void WriteGameSetupStructBase(const DataUtil::GameData &game, Stream *out)
{
    String game_name = game.Settings.GameName;
    if (game_name.IsEmpty())
        game_name = game.Settings.GameFileName;
    WriteFixedText(out, game_name, GameSetupStructBase::LEGACY_GAME_NAME_LENGTH);
    out->WriteInt16(0); // alignment padding to int32

    // TEMP: this is still a partial reconstruction of the editor's full
    // GameSetupStructBase serialization. Only the fields the current tool
    // model knows about are wired here; the rest remain their default values.
    int options[GameSetupStructBase::MAX_OPTIONS] = { 0 };
    options[OPT_ALWAYSSPCH] = game.Settings.AlwaysDisplayTextAsSpeech ? 1 : 0;
    options[OPT_ANTIALIASFONTS] = game.Settings.AntiAliasFonts ? 1 : 0;
    options[OPT_ANTIGLIDE] = game.Settings.AntiGlideMode ? 1 : 0;
    options[OPT_NOWALKMODE] = game.Settings.AutoMoveInWalkMode ? 0 : 1;
    options[OPT_RIGHTLEFTWRITE] = game.Settings.BackwardsText ? 1 : 0;
    options[OPT_DIALOGUPWARDS] = game.Settings.DialogOptionsBackwards ? 1 : 0;
    options[OPT_DIALOGGAP] = game.Settings.DialogOptionsGap;
    options[OPT_DIALOGIFACE] = game.Settings.DialogOptionsGUI;
    options[OPT_DUPLICATEINV] = game.Settings.DisplayMultipleInventory ? 1 : 0;
    options[OPT_HIRES_FONTS] = 0;
    options[OPT_NEWGUIALPHA] = static_cast<int>(game.Settings.GUIAlphaStyle);
    options[OPT_SPRITEALPHA] = static_cast<int>(game.Settings.SpriteAlphaStyle);
    options[OPT_DIALOGNUMBERED] = static_cast<int>(game.Settings.NumberDialogOptions);
    options[OPT_NOSKIPTEXT] = static_cast<int>(game.Settings.SkipSpeech);
    options[OPT_PORTRAITSIDE] = static_cast<int>(game.Settings.SpeechPortraitSide);
    options[OPT_SPEECHTYPE] = static_cast<int>(game.Settings.SpeechStyle);
    options[OPT_SPLITRESOURCES] = ParseSplitResources(game.Settings.SplitResources);
    options[OPT_BASESCRIPTAPI] = ParseScriptApiVersion(game.Settings.ScriptAPIVersion);
    options[OPT_SCRIPTCOMPATLEV] = ParseScriptApiVersion(game.Settings.ScriptCompatLevel);
    options[OPT_GAMETEXTENCODING] = ParseGameTextEncodingCodePage(game.Settings.GameTextEncoding);
    options[OPT_DEBUGMODE] = game.Settings.DebugMode ? 1 : 0;
    options[OPT_COMPRESSSPRITES] = static_cast<int>(game.Settings.CompressSpritesType);
    options[OPT_LETTERBOX] = game.Settings.LetterboxMode ? 1 : 0;
    options[OPT_NATIVECOORDINATES] = game.Settings.UseLowResCoordinatesInScript ? 0 : 1;
    options[OPT_SAFEFILEPATHS] = 1;
    options[OPT_RELATIVEASSETRES] = game.Settings.AllowRelativeAssetResolutions ? 1 : 0;
    options[OPT_STRICTSTRINGS] = game.Settings.EnforceNewStrings ? 1 : 0;
    options[OPT_STRICTSCRIPTING] = game.Settings.EnforceObjectBasedScript ? 1 : 0;
    options[OPT_HANDLEINVCLICKS] = game.Settings.HandleInvClicksInScript ? 1 : 0;
    options[OPT_FIXEDINVCURSOR] = game.Settings.InventoryCursors ? 0 : 1;
    options[OPT_GLOBALTALKANIMSPD] = game.Settings.UseGlobalSpeechAnimationDelay
        ? game.Settings.GlobalSpeechAnimationDelay
        : (-game.Settings.GlobalSpeechAnimationDelay - 1);
    options[OPT_LEFTTORIGHTEVAL] = game.Settings.LeftToRightPrecedence ? 1 : 0;
    options[OPT_MOUSEWHEEL] = game.Settings.MouseWheelEnabled ? 1 : 0;
    options[OPT_PIXPERFECT] = game.Settings.PixelPerfect ? 1 : 0;
    options[OPT_RUNGAMEDLGOPTS] = game.Settings.RunGameLoopsWhileDialogOptionsDisplayed ? 1 : 0;
    options[OPT_SAVESCREENSHOT] = game.Settings.SaveScreenshots ? 1 : 0;
    options[OPT_CHARTURNWHENFACE] = game.Settings.TurnBeforeFacing ? 1 : 0;
    options[OPT_CHARTURNWHENWALK] = game.Settings.TurnBeforeWalking ? 1 : 0;
    options[OPT_WALKONLOOK] = game.Settings.WalkInLookMode ? 1 : 0;
    options[OPT_CLIPGUICONTROLS] = game.Settings.ClipGUIControls ? 1 : 0;
    options[OPT_SCALECHAROFFSETS] = game.Settings.ScaleCharacterSpriteOffsets ? 1 : 0;
    options[OPT_WALKSPEEDABSOLUTE] = game.Settings.ScaleMovementSpeedWithMaskResolution ? 0 : 1;
    options[OPT_SAVESCREENSHOTLAYER] = -1;
    options[OPT_DIALOGOPTIONSAPI] = game.Settings.UseOldCustomDialogOptionsAPI ? -1 : 1;
    options[OPT_KEYHANDLEAPI] = game.Settings.UseOldKeyboardHandling ? 0 : 1;
    options[OPT_TWCUSTOM] = game.Settings.TextWindowGUI;
    options[OPT_THOUGHTGUI] = game.Settings.ThoughtGUI;
    options[OPT_DISABLEOFF] = static_cast<int>(game.Settings.WhenInterfaceDisabled);
    options[OPT_RENDERATSCREENRES] = static_cast<int>(game.Settings.RenderAtScreenResolution);

    for (int i = 0; i < GameSetupStructBase::MAX_OPTIONS; ++i)
        out->WriteInt32(options[i]);

    for (int i = 0; i < 256; ++i)
        out->WriteByte(0);
    for (int i = 0; i < 256; ++i)
    {
        out->WriteByte(0);
        out->WriteByte(0);
        out->WriteByte(0);
        out->WriteByte(255);
    }

    const int num_views = static_cast<int>(game.Views.size());
    const int num_characters = static_cast<int>(game.Characters.size());
    const int num_inventory = static_cast<int>(game.Inventory.size());
    const int num_dialogs = 0; // old dialog block is not used by this tool
    const int num_fonts = static_cast<int>(game.Fonts.size());
    const int num_gui = static_cast<int>(game.GUI.size());
    const int num_cursors = static_cast<int>(game.Cursors.size());

    out->WriteInt32(num_views);
    out->WriteInt32(num_characters);
    out->WriteInt32(game.PlayerCharacter);
    out->WriteInt32(game.Settings.MaximumScore);
    out->WriteInt16(static_cast<int16_t>(num_inventory + 1));
    out->WriteInt16(0);
    out->WriteInt32(num_dialogs);
    out->WriteInt32(0); // numdlgmessage, deprecated
    out->WriteInt32(num_fonts);
    out->WriteInt32(static_cast<int32_t>(game.Settings.ColorDepth)); // color_depth in bytes per pixel
    out->WriteInt32(0); // target_win
    out->WriteInt32(game.Settings.DialogOptionsBullet);
    out->WriteInt16(static_cast<int16_t>(game.Settings.InventoryHotspotMarkerDotColor));
    out->WriteInt16(static_cast<int16_t>(game.Settings.InventoryHotspotMarkerCrosshairColor));
    out->WriteInt32(game.Settings.UniqueID);
    out->WriteInt32(num_gui);
    out->WriteInt32(num_cursors);

    int game_width = 320;
    int game_height = 200;
    const bool has_resolution = ParseResolution(game.Settings.CustomResolution, game_width, game_height);
    if (has_resolution && game_width > 0 && game_height > 0)
    {
        out->WriteInt32(kGameResolution_Custom);
        out->WriteInt32(game_width);
        out->WriteInt32(game_height);
    }
    else
    {
        out->WriteInt32(kGameResolution_Custom);
        out->WriteInt32(320);
        out->WriteInt32(200);
    }

    out->WriteInt32(0); // default lipsync frame
    out->WriteInt32(game.Settings.InventoryHotspotMarkerSprite);
    for (int i = 0; i < GameSetupStructBase::NUM_INTS_RESERVED; ++i)
        out->WriteInt32(0);

    out->WriteInt32(0); // extension offset - none
    for (int i = 0; i < MAXGLOBALMES; ++i)
        out->WriteInt32(0);
    out->WriteInt32(0); // HasWordsDict
    out->WriteInt32(0); // globalscript pointer placeholder
    out->WriteInt32(0); // chars pointer placeholder
    out->WriteInt32(0); // HasCCScript
}

static void WriteSaveGameInfo(const DataUtil::GameData &game, Stream *out)
{
    String guid = game.Settings.GUIDAsString;
    WriteFixedText(out, guid, MAX_GUID_LENGTH);
    WriteFixedText(out, game.Settings.SaveGameFileExtension, MAX_SG_EXT_LENGTH);

    String folder = game.Settings.SaveGameFolderName;
    if (folder.IsEmpty())
    {
        if (!game.Settings.GameName.IsEmpty())
            folder = game.Settings.GameName;
        else if (!guid.IsEmpty())
            folder = guid;
        else
            folder = String::FromFormat("AGS-Game-%d", game.Settings.UniqueID);
    }
    WriteFixedText(out, folder, LEGACY_MAX_SG_FOLDER_LEN);
}

static void WriteFontBlock(const DataUtil::GameData &game, Stream *out)
{
    for (size_t i = 0; i < game.Fonts.size(); ++i)
        WriteDefaultFontInfo(out);
}

static void WriteSpriteFlags(Stream *out)
{
    out->WriteInt32(0);
}

static void WriteInventoryBlock(const DataUtil::GameData &game, Stream *out)
{
    out->WriteByteCount(0, 68); // slot 0 is unused
    for (const auto &item_ref : game.Inventory)
        WriteInventoryItem(out, item_ref);
}

static void WriteCursorBlock(const DataUtil::GameData &game, Stream *out)
{
    for (const auto &cursor_ref : game.Cursors)
        WriteCursor(out, cursor_ref);
}

static void WriteInteractionScriptsBlock(const DataUtil::GameData &game, Stream *out)
{
    for (size_t i = 0; i < game.Characters.size(); ++i)
        WriteEmptyInteractionEvents(out);
    for (size_t i = 1; i < game.Inventory.size(); ++i)
        WriteEmptyInteractionEvents(out);
}

static void WriteViewsBlock(const DataUtil::GameData &game, Stream *out)
{
    for (size_t i = 0; i < game.Views.size(); ++i)
        WriteDefaultView(out);
}

static void WriteCharactersBlock(const DataUtil::GameData &game, Stream *out)
{
    for (size_t i = 0; i < game.Characters.size(); ++i)
        WriteDefaultCharacter(out, game.Characters[i], static_cast<int>(i));
}

static void WriteLipSyncBlock(Stream *out)
{
    for (int i = 0; i < MAXLIPSYNCFRAMES; ++i)
        WriteFixedText(out, "", 50);
}

static void WriteGuiBlock(const DataUtil::GameData &game, Stream *out)
{
    out->WriteInt32(GUIMAGIC);
    out->WriteInt32(kGuiVersion_Current);
    out->WriteInt32(static_cast<int32_t>(game.GUI.size()));
    for (size_t i = 0; i < game.GUI.size(); ++i)
        WriteGui(out, game.GUI[i], static_cast<int>(i));

    WriteEmptyGuiControlList(out); // buttons
    WriteEmptyGuiControlList(out); // labels
    WriteEmptyGuiControlList(out); // inv windows
    WriteEmptyGuiControlList(out); // sliders
    WriteEmptyGuiControlList(out); // text boxes
    WriteEmptyGuiControlList(out); // list boxes
}

static void WritePluginsBlock(Stream *out)
{
    out->WriteInt32(0);
}

static void WriteCustomPropertiesBlock(const DataUtil::GameData &game, Stream *out)
{
    WritePropertySchemaBlock(out, game.PropertySchema);
    for (size_t i = 0; i < game.Characters.size(); ++i)
        WriteEmptyPropertyValues(out);
    WriteEmptyPropertyValues(out); // inventory slot 0
    for (size_t i = 0; i < game.Inventory.size(); ++i)
        WriteEmptyPropertyValues(out);

    for (size_t i = 0; i < game.Views.size(); ++i)
    {
        const String &name = game.Views[i].ScriptName.IsEmpty() ? game.Views[i].TypeName : game.Views[i].ScriptName;
        WriteNullTerminatedString(out, name);
    }

    WriteNullTerminatedString(out, "");
    for (size_t i = 0; i < game.Inventory.size(); ++i)
    {
        const String &name = game.Inventory[i].ScriptName.IsEmpty() ? game.Inventory[i].TypeName : game.Inventory[i].ScriptName;
        WriteNullTerminatedString(out, name);
    }
}

static void WriteAudioBlock(const DataUtil::GameData &game, Stream *out)
{
    const int audio_type_count = static_cast<int>(game.AudioTypes.size()) + 1;
    out->WriteInt32(audio_type_count);
    WriteDefaultAudioType(out, true);
    for (const auto &audio_type : game.AudioTypes)
        WriteDefaultAudioType(out, false);

    out->WriteInt32(static_cast<int32_t>(game.AudioClips.size()));
    for (size_t i = 0; i < game.AudioClips.size(); ++i)
        WriteDefaultAudioClip(out, game.AudioClips[i], static_cast<int>(i));

    out->WriteInt32(-1); // no score sound
}

static void WriteRoomNamesBlock(const DataUtil::GameData &game, Stream *out)
{
    if (!game.Settings.DebugMode)
        return;

    out->WriteInt32(static_cast<int32_t>(game.Rooms.size()));
    for (const auto &room : game.Rooms)
        WriteRoomName(out, room.first, room.second);
}

} // namespace

namespace AGS {
namespace DataUtil {

bool DataFileWriter::WriteGame28(const GameData &game,
    Stream *out, String &error)
{
    if (out == nullptr)
    {
        error = "Invalid output stream";
        return false;
    }

    WriteHeader(game, out);
    WriteGameSetupStructBase(game, out);
    WriteSaveGameInfo(game, out);
    WriteFonts(game, out);
    WriteSpriteFlags(out);
    WriteInventory(game, out);
    WriteCursorBlock(game, out);
    WriteInteractionScriptsBlock(game, out);
    WriteViews(game, out);
    WriteCharactersBlock(game, out);
    WriteLipSyncBlock(out);
    WriteGUIs(game, out);
    WritePluginsBlock(out);
    WriteCustomPropertiesBlock(game, out);
    WriteAudioBlock(game, out);
    WriteRoomNamesBlock(game, out);

    return true;
}

void DataFileWriter::WriteString(Stream *out, const String &text)
{
    WriteCountedText(out, text);
}

void DataFileWriter::WriteFixedString(Stream *out,
    const String &text, size_t length)
{
    WriteFixedText(out, text, length);
}

void DataFileWriter::WriteHeader(const GameData &, Stream *out)
{
    WriteFixedText(out, "Adventure Creator Game File v2", 30);
    out->WriteInt32(kGameVersion_Current);

    // TEMP: hardcoded until the tool exposes build metadata.
    const String compiled_with = "3.6.3.12";
    out->WriteInt32(static_cast<int32_t>(compiled_with.GetLength()));
    WriteFixedText(out, compiled_with, compiled_with.GetLength());

    out->WriteInt32(0); // no extended capabilities
}

void DataFileWriter::WriteFonts(const GameData &game, Stream *out)
{
    if (!out)
        return;
    WriteFontBlock(game, out);
}

void DataFileWriter::WriteInventory(const GameData &game, Stream *out)
{
    if (!out)
        return;
    WriteInventoryBlock(game, out);
}

void DataFileWriter::WriteViews(const GameData &game, Stream *out)
{
    if (!out)
        return;
    WriteViewsBlock(game, out);
}

void DataFileWriter::WriteGUIs(const GameData &game, Stream *out)
{
    if (!out)
        return;
    WriteGuiBlock(game, out);
}

} // namespace DataUtil
} // namespace AGS
