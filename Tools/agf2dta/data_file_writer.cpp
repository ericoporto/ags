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
#include "gfx/gfx_def.h"
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

static int GetGuiPopupStyle(DataUtil::GUIPopupStyle style)
{
    switch (style)
    {
    case DataUtil::kGUIPopupStyle_MouseYPos: return kGUIPopupMouseY;
    case DataUtil::kGUIPopupStyle_PopupModal: return kGUIPopupModal;
    case DataUtil::kGUIPopupStyle_Persistent: return kGUIPopupNoAutoRemove;
    case DataUtil::kGUIPopupStyle_Normal:
    default: return kGUIPopupNormal;
    }
}

static void WriteGui(Stream *out, const DataUtil::GUIData &ref, int index,
    const std::vector<uint32_t> &control_refs)
{
    const String &name = ref.ScriptName.IsEmpty() ? ref.TypeName : ref.ScriptName;
    WriteCountedText(out, name);
    WriteCountedText(out, ref.IsTextWindow ? String() : ref.OnClick);
    out->WriteInt32(ref.IsTextWindow ? 0 : ref.Left);
    out->WriteInt32(ref.IsTextWindow ? 0 : ref.Top);
    out->WriteInt32(ref.IsTextWindow ? 200 : ref.Width);
    out->WriteInt32(ref.IsTextWindow ? 100 : ref.Height);
    out->WriteInt32(static_cast<int32_t>(control_refs.size()));
    out->WriteInt32(ref.IsTextWindow ? kGUIPopupModal : GetGuiPopupStyle(ref.PopupStyle));
    out->WriteInt32(ref.IsTextWindow ? -1 : ref.PopupYPos);
    out->WriteInt32(ref.BackgroundColor);
    out->WriteInt32(ref.BackgroundImage);
    out->WriteInt32(ref.IsTextWindow ? ref.TextColor : ref.BorderColor);
    int flags = ref.IsTextWindow ? kGUIMain_TextWindow : kGUIMain_DefFlags;
    if (!ref.IsTextWindow && !ref.Clickable) flags &= ~kGUIMain_Clickable;
    if (!ref.IsTextWindow && !ref.Visible) flags &= ~kGUIMain_Visible;
    out->WriteInt32(flags);
    out->WriteInt32(ref.IsTextWindow ? 0 : GfxDef::Trans100ToLegacyTrans255(ref.Transparency));
    out->WriteInt32(ref.IsTextWindow ? -1 : ref.ZOrder);
    out->WriteInt32(ref.ID >= 0 ? ref.ID : index);
    out->WriteInt32(ref.IsTextWindow ? ref.Padding : TEXTWINDOW_PADDING_DEFAULT);
    for (uint32_t control_ref : control_refs)
        out->WriteInt32(control_ref);
}

static int GetGuiControlFlags(const DataUtil::GUIControlData &control)
{
    int flags = 0;
    if (control.Clickable) flags |= kGUICtrl_Clickable;
    if (control.Enabled) flags |= kGUICtrl_Enabled;
    if (control.Visible) flags |= kGUICtrl_Visible;
    if (control.Translated) flags |= kGUICtrl_Translated;
    return flags;
}

static void WriteGuiControl(Stream *out, const DataUtil::GUIControlData &control,
    int extra_flags, const String *event_handler)
{
    out->WriteInt32(GetGuiControlFlags(control) | extra_flags);
    out->WriteInt32(control.Left);
    out->WriteInt32(control.Top);
    out->WriteInt32(control.Width);
    out->WriteInt32(control.Height);
    out->WriteInt32(control.ZOrder);
    WriteNullTerminatedString(out, control.ScriptName);
    out->WriteInt32(event_handler ? 1 : 0);
    if (event_handler)
        WriteNullTerminatedString(out, *event_handler);
}

static int GetButtonClickAction(const String &action)
{
    if (action.CompareNoCase("SetCursorMode") == 0 || action.CompareNoCase("SetMode") == 0)
        return 1;
    if (action.CompareNoCase("RunScript") == 0)
        return 2;
    return 0;
}

