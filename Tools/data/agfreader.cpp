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
    if (strcmp(attr_filever, LATEST_XML_VERSION))
        return new Error(String::FromFormat("Unsupported Game.agf format: %s", attr_filever));

    const int attr_format = _doc->RootElement()->IntAttribute(XML_ATTRIBUTE_VERSION_INDEX);
    const char *attr_editorver = _doc->RootElement()->Attribute(XML_ATTRIBUTE_EDITOR_VERSION);
    Debug::Printf("AGFReader: opened %s,\n format tag: %s\n format index: %d\n saved by AGS %s",
        filename, attr_filever, attr_format, attr_editorver);

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
    DocElem node = list_root;
    if (folder_elem)
    { // Get the main folder
        node = node->FirstChildElement(folder_elem);
    }
    return GetElemsRecursive(node, elems, folder_elem, list_elem, type_elem);
}

void EntityListParser::GetAllElems(DocElem game_root, std::vector<DocElem> &elems,
    const char *root_elem, const char *folder_elem, const char *list_elem, const char *type_elem)
{
    DocElem list_root = game_root->FirstChildElement(root_elem);
    DocElem node = list_root;
    if (folder_elem)
    { // Get the main folder
        node = node->FirstChildElement(folder_elem);
    }
    return GetElemsRecursive(node, elems, folder_elem, list_elem, type_elem);
}

void EntityListParser::GetElemsRecursive(DocElem folder,  std::vector<DocElem> &elems,
     const char *folder_elem, const char *list_elem, const char *type_elem)
{
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
    DocElem name_f = elem->FirstChildElement(field);
    if (name_f)
        return name_f->GetText();
    return def_value;
}

int ValueParser::ReadInt(DocElem elem, const char *field, int def_value)
{
    DocElem name_f = elem->FirstChildElement(field);
    if (name_f)
        return StrUtil::StringToInt(name_f->GetText(), def_value);
    return def_value;
}

bool ValueParser::ReadBool(DocElem elem, const char *field, bool def_value)
{
    DocElem name_f = elem->FirstChildElement(field);
    if (name_f)
        return ags_stricmp(name_f->GetText(), "True") == 0;
    return def_value;
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
    data.TextAlignment = ReadString(elem, "TextAlignment");
    data.TextColor = ReadInt(elem, "TextColor");
}

void GUIControl::ReadLabelData(DocElem elem, DataUtil::GUILabelData& data)
{
    data.Font = ReadInt(elem, "Font");
    data.Text = ReadString(elem, "Text");
    data.TextAlignment = ReadString(elem, "TextAlignment");
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
    data.TextAlignment = ReadString(elem, "TextAlignment");
    data.TextColor = ReadInt(elem, "TextColor");
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
            DataUtil::GUIButtonData button;
            ReadEntityRef(button, control, el);
            gui_data.Controls.push_back(button);
        }
        else if(type == "Label")
        {
            DataUtil::GUILabelData label;
            ReadEntityRef(label, control, el);
            gui_data.Controls.push_back(label);
        }
        else if(type == "InvWindow")
        {
            DataUtil::GUIInventoryData inventory;
            ReadEntityRef(inventory, control, el);
            gui_data.Controls.push_back(inventory);
        }
        else if(type == "ListBox")
        {
            DataUtil::GUIListBoxData listbox;
            ReadEntityRef(listbox, control, el);
            gui_data.Controls.push_back(listbox);
        }
        else if(type == "Slider")
        {
            DataUtil::GUISliderData slider;
            ReadEntityRef(slider, control, el);
            gui_data.Controls.push_back(slider);
        }
        else if(type == "TextBox")
        {
            DataUtil::GUITextBoxData textbox;
            ReadEntityRef(textbox, control, el);
            gui_data.Controls.push_back(textbox);
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

void ReadGameSettings(DataUtil::GameSettings &opt, DocElem elem)
{
    AGF::Game p_game;
    AGF::GameSettings p_set;
    DocElem set_elem = p_game.GetSettings(elem);
    p_set.ReadGameSettings(set_elem, opt);
}



void ReadGameRef(DataUtil::GameRef &game, AGFReader &reader)
{
    DocElem root = reader.GetGameRoot();
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
    AGF::Cursors cursors;
    AGF::Cursor cursor;
    ReadAllEntityRefs(game.Cursors, cursors, cursor, root);
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
            DataUtil::GUIRef gui_ent;
            ReadEntityRef(gui_ent, gui, el);
            ReadAllEntityRefs(gui_ent.Controls, controls, control, el);
            game.GUI.push_back(gui_ent);
        }
    }
    // Inventory items
    AGF::Inventory inventory;
    AGF::InventoryItem invitem;
    ReadAllEntityRefs(game.Inventory, inventory, invitem, root);
    // Views
    AGF::View view;
    AGF::Views views;
    ReadAllEntityRefs(game.Views, views, view, root);

    // Global Variables
    ReadGlobalVariables(game.GlobalVars, root);

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
