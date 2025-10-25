using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AGS.Types;

namespace AGS.Editor.Components
{
    class CustomPropertiesComponent : BaseComponent
    {
        private const string TOP_LEVEL_COMMAND_ID = "CustomProperties";
        private const string ICON_KEY = "CustomPropertiesIcon";
        private const string COMMAND_SHOW_CUSTOM_PROPERTY_SCHEMA_EDITOR = "ShowCustomPropertySchemaEditor";
        private const string COMMAND_IMPORT_CUSTOM_PROPERTIES = "ImportCustomProperties";
        private const string COMMAND_EXPORT_CUSTOM_PROPERTIES = "ExportCustomProperties";

        public CustomPropertiesComponent(GUIController guiController, AGSEditor agsEditor)
           : base(guiController, agsEditor)
        {
            _guiController.RegisterIcon(ICON_KEY, Resources.ResourceManager.GetIcon("customproperties.ico"));
            _guiController.ProjectTree.AddTreeRoot(this, TOP_LEVEL_COMMAND_ID, "Custom Properties", ICON_KEY);
        }

        private void ShowSchemaEditor()
        {
            CustomPropertySchemaEditor schemaEditor = new CustomPropertySchemaEditor(_agsEditor.CurrentGame.PropertySchema);
            schemaEditor.ShowDialog();
            schemaEditor.Dispose();
        }

        private void Export()
        {
            CustomPropertySchema schema = _agsEditor.CurrentGame.PropertySchema;
            Game game = _agsEditor.CurrentGame;
            string filename = "example.xml";
            ImportExport.ExportCustomPropertiesSchemaToFile(schema, filename, game);
        }

        private void Import()
        {
            Game game = _agsEditor.CurrentGame;
            string filename = "example.xml";
            CustomPropertySchema schema = ImportExport.ImportCustomPropertiesSchemaFromFile(filename, game);
        }

        public override void CommandClick(string controlID)
        {
            if (controlID == TOP_LEVEL_COMMAND_ID || controlID == COMMAND_SHOW_CUSTOM_PROPERTY_SCHEMA_EDITOR)
            {
                // should show schema editor on either double click or selecting edit schema in right click menu
                ShowSchemaEditor();
            }
            else if (controlID == COMMAND_IMPORT_CUSTOM_PROPERTIES)
            {
                Import();
            }
            else if (controlID == COMMAND_EXPORT_CUSTOM_PROPERTIES)
            {
                Export();
            }
        }

        public override IList<MenuCommand> GetContextMenu(string controlID)
        {
            IList<MenuCommand> menu = new List<MenuCommand>();
            if (controlID == TOP_LEVEL_COMMAND_ID)
            {
                menu.Add(new MenuCommand(COMMAND_SHOW_CUSTOM_PROPERTY_SCHEMA_EDITOR, "Edit Schema...", null));
                menu.Add(MenuCommand.Separator);
                menu.Add(new MenuCommand(COMMAND_IMPORT_CUSTOM_PROPERTIES, "Import...", null));
                menu.Add(new MenuCommand(COMMAND_EXPORT_CUSTOM_PROPERTIES, "Export...", null));
            }
            return menu;
        }

        public override string ComponentID
        {
            get { return ComponentIDs.CustomProperties; }
        }
    }
}