static void WriteGuiButton(Stream *out, const DataUtil::GUIButtonData &button)
{
    WriteGuiControl(out, button, button.ClipImage ? kGUICtrl_Clip : 0, &button.OnClick);
    out->WriteInt32(button.Image);
    out->WriteInt32(button.MouseoverImage);
    out->WriteInt32(button.PushedImage);
    out->WriteInt32(button.Font);
    out->WriteInt32(button.TextColor);
    out->WriteInt32(GetButtonClickAction(button.ClickAction));
    out->WriteInt32(0);
    out->WriteInt32(button.NewModeNumber);
    out->WriteInt32(0);
    WriteCountedText(out, button.Text);
    out->WriteInt32(button.TextAlignment);
}

static void WriteGuiLabel(Stream *out, const DataUtil::GUILabelData &label)
{
    WriteGuiControl(out, label, 0, nullptr);
    WriteCountedText(out, label.Text);
    out->WriteInt32(label.Font);
    out->WriteInt32(label.TextColor);
    out->WriteInt32(label.TextAlignment);
}

static void WriteGuiInvWindow(Stream *out, const DataUtil::GUIInventoryData &inv)
{
    WriteGuiControl(out, inv, 0, nullptr);
    out->WriteInt32(inv.CharacterID);
    out->WriteInt32(inv.ItemWidth);
    out->WriteInt32(inv.ItemHeight);
}

static void WriteGuiSlider(Stream *out, const DataUtil::GUISliderData &slider)
{
    WriteGuiControl(out, slider, 0, &slider.OnChange);
    out->WriteInt32(slider.MinValue);
    out->WriteInt32(slider.MaxValue);
    out->WriteInt32(slider.Value);
    out->WriteInt32(slider.HandleImage);
    out->WriteInt32(slider.HandleOffset);
    out->WriteInt32(slider.BackgroundImage);
}

static void WriteGuiTextBox(Stream *out, const DataUtil::GUITextBoxData &text_box)
{
    WriteGuiControl(out, text_box, text_box.ShowBorder ? kGUICtrl_ShowBorder : 0, &text_box.OnActivate);
    WriteCountedText(out, text_box.Text);
    out->WriteInt32(text_box.Font);
    out->WriteInt32(text_box.TextColor);
    out->WriteInt32(text_box.ShowBorder ? kTextBox_ShowBorder : 0);
}

static void WriteGuiListBox(Stream *out, const DataUtil::GUIListBoxData &list_box)
{
    WriteGuiControl(out, list_box, list_box.ShowBorder ? kGUICtrl_ShowBorder : 0,
        &list_box.OnSelectionChanged);
    out->WriteInt32(0); // runtime items are not part of the project data
    out->WriteInt32(list_box.Font);
    out->WriteInt32(list_box.TextColor);
    out->WriteInt32(list_box.SelectedTextColor);
    int flags = 0;
    if (list_box.ShowBorder) flags |= kListBox_ShowBorder;
    if (list_box.ShowScrollArrows) flags |= kListBox_ShowArrows;
    out->WriteInt32(flags);
    out->WriteInt32(list_box.TextAlignment);
    out->WriteInt32(list_box.SelectedBackgroundColor);
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
    // TODO: NOT IMPLEMENTED YET
    for (int i = 0; i < MAXLIPSYNCFRAMES; ++i)
        WriteFixedText(out, "", 50);
}

