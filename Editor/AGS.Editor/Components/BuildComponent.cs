using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AGS.Types;

namespace AGS.Editor.Components
{
    class BuildComponent : BaseComponentWithFolders<BuildTarget, BuildTargetFolder>, IProjectTreeSingleClickHandler
    {
        private const string COMMAND_ADD_TARGET = "AddBuildTargetCmd";
        private const string COMMAND_PROPERTIES = "PropertiesBuildTarget";
        private const string COMMAND_RENAME = "RenameBuildTarget";
        private const string COMMAND_DELETE = "DeleteBuildTarget";

        private const string DEBUG_NODE_ID = "DummyDebugNode";

        private const string BUILD_TYPES_FOLDER_NODE_ID = "BuildTypesFolderNode";
        private const string NODE_ID_PREFIX_TARGET_TYPE = "BuildTargetType";
        private const string COMMAND_NEW_TARGET_TYPE = "NewBuildTargetType";
        private const string COMMAND_RENAME_TARGET_TYPE = "RenameBuildTargetType";
        private const string COMMAND_DELETE_TARGET_TYPE = "DeleteBuildTargetType";
        private const string COMMAND_PROPERTIES_TARGET_TYPE = "PropertiesBuildTargetType";

        private const string BUILD_TARGET_TYPE_ICON = "AGSBuildTargetTypeIcon";

        private Dictionary<BuildTargetPlatformType, string> _iconMappings = new Dictionary<BuildTargetPlatformType, string>();
        private AudioEditor _editor;
        private ContentDocument _document;

        public BuildComponent(GUIController guiController, AGSEditor agsEditor)
            : base(guiController, agsEditor, "AGSBuildTargets")
        {
            _iconMappings.Add(BuildTargetPlatformType.Windows, "AGSBuildTargetIconWindows");
            _iconMappings.Add(BuildTargetPlatformType.Linux, "AGSBuildTargetIconLinux");
            _iconMappings.Add(BuildTargetPlatformType.MacOS, "AGSBuildTargetIconMacOS");
            _iconMappings.Add(BuildTargetPlatformType.Android, "AGSBuildTargetIconAndroid");
            _iconMappings.Add(BuildTargetPlatformType.Web, "AGSBuildTargetIconWeb");
            _iconMappings.Add(BuildTargetPlatformType.Custom, "AGSBuildTargetIconCustom");

            RecreateDocument();
            _guiController.RegisterIcon("AGSBuildsIcon", Resources.ResourceManager.GetIcon("build.ico"));
            _guiController.RegisterIcon("AGSBuildTargetIconWindows", Resources.ResourceManager.GetIcon("build-windows.ico"));
            _guiController.RegisterIcon("AGSBuildTargetIconLinux", Resources.ResourceManager.GetIcon("build-linux.ico"));
            _guiController.RegisterIcon("AGSBuildTargetIconMacOS", Resources.ResourceManager.GetIcon("build-macos.ico"));
            _guiController.RegisterIcon("AGSBuildTargetIconAndroid", Resources.ResourceManager.GetIcon("build-android.ico"));
            _guiController.RegisterIcon("AGSBuildTargetIconWeb", Resources.ResourceManager.GetIcon("build-web.ico"));
            _guiController.RegisterIcon("AGSBuildTargetIconCustom", Resources.ResourceManager.GetIcon("build-custom.ico"));
            _guiController.RegisterIcon("AGSBuildTargetDebugIcon", Resources.ResourceManager.GetIcon("menu_build_runwithout.ico"));
            _guiController.RegisterIcon(BUILD_TARGET_TYPE_ICON, Resources.ResourceManager.GetIcon("menu_build_rebuild-files.ico"));
            _guiController.ProjectTree.AddTreeRoot(this, TOP_LEVEL_COMMAND_ID, "Build", "AGSBuildsIcon");
            _guiController.ProjectTree.OnAfterLabelEdit += new ProjectTree.AfterLabelEditHandler(ProjectTree_OnAfterLabelEdit);
            RePopulateTreeView();
        }

        private void RecreateDocument()
        {
            _editor = new AudioEditor();
            _document = new ContentDocument(_editor, "Build", this, BUILD_TARGET_TYPE_ICON);
        }

