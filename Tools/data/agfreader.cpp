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
#include "data/agfreader.h"
#include <tinyxml2.h>
#include "debug/out.h"
#include "util/string_compat.h"
#include "util/string_utils.h"

const char *XML_ROOT_NODE_NAME = "AGSEditorDocument";
const char *XML_ATTRIBUTE_VERSION = "Version";
const char *XML_ATTRIBUTE_VERSION_INDEX = "VersionIndex";
const char *XML_ATTRIBUTE_EDITOR_VERSION = "EditorVersion";
const char *LATEST_XML_VERSION = "3.0.3.2";
const int  LOWEST_SUPPORTED_FORMAT = 26; // AGS 3.5.0

using namespace AGS::Common;
using namespace tinyxml2;

namespace
{

static const CstrArr<3> kGameColorDepthNames = {{
    "Palette", "HighColor", "TrueColor"
}};

static const CstrArr<4> kCompressSpritesTypeNames = {{
    "None", "RLE", "LZW", "Deflate"
}};

static const CstrArr<3> kAndroidBuildFormatNames = {{
    "ApkEmbedded", "AabEmbedded", "Aab"
}};

static const CstrArr<3> kGuiAlphaStyleNames = {{
    "Classic", "AdditiveOpacity", "MultiplyTranslucenceSrcBlend"
}};

static const CstrArr<2> kSpriteAlphaStyleNames = {{
    "Classic", "Improved"
}};

static const CstrArr<3> kRenderAtScreenResNames = {{
    "UserDefined", "True", "False"
}};

static const CstrArr<5> kRoomTransitionNames = {{
    "FadeOutAndIn", "Instant", "Dissolve", "BlackBoxOut", "CrossFade"
}};

static const CstrArr<3> kNumberDialogOptionsNames = {{
    "None", "KeyShortcutsOnly", "Normal"
}};

static const CstrArr<7> kSkipSpeechNames = {{
    "MouseOrKeyboardOrTimer", "KeyboardOnly", "TimerOnly",
    "MouseOrKeyboard", "MouseOrTimer", "KeyboardOnlyStrict", "MouseOnlyStrict"
}};

static const CstrArr<4> kSpeechPortraitSideNames = {{
    "Left", "Right", "Alternate", "BasedOnCharacterPosition"
}};

static const CstrArr<4> kSpeechStyleNames = {{
    "Lucasarts", "SierraTransparent", "SierraWithBackground", "WholeScreen"
}};

static const CstrArr<3> kFontHeightDefinitionNames = {{
    "NominalHeight", "PixelHeight", "CustomValue"
}};

static const CstrArr<2> kFontMetricsFixupNames = {{
    "None", "SetAscenderToHeight"
}};

static const CstrArr<4> kInterfaceDisabledNames = {{
    "GreyOut", "GoBlack", "Unchanged", "TurnOff"
}};

static const CstrArr<3> kInventoryHotspotMarkerNames = {{
    "None", "Crosshair", "Sprite"
}};

static const CstrArr<3> kCustomPropertyTypeNames = {{
    "Boolean", "Number", "Text"
}};

static const CstrArr<9> kFrameAlignmentNames = {{
    "TopLeft", "TopCenter", "TopRight",
    "MiddleLeft", "MiddleCenter", "MiddleRight",
    "BottomLeft", "BottomCenter", "BottomRight"
}};

} // namespace

