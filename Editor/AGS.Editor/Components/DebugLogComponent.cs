﻿using AGS.Types;
using System;
using System.Collections.Generic;
using System.Text;
using AGS.Editor.Preferences;
using System.Xml;
using System.Drawing;

namespace AGS.Editor.Components
{
    internal class DebugLogComponent : BaseComponent
    {
        private LogPanel _logPanel;
        private DebugLog _logConfig;  // Particular settings on logging from the game when running the game in debug mode in the Editor
        private ContentDocument _document;
        private MenuCommands _menuCommands = new MenuCommands(GUIController.HELP_MENU_ID, 500);

        private const string ICON_KEY = "MenuIconShowDebugLog";
        private const string SHOW_DEBUG_LOG_COMMAND = "ShowDebugLog";


        public DebugLogComponent(GUIController guiController, AGSEditor agsEditor)
            : base(guiController, agsEditor)
        {
            _logPanel = new LogPanel(guiController);
            _document = new ContentDocument(_logPanel, "Debug Log", this, ICON_KEY);
            _document.PreferredDockData = new DockData(DockingState.DockBottom, Rectangle.Empty);
            _logConfig = new DebugLog();

            _document.SelectedPropertyGridObject = _logConfig;

            _guiController.RegisterIcon(ICON_KEY, Resources.ResourceManager.GetIcon("debug_log.ico"));

            _menuCommands.Commands.Add(new MenuCommand(SHOW_DEBUG_LOG_COMMAND, "Show Debug Log", ICON_KEY));
            _guiController.AddMenuItems(this, _menuCommands);
            _guiController.SetLogPanel(_logPanel);
        }

        public override string ComponentID
        {
            get { return ComponentIDs.DebugLog; }
        }
        
        public override void FromXml(XmlNode node)
        {
            if (node != null)
            {
                try
                {
                    _logConfig.FromXml(node);
                } 
                catch
                {
                    _logConfig.SetDefaults();
                }
            }
        }

        public override void ToXml(XmlTextWriter writer)
        {
            _logConfig.ToXml(writer);
        }

        public override void CommandClick(string controlID)
        {
            if (controlID == SHOW_DEBUG_LOG_COMMAND)
            {
                _guiController.AddOrShowPane(_document);
            }
        }

        public override void RefreshDataFromGame()
        {
            // Game has just been loaded
            if (Factory.AGSEditor.Settings.StartupPane == StartupPane.StartPage)
            {
                _guiController.AddOrShowPane(_document);
            }
        }
    }
}