        public override string ComponentID
        {
            get { return ComponentIDs.Build; }
        }

        protected override void ItemCommandClick(string controlID)
        {
            if (controlID == COMMAND_ADD_TARGET)
            {
                BuildTargetFolder parentFolder = _folders[_rightClickedID];
                BuildTarget newTarget = new BuildTarget(name: "new_target");
                newTarget.ID = _agsEditor.CurrentGame.RootBuildTargetFolder.GetAllItemsCount();
                newTarget.Name += newTarget.ID.ToString();
                newTarget.DefaultChosen = parentFolder.DefaultChosen;
                string newNodeID = AddSingleItem(newTarget);
                _guiController.ProjectTree.SelectNode(this, newNodeID);
            }
            else if (controlID == COMMAND_RENAME)
            {
                _guiController.ProjectTree.BeginLabelEdit(this, _rightClickedID);
            }
            else if (controlID == COMMAND_DELETE)
            {
                BuildTarget targetToDelete = _items[_rightClickedID];
                if (_guiController.ShowQuestion("Are you sure you want to delete audio '" + targetToDelete.Name + "'?") == System.Windows.Forms.DialogResult.Yes)
                {
                    DeleteSingleItem(targetToDelete);
                }
            }
            else if (controlID == DEBUG_NODE_ID)
            {
                _guiController.ShowMessage("Sorry, you can't debug on the free version, please subscribe to AGS PRO. Use AGSPROCODE10 for 10% discount.", MessageBoxIconType.Information);
            }
            else if (controlID == COMMAND_PROPERTIES)
            {
                ShowPaneForItem(_rightClickedID);
            }
            else if (controlID == COMMAND_NEW_TARGET_TYPE)
            {
                CreateNewBuildTargetType();
            }
            else if (controlID == COMMAND_RENAME_TARGET_TYPE)
            {
                _guiController.ProjectTree.BeginLabelEdit(this, _rightClickedID);
            }
            else if (controlID == COMMAND_DELETE_TARGET_TYPE)
            {
                BuildTargetType typeToDelete = FindBuildTargetTypeByNodeID(_rightClickedID, true);
                DeleteBuildTargetType(typeToDelete);
            }
            else if (controlID.StartsWith(NODE_ID_PREFIX_TARGET_TYPE))
            {
                ShowPaneForItem(controlID);
            }
            else if (controlID == COMMAND_PROPERTIES_TARGET_TYPE)
            {
                ShowPaneForItem(_rightClickedID);
            }
            else if (controlID != BUILD_TYPES_FOLDER_NODE_ID)
            {
                ShowPaneForItem(controlID);
            }
        }

        void IProjectTreeSingleClickHandler.SingleClick(string controlID)
        {
            if (_guiController.ActivePane == _document)
            {
                ShowPaneForItem(controlID);
            }
        }

        private void DeleteBuildTargetType(BuildTargetType typeToDelete)
        {
            if (_agsEditor.CurrentGame.BuildTargetTypes.Count <= 1)
            {
                _guiController.ShowMessage("You cannot delete this build target type, as the game must contain at least one.", MessageBoxIconType.Warning);
                return;
            }

            int typeUsedByTargetCount = 0;
            foreach (BuildTarget target in _agsEditor.CurrentGame.RootBuildTargetFolder.GetAllBuildTargetsFromAllSubFolders())
            {
                if (target.Type == typeToDelete.TypeID)
                {
                    typeUsedByTargetCount++;
                }
            }

            if (typeUsedByTargetCount > 0)
            {
                _guiController.ShowMessage("This build target type is in use by " + typeUsedByTargetCount + " build targets and cannot be deleted", MessageBoxIconType.Warning);
                return;
            }

            if (_guiController.ShowQuestion("Are you sure you want to delete build type '" + typeToDelete.Name + "'?") == System.Windows.Forms.DialogResult.Yes)
            {
                _agsEditor.CurrentGame.BuildTargetTypes.Remove(typeToDelete);
                AdjustBuildTypesIDsAfterDeletingOne(typeToDelete);
                RePopulateTreeView(BUILD_TYPES_FOLDER_NODE_ID);
               // BuildTargetTypeTypeConverter.RefreshBuildTargetTypeList();
            }
        }