static void WriteGuiBlock(const DataUtil::GameData &game, Stream *out)
{
    std::vector<std::shared_ptr<DataUtil::GUIButtonData>> buttons;
    std::vector<std::shared_ptr<DataUtil::GUILabelData>> labels;
    std::vector<std::shared_ptr<DataUtil::GUIInventoryData>> inv_windows;
    std::vector<std::shared_ptr<DataUtil::GUISliderData>> sliders;
    std::vector<std::shared_ptr<DataUtil::GUITextBoxData>> text_boxes;
    std::vector<std::shared_ptr<DataUtil::GUIListBoxData>> list_boxes;
    std::vector<std::vector<uint32_t>> gui_control_refs(game.GUI.size());

    for (size_t gui_index = 0; gui_index < game.GUI.size(); ++gui_index)
    {
        for (const auto &control : game.GUI[gui_index].Controls)
        {
            uint32_t packed_ref = 0;
            if (auto button = std::dynamic_pointer_cast<DataUtil::GUIButtonData>(control))
            {
                packed_ref = (kGUIButton << 16) | static_cast<uint32_t>(buttons.size());
                buttons.push_back(button);
            }
            else if (auto label = std::dynamic_pointer_cast<DataUtil::GUILabelData>(control))
            {
                packed_ref = (kGUILabel << 16) | static_cast<uint32_t>(labels.size());
                labels.push_back(label);
            }
            else if (auto inv = std::dynamic_pointer_cast<DataUtil::GUIInventoryData>(control))
            {
                packed_ref = (kGUIInvWindow << 16) | static_cast<uint32_t>(inv_windows.size());
                inv_windows.push_back(inv);
            }
            else if (auto slider = std::dynamic_pointer_cast<DataUtil::GUISliderData>(control))
            {
                packed_ref = (kGUISlider << 16) | static_cast<uint32_t>(sliders.size());
                sliders.push_back(slider);
            }
            else if (auto text_box = std::dynamic_pointer_cast<DataUtil::GUITextBoxData>(control))
            {
                packed_ref = (kGUITextBox << 16) | static_cast<uint32_t>(text_boxes.size());
                text_boxes.push_back(text_box);
            }
            else if (auto list_box = std::dynamic_pointer_cast<DataUtil::GUIListBoxData>(control))
            {
                packed_ref = (kGUIListBox << 16) | static_cast<uint32_t>(list_boxes.size());
                list_boxes.push_back(list_box);
            }
            else
            {
                continue;
            }
            gui_control_refs[gui_index].push_back(packed_ref);
        }
    }

    out->WriteInt32(GUIMAGIC);
    out->WriteInt32(kGuiVersion_Current);
    out->WriteInt32(static_cast<int32_t>(game.GUI.size()));
    for (size_t i = 0; i < game.GUI.size(); ++i)
        WriteGui(out, game.GUI[i], static_cast<int>(i), gui_control_refs[i]);

    out->WriteInt32(static_cast<int32_t>(buttons.size()));
    for (const auto &button : buttons)
        WriteGuiButton(out, *button);
    out->WriteInt32(static_cast<int32_t>(labels.size()));
    for (const auto &label : labels)
        WriteGuiLabel(out, *label);
    out->WriteInt32(static_cast<int32_t>(inv_windows.size()));
    for (const auto &inv : inv_windows)
        WriteGuiInvWindow(out, *inv);
    out->WriteInt32(static_cast<int32_t>(sliders.size()));
    for (const auto &slider : sliders)
        WriteGuiSlider(out, *slider);
    out->WriteInt32(static_cast<int32_t>(text_boxes.size()));
    for (const auto &text_box : text_boxes)
        WriteGuiTextBox(out, *text_box);
    out->WriteInt32(static_cast<int32_t>(list_boxes.size()));
    for (const auto &list_box : list_boxes)
        WriteGuiListBox(out, *list_box);
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

static void WriteHeaderBlock(Stream *out)
{
    WriteFixedText(out, "Adventure Creator Game File v2", 30);
    out->WriteInt32(kGameVersion_Current);

    // TEMP: hardcoded until the tool exposes build metadata.
    // TODO: FIGURE OUT HOW??? Should I pick from Common/ac/def_version.h ?
    const String compiled_with = "3.6.3.12";
    out->WriteInt32(static_cast<int32_t>(compiled_with.GetLength()));
    WriteFixedText(out, compiled_with, compiled_with.GetLength());

    out->WriteInt32(0); // no extended capabilities
}

} // namespace

namespace AGS {
namespace DataUtil {

HError WriteGameData28(const GameData &game, std::unique_ptr<Stream> &&out)
{
    if (!out)
        return new Error("WriteGameData28: Invalid output stream.");

    Stream *stream = out.get();

    WriteHeaderBlock(stream);
    WriteGameSetupStructBase(game, stream);
    WriteSaveGameInfo(game, stream);
    WriteFontBlock(game, stream);
    WriteSpriteFlags(stream);
    WriteInventoryBlock(game, stream);
    WriteCursorBlock(game, stream);
    WriteInteractionScriptsBlock(game, stream);
    WriteViewsBlock(game, stream);
    WriteCharactersBlock(game, stream);
    WriteLipSyncBlock(stream);
    WriteGuiBlock(game, stream);
    WritePluginsBlock(stream);
    WriteCustomPropertiesBlock(game, stream);
    WriteAudioBlock(game, stream);
    WriteRoomNamesBlock(game, stream);

    if (!out->Flush())
        return new Error("WriteGameData28: Failed to flush game data output stream.");
    return HError::None();
}

} // namespace DataUtil
} // namespace AGS