namespace AGS
{
namespace AGF
{

AGFReader::AGFReader()
    : _gameRoot(nullptr)
{
}

AGFReader::~AGFReader()
{
}

HError AGFReader::Open(const char *filename)
{
    Close();

    _doc.reset(new Document());
    if (_doc->LoadFile(filename) != XML_SUCCESS)
        return new Error("Failed to open XML", _doc->ErrorIDToName(_doc->ErrorID()));
    if (!_doc->RootElement() || strcmp(_doc->RootElement()->Name(), XML_ROOT_NODE_NAME))
        return new Error("Not a valid AGS game project");

    const char *attr_filever = _doc->RootElement()->Attribute(XML_ATTRIBUTE_VERSION);
    if (!attr_filever)
        return new Error("Game.agf format version is missing");
    if (strcmp(attr_filever, LATEST_XML_VERSION))
        return new Error(String::FromFormat("Unsupported Game.agf format: %s", attr_filever));

    const int attr_format = _doc->RootElement()->IntAttribute(XML_ATTRIBUTE_VERSION_INDEX);
    const char *attr_editorver = _doc->RootElement()->Attribute(XML_ATTRIBUTE_EDITOR_VERSION);
    Debug::Printf("AGFReader: opened %s,\n format tag: %s\n format index: %d\n saved by AGS %s",
        filename, attr_filever, attr_format, attr_editorver ? attr_editorver : "unknown");

    if (attr_format < LOWEST_SUPPORTED_FORMAT)
        return new Error(String::FromFormat("Unsupported Game.agf format index: %d", attr_format));

    DocElem game = _doc->RootElement()->FirstChildElement("Game");
    if (!game)
        return new Error("<Game> element not found");
    _gameRoot = game;
    return HError::None();
}

void AGFReader::Close()
{
    _doc.reset();
    _gameRoot = nullptr;
}

//-----------------------------------------------------------------------------
// Entity list parsers
//-----------------------------------------------------------------------------

void EntityListParser::GetAllElems(DocElem game_root, std::vector<DocElem> &elems,
    const char *folder_elem, const char *list_elem, const char *type_elem)
{
    DocElem list_root = game_root->FirstChildElement(list_elem);
    if (!list_root)
        return;
    DocElem node = list_root;
    if (folder_elem)
    { // Get the main folder
        node = node->FirstChildElement(folder_elem);
        if (!node)
            return;
    }
    return GetElemsRecursive(node, elems, folder_elem, list_elem, type_elem);
}

void EntityListParser::GetAllElems(DocElem game_root, std::vector<DocElem> &elems,
    const char *root_elem, const char *folder_elem, const char *list_elem, const char *type_elem)
{
    DocElem list_root = game_root->FirstChildElement(root_elem);
    if (!list_root)
        return;
    DocElem node = list_root;
    if (folder_elem)
    { // Get the main folder
        node = node->FirstChildElement(folder_elem);
        if (!node)
            return;
    }
    return GetElemsRecursive(node, elems, folder_elem, list_elem, type_elem);
}

void EntityListParser::GetElemsRecursive(DocElem folder,  std::vector<DocElem> &elems,
     const char *folder_elem, const char *list_elem, const char *type_elem)
{
    if (!folder)
        return;
    // First pass subfolders
    DocElem list_node = folder;
    if (folder_elem)
    {
        DocElem sub = folder->FirstChildElement("SubFolders");
        if (sub)
        {
            for (DocElem node = sub->FirstChildElement(folder_elem);
                node; node = node->NextSiblingElement(folder_elem))
            {
                GetElemsRecursive(node, elems, folder_elem, list_elem, type_elem);
            }
        }
        // get to elements inside a folder
        list_node = folder->FirstChildElement(list_elem);
        if (!list_node)
            return;
    }
    // Then pass elements themselves
    for (DocElem node = list_node->FirstChildElement(type_elem);
        node; node = node->NextSiblingElement(type_elem))
    {
        elems.push_back(node);
    }
}

//-----------------------------------------------------------------------------
// Entity parsers
//-----------------------------------------------------------------------------

const char* ValueParser::ReadString(DocElem elem, const char *field, const char *def_value)
{
    if (!elem)
        return def_value;
    DocElem name_f = elem->FirstChildElement(field);
    if (name_f && name_f->GetText())
        return name_f->GetText();
    return def_value;
}

int ValueParser::ReadInt(DocElem elem, const char *field, int def_value)
{
    if (!elem)
        return def_value;
    DocElem name_f = elem->FirstChildElement(field);
    if (name_f && name_f->GetText())
        return StrUtil::StringToInt(name_f->GetText(), def_value);
    return def_value;
}

bool ValueParser::ReadBool(DocElem elem, const char *field, bool def_value)
{
    if (!elem)
        return def_value;
    DocElem name_f = elem->FirstChildElement(field);
    if (name_f && name_f->GetText())
        return ags_stricmp(name_f->GetText(), "True") == 0;
    return def_value;
}

static DataUtil::GameColorDepth ReadGameColorDepth(const String &value)
{
    switch (StrUtil::ParseEnum(value, kGameColorDepthNames, 2))
    {
    case 0: return DataUtil::kGameColorDepth_Palette;
    case 1: return DataUtil::kGameColorDepth_HighColor;
    case 2:
    default: return DataUtil::kGameColorDepth_TrueColor;
    }
}

static SpriteCompression ReadSpriteCompression(const String &value)
{
    return StrUtil::ParseEnum(value, kCompressSpritesTypeNames, kSprCompress_None);
}

static DataUtil::AndroidBuildFormat ReadAndroidBuildFormat(const String &value)
{
    return StrUtil::ParseEnum(value, kAndroidBuildFormatNames, DataUtil::kAndroidBuild_ApkEmbedded);
}

static GameGuiAlphaRenderingStyle ReadGuiAlphaStyle(const String &value)
{
    return StrUtil::ParseEnum(value, kGuiAlphaStyleNames, kGuiAlphaRender_Legacy);
}

static GameSpriteAlphaRenderingStyle ReadSpriteAlphaStyle(const String &value)
{
    return StrUtil::ParseEnum(value, kSpriteAlphaStyleNames, kSpriteAlphaRender_Legacy);
}

static RenderAtScreenRes ReadRenderAtScreenResolution(const String &value)
{
    return StrUtil::ParseEnum(value, kRenderAtScreenResNames, kRenderAtScreenRes_UserDefined);
}

static ScreenTransitionStyle ReadRoomTransition(const String &value)
{
    return StrUtil::ParseEnum(value, kRoomTransitionNames, kScrTran_Fade);
}

static DialogOptionNumbering ReadDialogOptionsNumbering(const String &value)
{
    return StrUtil::ParseEnumWithBase(value, kNumberDialogOptionsNames,
        static_cast<DialogOptionNumbering>(-1), kDlgOptNoNumbering);
}

static DataUtil::SkipSpeechStyle ReadSkipSpeech(const String &value)
{
    return StrUtil::ParseEnum(value, kSkipSpeechNames, DataUtil::kSkipSpeech_MouseOrKeyboardOrTimer);
}

static DataUtil::SpeechPortraitSide ReadSpeechPortraitSide(const String &value)
{
    return StrUtil::ParseEnum(value, kSpeechPortraitSideNames, DataUtil::kSpeechPortrait_Left);
}

static DataUtil::SpeechStyle ReadSpeechStyle(const String &value)
{
    switch (StrUtil::ParseEnum(value, kSpeechStyleNames, 0))
    {
    case 0: return DataUtil::kSpeechStyle_Lucasarts;
    case 1: return DataUtil::kSpeechStyle_SierraTransparent;
    case 2: return DataUtil::kSpeechStyle_SierraWithBackground;
    case 3:
    default: return DataUtil::kSpeechStyle_WholeScreen;
    }
}

static DataUtil::FontHeightDefinition ReadFontHeightDefinition(const String &value)
{
    return StrUtil::ParseEnum(value, kFontHeightDefinitionNames, DataUtil::kFontHeight_NominalHeight);
}

static DataUtil::FontMetricsFixup ReadFontMetricsFixup(const String &value)
{
    return StrUtil::ParseEnum(value, kFontMetricsFixupNames, DataUtil::kFontMetrics_None);
}

static GuiDisableStyle ReadGuiDisableStyle(const String &value)
{
    return StrUtil::ParseEnum(value, kInterfaceDisabledNames, kGuiDis_Greyout);
}

static DataUtil::InventoryHotspotMarkerStyle ReadInventoryHotspotMarkerStyle(const String &value)
{
    return StrUtil::ParseEnum(value, kInventoryHotspotMarkerNames, DataUtil::kInventoryHotspot_None);
}

static AGS::Common::PropertyType ReadCustomPropertyType(const String &value)
{
    return StrUtil::ParseEnumWithBase(value, kCustomPropertyTypeNames,
        AGS::Common::kPropertyBoolean, AGS::Common::kPropertyUndefined);
}

int Dialog::ReadOptionCount(DocElem elem)
{
    // Option count is not written in AGF, so we have to calculate number of elems
    elem = elem->FirstChildElement("DialogOptions");
    if (!elem)
        return 0;
    int count = 0;
    for (elem = elem->FirstChildElement("DialogOption");
        elem; elem = elem->NextSiblingElement("DialogOption"), count++);
    return count;
}

static FrameAlignment ReadFrameAlignment(const String &value, FrameAlignment def_value)
{
    // These aliases match AGS.Types.FrameAlignment's deserialization rules
    // and keep older Game.agf projects readable.
    if (value.CompareNoCase("Left") == 0)
        return kAlignTopLeft;
    if (value.CompareNoCase("Center") == 0)
        return kAlignTopCenter;
    if (value.CompareNoCase("Centre") == 0 || value.CompareNoCase("Centred") == 0)
        return kAlignMiddleCenter;
    if (value.CompareNoCase("Right") == 0)
        return kAlignTopRight;
    if (value.CompareNoCase("TopMiddle") == 0)
        return kAlignTopCenter;
    if (value.CompareNoCase("BottomMiddle") == 0)
        return kAlignBottomCenter;

    const int index = StrUtil::ParseEnum(value, kFrameAlignmentNames, -1);
    return (index >= 0) ? static_cast<FrameAlignment>(1 << index) : def_value;
}

static HorAlignment ReadHorizontalAlignment(const String &value, HorAlignment def_value)
{
    if (value.CompareNoCase("Left") == 0 || value.CompareNoCase("TopLeft") == 0)
        return kHAlignLeft;
    if (value.CompareNoCase("Center") == 0 || value.CompareNoCase("Centre") == 0 ||
        value.CompareNoCase("TopCenter") == 0 || value.CompareNoCase("TopMiddle") == 0)
        return kHAlignCenter;
    if (value.CompareNoCase("Right") == 0 || value.CompareNoCase("TopRight") == 0)
        return kHAlignRight;
    return def_value;
}

void Cursor::ReadAllData(DocElem elem, DataUtil::CursorData &data)
{
    data.Image = ReadInt(elem, "Image");
    data.HotspotX = ReadInt(elem, "HotspotX");
    data.HotspotY = ReadInt(elem, "HotspotY");
    data.Animate = ReadBool(elem, "Animate");
    data.View = ReadInt(elem, "View", -1);
}

void InventoryItem::ReadAllData(DocElem elem, DataUtil::InventoryItemData &data)
{
    data.Description = ReadString(elem, "Description");
    data.Image = ReadInt(elem, "Image");
    data.CursorImage = ReadInt(elem, "CursorImage");
    data.HotspotX = ReadInt(elem, "HotspotX");
    data.HotspotY = ReadInt(elem, "HotspotY");
    data.PlayerStartsWith = ReadBool(elem, "PlayerStartsWithItem");
}

int GUIMain::ReadID(DocElem elem)
{
    DocElem self = GetNormalGUI(elem);
    if (!self)
        self = GetTextWindow(elem);
    if (!self)
        return -1;
    return ReadInt(self, "ID");
}

String GUIMain::ReadScriptName(DocElem elem)
{
    DocElem self = GetNormalGUI(elem);
    if (!self)
        self = GetTextWindow(elem);
    if (!self)
        return "";
    return ReadString(self, "Name");
}

void GUIMain::ReadAllData(DocElem elem, DataUtil::GUIData& gui_data)
{
    DocElem self = GetNormalGUI(elem);
    if (!self)
        self = GetTextWindow(elem);
    if (!self)
        return;
    gui_data.Clickable = ReadBool(self, "Clickable");
    gui_data.Height = ReadInt(self, "Height");
    gui_data.Left = ReadInt(self, "Left");
    gui_data.Top = ReadInt(self, "Top");
    gui_data.Width = ReadInt(self, "Width");
    gui_data.Transparency = ReadInt(self, "Transparency");
    gui_data.Visible = ReadBool(self, "Visible");
    gui_data.BackgroundColor = ReadInt(self, "BackgroundColor");
    gui_data.BackgroundImage = ReadInt(self, "BackgroundImage");
    gui_data.BorderColor = ReadInt(self, "BorderColor");
    gui_data.OnClick = ReadString(self, "OnClick");
    gui_data.PopupStyle = ReadString(self, "PopupStyle");
    gui_data.ZOrder = ReadInt(self, "ZOrder");
    gui_data.PopupYPos = ReadInt(self, "PopupYPos");
}

DocElem GUIMain::GetNormalGUI(DocElem elem)
{
    return elem->FirstChildElement("NormalGUI");
}

DocElem GUIMain::GetTextWindow(DocElem elem)
{
    return elem->FirstChildElement("TextWindowGUI");
}

String GUIControl::ReadType(DocElem elem)
{
    const char *name = elem->Name();
    if (strcmp(name, "GUIButton") == 0 || strcmp(name, "GUITextWindowEdge") == 0)
        return "Button";
    if (strcmp(name, "GUILabel") == 0)
        return "Label";
    if (strcmp(name, "GUIInventory") == 0)
        return "InvWindow";
    if (strcmp(name, "GUIListBox") == 0)
        return "ListBox";
    if (strcmp(name, "GUISlider") == 0)
        return "Slider";
    if (strcmp(name, "GUITextBox") == 0)
        return "TextBox";
    return "GUIControl";
}

void GUIControl::ReadAllData(DocElem elem, DataUtil::GUIControlData& data)
{
    data.Clickable = ReadBool(elem, "Clickable");
    data.Height = ReadInt(elem, "Height");
    data.Left = ReadInt(elem, "Left");
    data.Top = ReadInt(elem, "Top");
    data.Width = ReadInt(elem, "Width");
    data.Enabled = ReadBool(elem, "Enabled");
    data.Visible = ReadBool(elem, "Visible");
    data.Translated = ReadBool(elem, "Translated");
    data.ZOrder = ReadInt(elem, "ZOrder");
}

void GUIControl::ReadButtonData(DocElem elem, DataUtil::GUIButtonData& data)
{
    data.ClickAction = ReadString(elem, "ClickAction");
    data.ClipImage = ReadBool(elem, "ClipImage");
    data.Font = ReadInt(elem, "Font");
    data.Image = ReadInt(elem, "Image");
    data.MouseoverImage = ReadInt(elem, "MouseoverImage");
    data.NewModeNumber = ReadInt(elem, "NewModeNumber");
    data.OnClick = ReadString(elem, "OnClick");
    data.PushedImage = ReadInt(elem, "PushedImage");
    data.Text = ReadString(elem, "Text");
    data.TextAlignment = ReadFrameAlignment(ReadString(elem, "TextAlignment"), kAlignTopCenter);
    data.TextColor = ReadInt(elem, "TextColor");
}

void GUIControl::ReadLabelData(DocElem elem, DataUtil::GUILabelData& data)
{
    data.Font = ReadInt(elem, "Font");
    data.Text = ReadString(elem, "Text");
    data.TextAlignment = ReadFrameAlignment(ReadString(elem, "TextAlignment"), kAlignTopLeft);
    data.TextColor = ReadInt(elem, "TextColor");
}

void GUIControl::ReadSliderData(DocElem elem, DataUtil::GUISliderData& data)
{
    data.BackgroundImage = ReadInt(elem, "BackgroundImage");
    data.HandleImage = ReadInt(elem, "HandleImage");
    data.HandleOffset = ReadInt(elem, "HandleOffset");
    data.MaxValue = ReadInt(elem, "MaxValue");
    data.MinValue = ReadInt(elem, "MinValue");
    data.OnChange = ReadString(elem, "OnChange");
    data.Value = ReadInt(elem, "Value");
}

void GUIControl::ReadInventoryData(DocElem elem, DataUtil::GUIInventoryData& data)
{
    data.CharacterID = ReadInt(elem, "CharacterID");
    data.ItemHeight = ReadInt(elem, "ItemHeight");
    data.ItemWidth = ReadInt(elem, "ItemWidth");
}

void GUIControl::ReadTextBoxData(DocElem elem, DataUtil::GUITextBoxData& data)
{
    data.Font = ReadInt(elem, "Font");
    data.OnActivate = ReadString(elem, "OnActivate");
    data.ShowBorder = ReadBool(elem, "ShowBorder");
    data.Text = ReadString(elem, "Text");
    data.TextAlignment = ReadFrameAlignment(ReadString(elem, "TextAlignment"), kAlignTopLeft);
    data.TextColor = ReadInt(elem, "TextColor");
}

void GUIControl::ReadListBoxData(DocElem elem, DataUtil::GUIListBoxData& data)
{
    data.Font = ReadInt(elem, "Font");
    data.OnSelectionChanged = ReadString(elem, "OnSelectionChanged");
    data.SelectedBackgroundColor = ReadInt(elem, "SelectedBackgroundColor");
    data.SelectedTextColor = ReadInt(elem, "SelectedTextColor");
    data.ShowBorder = ReadBool(elem, "ShowBorder");
    data.ShowScrollArrows = ReadBool(elem, "ShowScrollArrows");
    data.TextAlignment = ReadHorizontalAlignment(ReadString(elem, "TextAlignment"), kHAlignLeft);
    data.TextColor = ReadInt(elem, "TextColor");
}

AGS::Common::PropertyType CustomPropertySchemaItem::ReadType(DocElem elem)
{
    const String value = ReadString(elem, "Type");
    return ReadCustomPropertyType(value);
}

void GameSettings::ReadGameSettings(DocElem elem, DataUtil::GameSettings& s)
{
    s.AllowRelativeAssetResolutions = ReadBool(elem, "AllowRelativeAssetResolutions");
    s.AlwaysDisplayTextAsSpeech = ReadBool(elem, "AlwaysDisplayTextAsSpeech");
    s.AndroidAppVersionCode = ReadInt(elem, "AndroidAppVersionCode");
    s.AndroidAppVersionName = ReadString(elem, "AndroidAppVersionName");
    s.AndroidApplicationId = ReadString(elem, "AndroidApplicationId");
    s.AndroidBuildFormat = ReadAndroidBuildFormat(ReadString(elem, "AndroidBuildFormat"));
    s.AntiAliasFonts = ReadBool(elem, "AntiAliasFonts");
    s.AntiGlideMode = ReadBool(elem, "AntiGlideMode");
    s.AttachDataToExe = ReadBool(elem, "AttachDataToExe");
    s.AudioIndexer = ReadInt(elem, "AudioIndexer");
    s.AutoMoveInWalkMode = ReadBool(elem, "AutoMoveInWalkMode");
    s.BackwardsText = ReadBool(elem, "BackwardsText");
    s.BuildTargets = ReadString(elem, "BuildTargets");
    s.ClipGUIControls = ReadBool(elem, "ClipGUIControls");
    s.ColorDepth = ReadGameColorDepth(ReadString(elem, "ColorDepth"));
    s.CompressSpritesType = ReadSpriteCompression(ReadString(elem, "CompressSpritesType"));
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
    s.GUIAlphaStyle = ReadGuiAlphaStyle(ReadString(elem, "GUIAlphaStyle"));
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
    s.InventoryHotspotMarkerStyle = ReadInventoryHotspotMarkerStyle(ReadString(elem, "InventoryHotspotMarkerStyle"));
    s.LeftToRightPrecedence = ReadBool(elem, "LeftToRightPrecedence");
    s.LetterboxMode = ReadBool(elem, "LetterboxMode");
    s.MaximumScore = ReadInt(elem, "MaximumScore");
    s.MouseWheelEnabled = ReadBool(elem, "MouseWheelEnabled");
    s.NumberDialogOptions = ReadDialogOptionsNumbering(ReadString(elem, "NumberDialogOptions"));
    s.OptimizeSpriteStorage = ReadBool(elem, "OptimizeSpriteStorage");
    s.PixelPerfect = ReadBool(elem, "PixelPerfect");
    s.PlaySoundOnScore = ReadInt(elem, "PlaySoundOnScore");
    s.ReleaseDate = ReadString(elem, "ReleaseDate");
    s.RenderAtScreenResolution = ReadRenderAtScreenResolution(ReadString(elem, "RenderAtScreenResolution"));
    s.RoomTransition = ReadRoomTransition(ReadString(elem, "RoomTransition"));
    s.RunGameLoopsWhileDialogOptionsDisplayed = ReadBool(elem, "RunGameLoopsWhileDialogOptionsDisplayed");
    s.SaveGameFileExtension = ReadString(elem, "SaveGameFileExtension");
    s.SaveGameFolderName = ReadString(elem, "SaveGameFolderName");
    s.SaveScreenshots = ReadBool(elem, "SaveScreenshots");
    s.ScaleCharacterSpriteOffsets = ReadBool(elem, "ScaleCharacterSpriteOffsets");
    s.ScaleMovementSpeedWithMaskResolution = ReadBool(elem, "ScaleMovementSpeedWithMaskResolution");
    s.ScriptAPIVersion = ReadString(elem, "ScriptAPIVersion");
    s.ScriptCompatLevel = ReadString(elem, "ScriptCompatLevel");
    s.SkipSpeech = ReadSkipSpeech(ReadString(elem, "SkipSpeech"));
    s.SpeechPortraitSide = ReadSpeechPortraitSide(ReadString(elem, "SpeechPortraitSide"));
    s.SpeechStyle = ReadSpeechStyle(ReadString(elem, "SpeechStyle"));
    s.SplitResources = ReadString(elem, "SplitResources");
    s.SpriteAlphaStyle = ReadSpriteAlphaStyle(ReadString(elem, "SpriteAlphaStyle"));
    s.TTFHeightDefinedBy = ReadFontHeightDefinition(ReadString(elem, "TTFHeightDefinedBy"));
    s.TTFMetricsFixup = ReadFontMetricsFixup(ReadString(elem, "TTFMetricsFixup"));
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
    s.WhenInterfaceDisabled = ReadGuiDisableStyle(ReadString(elem, "WhenInterfaceDisabled"));
}

void GlobalVariables::GetAll(DocElem root, std::vector<DocElem> &elems)
{
    DocElem list_node = root->FirstChildElement("GlobalVariables");
    if (!list_node)
        return;
    list_node = list_node->FirstChildElement("Variables");
    if (!list_node)
        return;
    for (DocElem node = list_node->FirstChildElement("GlobalVariable");
        node; node = node->NextSiblingElement("GlobalVariable"))
    {
        elems.push_back(node);
    }
}

void CustomPropertySchema::GetAll(DocElem root, std::vector<DocElem> &elems)
{
    DocElem list_node = root->FirstChildElement("PropertyDefinitions");
    if (!list_node)
        return;
    for (DocElem node = list_node->FirstChildElement("CustomPropertySchemaItem");
        node; node = node->NextSiblingElement("CustomPropertySchemaItem"))
    {
        elems.push_back(node);
    }
}

DocElem Game::GetSettings(DocElem elem)
{
    return elem->FirstChildElement("Settings");
}

DocElem ScriptWithHeader::GetHeader(DocElem elem)
{
    DocElem headelem = elem->FirstChildElement("ScriptAndHeader_Header");
    if (headelem)
        return headelem->FirstChildElement("Script");
    return nullptr;
}

DocElem ScriptWithHeader::GetBody(DocElem elem)
{
    DocElem headelem = elem->FirstChildElement("ScriptAndHeader_Script");
    if (headelem)
        return headelem->FirstChildElement("Script");
    return nullptr;
}


//-----------------------------------------------------------------------------
// Helper functions
//-----------------------------------------------------------------------------

void ReadEntityRef(DataUtil::EntityRef &ent, EntityParser &parser, DocElem elem)
{
    ent.TypeName = parser.ReadType(elem);
    ent.ID = parser.ReadID(elem);
    String name = parser.ReadScriptName(elem);
    // Remove any non-alphanumeric characters from the script name
    for (size_t c = 0; c < name.GetLength(); ++c)
    {
        if (!std::isalnum(name[c]) && name[c] != '_')
            name.ClipMid(c--, 1);
    }
    ent.ScriptName = name;
}

void ReadAllEntityRefs(std::vector<DataUtil::EntityRef> &ents, EntityListParser &list_parser,
    EntityParser &parser, DocElem root)
{
    std::vector<DocElem> elems;
    list_parser.GetAll(root, elems);
    for (const auto &el : elems)
    {
        DataUtil::EntityRef ent;
        ReadEntityRef(ent, parser, el);
        ents.push_back(ent);
    }
}

static void ReadGUI(DataUtil::GUIData& gui_data, AGF::DocElem elem)
{
    AGF::GUIMain gui;
    AGF::GUIControls controls;
    AGF::GUIControl control;

    ReadEntityRef(gui_data, gui, elem);
    gui.ReadAllData(elem, gui_data);

    std::vector<DocElem> elems;
    controls.GetAll(elem, elems);
    for (const auto &el : elems)
    {
        String type = control.ReadType(el);
        if(type == "Button")
        {
            auto data = std::make_shared<DataUtil::GUIButtonData>();
            ReadEntityRef(*data, control, el);
            control.ReadAllData(el, *data);
            control.ReadButtonData(el, *data);
            gui_data.Controls.push_back(data);
        }
        else if(type == "Label")
        {
            auto data = std::make_shared<DataUtil::GUILabelData>();
            ReadEntityRef(*data, control, el);
            control.ReadAllData(el, *data);
            control.ReadLabelData(el, *data);
            gui_data.Controls.push_back(data);
        }
        else if(type == "InvWindow")
        {
            auto data = std::make_shared<DataUtil::GUIInventoryData>();
            ReadEntityRef(*data, control, el);
            control.ReadAllData(el, *data);
            control.ReadInventoryData(el, *data);
            gui_data.Controls.push_back(data);
        }
        else if(type == "ListBox")
        {
            auto data = std::make_shared<DataUtil::GUIListBoxData>();
            ReadEntityRef(*data, control, el);
            control.ReadAllData(el, *data);
            control.ReadListBoxData(el, *data);
            gui_data.Controls.push_back(data);
        }
        else if(type == "Slider")
        {
            auto data = std::make_shared<DataUtil::GUISliderData>();
            ReadEntityRef(*data, control, el);
            control.ReadAllData(el, *data);
            control.ReadSliderData(el, *data);
            gui_data.Controls.push_back(data);
        }
        else if(type == "TextBox")
        {
            auto data = std::make_shared<DataUtil::GUITextBoxData>();
            ReadEntityRef(*data, control, el);
            control.ReadAllData(el, *data);
            control.ReadTextBoxData(el, *data);
            gui_data.Controls.push_back(data);
        }
    }
}

static void ReadDialog(DataUtil::DialogRef &dialog, AGF::DocElem elem)
{
    AGF::Dialog p_dialog;
    ReadEntityRef(dialog, p_dialog, elem);
    dialog.OptionCount = p_dialog.ReadOptionCount(elem);
}

void ReadGlobalVariables(std::vector<DataUtil::Variable> &vars, DocElem root)
{
    AGF::GlobalVariables glvars;
    std::vector<AGF::DocElem> var_elems;
    glvars.GetAll(root, var_elems);
    if (var_elems.size() == 0)
        return;

    AGF::GlobalVariable glvar;
    for (const auto &el : var_elems)
    {
        DataUtil::Variable var;
        var.Type = glvar.ReadType(el);
        var.Name = glvar.ReadScriptName(el);
        var.Value = glvar.ReadDefaultValue(el);
        vars.push_back(var);
    }
}

void ReadCustomPropertySchema(std::vector<DataUtil::CustomPropertySchemaItem> &schema, DocElem root)
{
    AGF::CustomPropertySchema props;
    std::vector<AGF::DocElem> prop_elems;
    props.GetAll(root, prop_elems);
    if (prop_elems.size() == 0)
        return;

    AGF::CustomPropertySchemaItem prop_parser;
    for (const auto &el : prop_elems)
    {
        DataUtil::CustomPropertySchemaItem prop;
        prop.Name = CustomPropertySchemaItem::ReadName(el);
        prop.Description = CustomPropertySchemaItem::ReadDescription(el);
        prop.DefaultValue = CustomPropertySchemaItem::ReadDefaultValue(el);
        prop.Type = CustomPropertySchemaItem::ReadType(el);
        prop.AppliesToCharacters = CustomPropertySchemaItem::ReadAppliesToCharacters(el);
        prop.AppliesToHotspots = CustomPropertySchemaItem::ReadAppliesToHotspots(el);
        prop.AppliesToObjects = CustomPropertySchemaItem::ReadAppliesToObjects(el);
        prop.AppliesToInvItems = CustomPropertySchemaItem::ReadAppliesToInvItems(el);
        prop.AppliesToRooms = CustomPropertySchemaItem::ReadAppliesToRooms(el);
        prop.Translated = CustomPropertySchemaItem::ReadTranslated(el);
        schema.push_back(prop);
    }
}

void ReadGameSettings(DataUtil::GameSettings &opt, DocElem elem)
{
    AGF::Game p_game;
    AGF::GameSettings p_set;
    DocElem set_elem = p_game.GetSettings(elem);
    p_set.ReadGameSettings(set_elem, opt);
}



void ReadGameRef(DataUtil::GameRef &game, AGFReader &reader)
{
    game = DataUtil::GameRef{};
    DocElem root = reader.GetGameRoot();

    game.PlayerCharacter = Game::ReadPlayerCharacter(root);

    // Audio clips
    AGF::AudioClips audioclips;
    AGF::AudioClip audioclip;
    ReadAllEntityRefs(game.AudioClips, audioclips, audioclip, root);
    // Audio types
    AGF::AudioTypes audiotypes;
    AGF::AudioType audiotype;
    ReadAllEntityRefs(game.AudioTypes, audiotypes, audiotype, root);
    // Characters
    AGF::Characters characters;
    AGF::Character character;
    ReadAllEntityRefs(game.Characters, characters, character, root);
    // Cursors
    {
        AGF::Cursors cursors;
        AGF::Cursor cursor;
        std::vector<AGF::DocElem> elems;
        cursors.GetAll(root, elems);
        for (const auto &el : elems)
        {
            DataUtil::CursorData data;
            ReadEntityRef(data, cursor, el);
            cursor.ReadAllData(el, data);
            game.Cursors.push_back(data);
        }
    }
    // Dialogs
    {
        AGF::Dialogs dialogs;
        std::vector<AGF::DocElem> elems;
        dialogs.GetAll(root, elems);
        for (const auto &el : elems)
        {
            DataUtil::DialogRef dialog;
            ReadDialog(dialog, el);
            game.Dialogs.push_back(dialog);
        }
    }
    // Fonts
    AGF::Fonts fonts;
    AGF::Font font;
    ReadAllEntityRefs(game.Fonts, fonts, font, root);
    // GUI and controls
    {
        AGF::GUIs guis;
        AGF::GUIMain gui;
        AGF::GUIControls controls;
        AGF::GUIControl control;
        std::vector<AGF::DocElem> elems;
        guis.GetAll(root, elems);
        for (const auto &el : elems)
        {
            DataUtil::GUIData gui_data;
            ReadGUI(gui_data, el);
            game.GUI.push_back(gui_data);
        }
    }
    // Inventory items
    {
        AGF::Inventory inventory;
        AGF::InventoryItem invitem;
        std::vector<AGF::DocElem> elems;
        inventory.GetAll(root, elems);
        for (const auto &el : elems)
        {
            DataUtil::InventoryItemData data;
            ReadEntityRef(data, invitem, el);
            invitem.ReadAllData(el, data);
            game.Inventory.push_back(data);
        }
    }
    // Views
    AGF::View view;
    AGF::Views views;
    ReadAllEntityRefs(game.Views, views, view, root);

    // Rooms
    ReadRoomList(game.Rooms, root);

    // Global Variables
    ReadGlobalVariables(game.GlobalVars, root);

    // Custom property schema
    ReadCustomPropertySchema(game.PropertySchema, root);

    // Game settings
    ReadGameSettings(game.Settings, root);
}

void ReadScriptList(std::vector<String> &script_list, DocElem root)
{
    AGF::ScriptModules scmodules;
    AGF::ScriptWithHeader scmodule;
    AGF::ScriptElem scelem;
    std::vector<DocElem> modules;
    scmodules.GetAll(root, modules);
    for (const auto &m : modules)
    {
        DocElem body = scmodule.GetBody(m);
        if (!body) continue;
        script_list.push_back(scelem.ReadFilename(body));
    }
}

void ReadScriptHeaderList(std::vector<String> &headers_list, DocElem root)
{
    AGF::ScriptModules scmodules;
    AGF::ScriptWithHeader scmodule;
    AGF::ScriptElem scelem;
    std::vector<DocElem> modules;
    scmodules.GetAll(root, modules);
    for (const auto &m : modules)
    {
        DocElem header = scmodule.GetHeader(m);
        if (!header) continue;
        headers_list.push_back(scelem.ReadFilename(header));
    }
}

void ReadRoomList(std::vector<std::pair<int, String>> &room_list, DocElem root)
{
    AGF::Rooms rooms;
    AGF::Room room;
    std::vector<DocElem> room_els;
    rooms.GetAll(root, room_els);
    for (const auto &r : room_els)
    {
        room_list.push_back(std::make_pair(room.ReadNumber(r), room.ReadDescription(r)));
    }
}

} // namespace AGF
} // namespace AGS