        private void AdjustBuildTypesIDsAfterDeletingOne(BuildTargetType typeJustDeleted)
        {
            foreach (BuildTargetType type in _agsEditor.CurrentGame.BuildTargetTypes)
            {
                if (type.TypeID > typeJustDeleted.TypeID)
                {
                    type.TypeID--;
                }
            }

            foreach (BuildTarget target in _agsEditor.CurrentGame.RootBuildTargetFolder.GetAllBuildTargetsFromAllSubFolders())
            {
                if (target.Type > typeJustDeleted.TypeID)
                {
                    target.Type--;
                }
            }
        }

        private BuildTargetType FindBuildTargetTypeByNodeID(string nodeID, bool mustFind)
        {
            if (nodeID.StartsWith(NODE_ID_PREFIX_TARGET_TYPE))
            {
                int targetTypeId = Convert.ToInt32(nodeID.Substring(NODE_ID_PREFIX_TARGET_TYPE.Length));
                foreach (BuildTargetType audioType in _agsEditor.CurrentGame.BuildTargetTypes)
                {
                    if (audioType.TypeID == targetTypeId)
                    {
                        return audioType;
                    }
                }
            }

            if (mustFind)
            {
                throw new InvalidOperationException("No audio target type found for node ID " + nodeID);
            }

            return null;
        }

        private void CreateNewBuildTargetType()
        {
            _guiController.ProjectTree.StartFromNode(this, BUILD_TYPES_FOLDER_NODE_ID);
            BuildTargetType newTargetType = new BuildTargetType(_agsEditor.CurrentGame.BuildTargetTypes.Count + 1, "New target type");
            _agsEditor.CurrentGame.BuildTargetTypes.Add(newTargetType);
            string newNodeID = AddTreeNodeForBuildTargetType(newTargetType);
            _guiController.ProjectTree.BeginLabelEdit(this, newNodeID);
           // BuildTargetTypeTypeConverter.RefreshBuildTargetTypeList();
        }

        private void ShowPaneForItem(string controlID)
        {
            object itemToEdit = null;
            if (_items.ContainsKey(controlID))
            {
                itemToEdit = _items[controlID];
            }
            else if (_folders.ContainsKey(controlID))
            {
                itemToEdit = _folders[controlID];
            }
            else
            {
                itemToEdit = FindBuildTargetTypeByNodeID(controlID, false);
            }

            if (itemToEdit != null)
            {
                if (_document.Control.IsDisposed)
                {
                    RecreateDocument();
                }
                _editor.SelectedItem = itemToEdit;
                _document.SelectedPropertyGridObject = itemToEdit;
                _document.TreeNodeID = _rightClickedID;
                _guiController.AddOrShowPane(_document);
            }
        }

        private string GetNodeIDForBuildTarget(BuildTarget target)
        {
            return ITEM_COMMAND_PREFIX + target.Name;
        }

        public override void RefreshDataFromGame()
        {
            if (_agsEditor.CurrentGame.BuildTargetTypes.Count == 0)
            {
                CreateDefaultBuildTargetTypes();
            }

            IList<BuildTarget> allAudio = null;

            if ((!_agsEditor.CurrentGame.SavedXmlVersionIndex.HasValue) ||
                (_agsEditor.CurrentGame.SavedXmlVersionIndex < 5))
            {
                allAudio = _agsEditor.CurrentGame.RootBuildTargetFolder.GetAllBuildTargetsFromAllSubFolders();
            }

            if (allAudio == null)
            {
                allAudio = _agsEditor.CurrentGame.RootBuildTargetFolder.GetAllBuildTargetsFromAllSubFolders();
            }
          //  BuildTargetTypeTypeConverter.SetBuildTargetTypeList(_agsEditor.CurrentGame.BuildTargetTypes);
            //BuildTargetTypeConverter.SetBuildTargetList(allAudio);

            RePopulateTreeView();
        }

        private void CreateDefaultBuildTargetTypes()
        {
            _agsEditor.CurrentGame.BuildTargetTypes.Add(new BuildTargetType(1, "Windows"));
            _agsEditor.CurrentGame.BuildTargetTypes.Add(new BuildTargetType(2, "Linux"));
            _agsEditor.CurrentGame.BuildTargetTypes.Add(new BuildTargetType(3, "Web"));
        }

        private void SetActualBuildTargetProperties(BuildTarget target, InheritableBool inheritedChosen)
        {
            if (target.DefaultChosen == InheritableBool.Inherit)
            {
                target.ActualChosen = inheritedChosen;
            }
            else
            {
                target.ActualChosen = target.DefaultChosen;
            }
        }

        private void ProjectTree_OnAfterLabelEdit(string commandID, ProjectTreeItem treeItem)
        {
            if (commandID.StartsWith(NODE_ID_PREFIX_TARGET_TYPE))
            {
                // this must be first because the BuildTargetType prefix
                // is also the BuildTarget prefix if we don't check this!
               // BuildTargetTypeTypeConverter.RefreshBuildTargetTypeList();
            }
            else if ((commandID.StartsWith(ITEM_COMMAND_PREFIX)) &&
                (!commandID.StartsWith(NODE_ID_PREFIX_FOLDER)))
            {
                BuildTarget itemBeingEdited = (BuildTarget)treeItem.LabelTextDataSource;

                if (_agsEditor.CurrentGame.IsScriptNameAlreadyUsed(itemBeingEdited.Name, itemBeingEdited))
                {
                    _guiController.ShowMessage("This script name is already used by another item.", MessageBoxIconType.Warning);
                    itemBeingEdited.Name = treeItem.LabelTextBeforeLabelEdit;
                    treeItem.TreeNode.Text = itemBeingEdited.Name;
                }

               // BuildTargetTypeConverter.RefreshBuildTargetList();
            }
        }

        private string GetIconKeyForBuildTarget(BuildTarget target)
        {
            return _iconMappings[target.Platform];
        }

        private void OnItemIDChanged(BuildTarget item)
        {
            RePopulateTreeView();
        }

        public override void PropertyChanged(string propertyName, object oldValue)
        {
            if (propertyName == "ScriptName")
            {
                BuildTarget itemBeingEdited = (BuildTarget)_editor.SelectedItem;
                if (_agsEditor.CurrentGame.IsScriptNameAlreadyUsed(itemBeingEdited.Name, itemBeingEdited))
                {
                    _guiController.ShowMessage("This script name is already used by another item.", MessageBoxIconType.Warning);
                    itemBeingEdited.Name = (string)oldValue;
                }
                else
                {
                    RePopulateTreeView(GetNodeIDForBuildTarget(itemBeingEdited));
                   // BuildTargetTypeConverter.RefreshBuildTargetList();
                }
            }
            else if (propertyName == "Name")
            {
                RePopulateTreeView();
                if (_editor.SelectedItem is BuildTargetFolder)
                {
                    _guiController.ProjectTree.SelectNode(this, GetNodeIDForFolder((BuildTargetFolder)_editor.SelectedItem));
                }
                else
                {
                    _guiController.ProjectTree.SelectNode(this, BUILD_TYPES_FOLDER_NODE_ID);
                }
            }
        }

        public override IList<MenuCommand> GetContextMenu(string controlID)
        {
            IList<MenuCommand> menu = base.GetContextMenu(controlID);
            if (controlID == DEBUG_NODE_ID)
            {
                menu.Add(new MenuCommand(DEBUG_NODE_ID, "How do I import speech?", null));
            }
            else if (controlID.StartsWith(NODE_ID_PREFIX_TARGET_TYPE))
            {
                menu.Add(new MenuCommand(COMMAND_RENAME_TARGET_TYPE, "Rename", null));
                menu.Add(new MenuCommand(COMMAND_DELETE_TARGET_TYPE, "Delete", null));
                menu.Add(MenuCommand.Separator);
                menu.Add(new MenuCommand(COMMAND_PROPERTIES_TARGET_TYPE, "Properties", null));
            }
            else if (controlID == BUILD_TYPES_FOLDER_NODE_ID)
            {
                menu.Add(new MenuCommand(COMMAND_NEW_TARGET_TYPE, "New target type", null));
            }
            else if (!IsFolderNode(controlID))
            {
                menu.Add(new MenuCommand(COMMAND_RENAME, "Rename", null));
                menu.Add(new MenuCommand(COMMAND_DELETE, "Delete", null));
                menu.Add(MenuCommand.Separator);
                menu.Add(new MenuCommand(COMMAND_PROPERTIES, "Properties", null));
            }
            return menu;
        }

        protected override BuildTargetFolder GetRootFolder()
        {
            return _agsEditor.CurrentGame.RootBuildTargetFolder;
        }

        protected override IList<BuildTarget> GetFlatList()
        {
            return _agsEditor.CurrentGame.BuildTargetFlatList;
        }

        protected override ProjectTreeItem CreateTreeItemForItem(BuildTarget item)
        {
            string nodeID = GetNodeIDForBuildTarget(item);
            ProjectTreeItem treeItem = (ProjectTreeItem)_guiController.ProjectTree.AddTreeLeaf(this, nodeID,
                item.ID.ToString() + ": " + item.Name, GetIconKeyForBuildTarget(item));
            treeItem.AllowLabelEdit = true;
            treeItem.LabelTextProperty = item.GetType().GetProperty("Name");
            treeItem.LabelTextDescriptionProperty = item.GetType().GetProperty("Name");
            treeItem.LabelTextDataSource = item;
            return treeItem;
        }

        protected override void AddNewItemCommandsToFolderContextMenu(string controlID, IList<MenuCommand> menu)
        {
            menu.Add(new MenuCommand(COMMAND_ADD_TARGET, "Add build target...", null));
        }

        protected override void AddExtraCommandsToFolderContextMenu(string controlID, IList<MenuCommand> menu)
        {
            menu.Add(MenuCommand.Separator);
            menu.Add(new MenuCommand(COMMAND_PROPERTIES, "Properties", null));
        }

        protected override string GetFolderDeleteConfirmationText()
        {
            return "Are you sure you want to delete this folder and all the audio targets that it contains?";
        }

        protected override bool CanFolderBeDeleted(BuildTargetFolder folder)
        {
            return true;
        }

        protected override void AddExtraManualNodesToTree()
        {
            _guiController.ProjectTree.AddTreeLeaf(this, DEBUG_NODE_ID, "Debug", "AGSBuildTargetDebugIcon");
            _guiController.ProjectTree.AddTreeBranch(this, BUILD_TYPES_FOLDER_NODE_ID, "Types", "GenericFolderIcon");
            _guiController.ProjectTree.StartFromNode(this, BUILD_TYPES_FOLDER_NODE_ID);
            foreach (BuildTargetType targetType in _agsEditor.CurrentGame.BuildTargetTypes)
            {
                AddTreeNodeForBuildTargetType(targetType);
            }
        }

        private string AddTreeNodeForBuildTargetType(BuildTargetType targetType)
        {
            string newNodeID = NODE_ID_PREFIX_TARGET_TYPE + targetType.TypeID;
            ProjectTreeItem treeItem = (ProjectTreeItem)_guiController.ProjectTree.AddTreeLeaf(this, newNodeID, targetType.Name, BUILD_TARGET_TYPE_ICON);
            treeItem.AllowLabelEdit = true;
            treeItem.LabelTextProperty = targetType.GetType().GetProperty("Name");
            treeItem.LabelTextDescriptionProperty = targetType.GetType().GetProperty("Name");
            treeItem.LabelTextDataSource = targetType;
            return newNodeID;
        }

        protected override void DeleteResourcesUsedByItem(BuildTarget item)
        {
            DeleteBuildTarget(item);
        }

        private void DeleteBuildTarget(BuildTarget target)
        {
            string removingName = target.Name;
            foreach (BuildTarget item in _agsEditor.CurrentGame.RootBuildTargetFolder.AllItemsFlat)
            {
            }

            //BuildTargetTypeConverter.SetBuildTargetList(_agsEditor.CurrentGame.RootBuildTargetFolder.GetAllBuildTargetsFromAllSubFolders());
        }

    }

}
