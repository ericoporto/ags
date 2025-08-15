using AGS.Types;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace AGS.Editor
{
    public partial class GlobalVariablesEditor : EditorContentPanel
    {
        private const string MENU_ITEM_RENAME = "Rename";
        private const string MENU_ITEM_CREATE_SUB_FOLDER = "NewFolder";
        private const string MENU_ITEM_DELETE_FOLDER = "DeleteFolder";

        private const string MENU_ITEM_EDIT_WORD = "EditWord";
        private const string MENU_ITEM_ADD_SYNONYM = "AddSynonym";
        private const string MENU_ITEM_DELETE_WORD = "DeleteWord";
        private const string MENU_ITEM_ADD_WORD = "AddWord";
        private const string MENU_ITEM_FIND_WORD = "FindWord";
        private const int SUB_ITEM_INDEX_WORD_TEXT = 1;

        private static ImageList _gvManagerIcons;
        private Dictionary<string, GlobalVariableFolder> _folders;
        private Dictionary<GlobalVariableFolder, TreeNode> _folderNodeMapping;
        private Dictionary<TreeNode, GlobalVariableFolder> _nodeFolderMapping;
        private GlobalVariableFolder _currentFolder;
        private GlobalVariableFolder _rootFolder;
        private TreeNode _rootTreeNode;

        public delegate void GlobalVariableChangedHandler();
        public event GlobalVariableChangedHandler GlobalVariableChanged;

        private GlobalVariables _variables;
        private Game _game;
        private SortedSet<APITypeDef> _apiTypes = new SortedSet<APITypeDef>();

        public GlobalVariablesEditor(Game game)
        {
            InitializeComponent();
            globalVariableList.ListViewItemSorter = new GlobalVariableComparer();
            _game = game;
            _variables = game.GlobalVariables;

            globalVariableList.Sorting = SortOrder.Ascending;
            foreach (GlobalVariable variable in _variables.ToList())
            {
                globalVariableList.Items.Add(CreateListItemFromVariable(variable));
            }
            globalVariableList.Sort();

            try
            {
                PopulateScriptTypeList();
            }
            catch (Exception) { }


            _folders = new Dictionary<string, GlobalVariableFolder>(
                // The TreeNodeCollection uses case-insensitive string comparer
                StringComparer.Create(System.Globalization.CultureInfo.CurrentCulture, true));
            _folderNodeMapping = new Dictionary<GlobalVariableFolder, TreeNode>();
            _nodeFolderMapping = new Dictionary<TreeNode, GlobalVariableFolder>();

            if (_gvManagerIcons == null)
            {
                _gvManagerIcons = new ImageList();
                _gvManagerIcons.Images.Add("Folder", Resources.ResourceManager.GetIcon("folder.ico"));
                _gvManagerIcons.Images.Add("OpenFolder", Resources.ResourceManager.GetIcon("openfldr.ico"));
            }
            folderList.ImageList = _gvManagerIcons;
            folderList.KeyDown += new System.Windows.Forms.KeyEventHandler(this.folderList_KeyDown);
            folderList.ItemTryDrag += FolderList_ItemTryDrag;
            folderList.ItemDragOver += FolderList_ItemDragOver;
            folderList.ItemDragDrop += FolderList_ItemDragDrop;
        }

        /// <summary>
        /// Gets the global variable out of the global variable list item 
        /// </summary>
        private GlobalVariable GetGlobalVariable(ListViewItem item)
        {
            return item.Tag as GlobalVariable;
        }

        private void DisplayGlobalVariablesForFolder(GlobalVariableFolder folder)
        {
            if (folder == null) return;

            _currentFolder = folder;
            globalVariableList.BeginUpdate();
            globalVariableList.Clear();
            List<ListViewItem> itemsToAdd = new List<ListViewItem>();
            for (int index = 0; index < folder.GlobalVariables.Count; index++)
            {
                GlobalVariable globalVariable = folder.GlobalVariables[index];
                itemsToAdd.Add(CreateListItemFromVariable(globalVariable));
            }

            globalVariableList.Items.AddRange(itemsToAdd.ToArray());
            globalVariableList.EndUpdate();
        }


        public void SetDataSource(GlobalVariableFolder rootFolder)
        {
            // store the hash for each expanded sprite folder
            List<int> expanded = new List<int>();
            for (int i = 0; i < folderList.Nodes.Count; i++)
            {
                AddNodeState(_nodeFolderMapping[folderList.Nodes[i]], expanded);
            }

            // reset - this could be adding a sprite or loading another game
            _rootFolder = rootFolder;
            folderList.Nodes.Clear();
            _folders.Clear();
            _folderNodeMapping.Clear();
            _nodeFolderMapping.Clear();
            BuildNodeTree(rootFolder, folderList.Nodes);

            // re-expand nodes where they look to be the same
            foreach (GlobalVariableFolder folder in _folderNodeMapping.Keys)
            {
                if (expanded.Contains(folder.GetHashCode()))
                {
                    _folderNodeMapping[folder].Expand();
                }
            }

            if (_currentFolder != null && _folderNodeMapping.ContainsKey(_currentFolder))
            {
                // reselect the previous node
                folderList.SelectedNode = _folderNodeMapping[_currentFolder];
                DisplayGlobalVariablesForFolder(_currentFolder);
            }
            else
            {
                // default to expanded root node
                folderList.SelectedNode = folderList.Nodes[0];
                DisplayGlobalVariablesForFolder(rootFolder);
                folderList.Nodes[0].Expand();
            }

            _rootTreeNode = folderList.Nodes[0];
        }

        private void AddNodeState(GlobalVariableFolder folder, List<int> expanded)
        {
            if (_folderNodeMapping.ContainsKey(folder) &&
                _folderNodeMapping[folder].IsExpanded)
            {
                expanded.Add(folder.GetHashCode());
            }

            foreach (GlobalVariableFolder subfolder in folder.SubFolders)
            {
                AddNodeState(subfolder, expanded);
            }
        }

        private void BuildNodeTree(GlobalVariableFolder folder, TreeNodeCollection parent)
        {
            TreeNode newNode = AddTreeNode(folder, parent);

            foreach (GlobalVariableFolder subFolder in folder.SubFolders)
            {
                BuildNodeTree(subFolder, newNode.Nodes);
            }
        }

        private TreeNode AddTreeNode(GlobalVariableFolder folder, TreeNodeCollection parent)
        {
            string nodeID = folder.Name;
            while (_folders.ContainsKey(nodeID))
            {
                nodeID = nodeID + "A";
            }
            TreeNode addedNode = parent.Add(nodeID, folder.Name, "Folder", "OpenFolder");
            _folders.Add(nodeID, folder);
            _folderNodeMapping.Add(folder, addedNode);
            _nodeFolderMapping.Add(addedNode, folder);
            return addedNode;
        }


        private void folderList_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.F2)
            {
                if (folderList.SelectedNode != null)
                    folderList.SelectedNode.BeginEdit();
            }
        }



        /// <summary>
        /// Fully repopulates the global variable listview.
        /// </summary>
        private void RefreshGlobalVariableDisplay()
        {
          /*   DisplayGlobalVariablesForFolder(_currentFolder);

            if (_sendUpdateNotifications)
            {
                _rootFolder.NotifyClientsOfUpdate();
            }*/
        }

        private void globalVariableList_ItemDrag(object sender, ItemDragEventArgs e)
        {
            GlobalVariableManagerDragDropData dragDropData = new GlobalVariableManagerDragDropData();

            foreach (ListViewItem selectedItem in globalVariableList.SelectedItems)
            {
                dragDropData.GlobalVariables.Add(GetGlobalVariable(selectedItem));
            }

            this.DoDragDrop(dragDropData, DragDropEffects.Move);
        }

        private void globalVariableList_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(GlobalVariableManagerDragDropData)))
            {
                // Moving a globalVariable already imported
                GlobalVariableManagerDragDropData dragged = (GlobalVariableManagerDragDropData)e.Data.GetData(typeof(GlobalVariableManagerDragDropData));
                Point locationInControl = globalVariableList.PointToClient(new Point(e.X, e.Y));
                bool putGlobalVariablesBeforeSelection = true;
                ListViewItem nearestItem = globalVariableList.HitTest(locationInControl).Item;
                if (nearestItem == null)
                {
                    putGlobalVariablesBeforeSelection = false;
                    nearestItem = globalVariableList.FindNearestItem(SearchDirectionHint.Left, locationInControl);

                    if (nearestItem == null)
                    {
                        putGlobalVariablesBeforeSelection = true;
                        nearestItem = globalVariableList.FindNearestItem(SearchDirectionHint.Right, locationInControl);
                    }
                }
                if (nearestItem != null)
                {
                    string nearestGlobalVariable = GetGlobalVariable(nearestItem).Name;
                    _currentFolder.GlobalVariables = MoveGlobalVariablesIntoNewPositionInFolder(nearestGlobalVariable, putGlobalVariablesBeforeSelection, dragged);
                    RefreshGlobalVariableDisplay();
                }
            }
        }

        /// <summary>
        /// Moves the set of supplied globalVariables to be before (or after) the specified
        /// globalVariable in the folder. This takes account of the fact that NearestGlobalVariable
        /// could be one of the ones being moved.
        /// </summary>
        private List<GlobalVariable> MoveGlobalVariablesIntoNewPositionInFolder(string nearestGlobalVariable, bool putGlobalVariablesBeforeSelection, GlobalVariableManagerDragDropData dragged)
        {
            List<GlobalVariable> newFolderContents = new List<GlobalVariable>();
            foreach (GlobalVariable globalVariable in _currentFolder.GlobalVariables)
            {
                bool addThisGlobalVariableToNewList = true;

                if (globalVariable.Name == nearestGlobalVariable)
                {
                    if (!putGlobalVariablesBeforeSelection)
                    {
                        newFolderContents.Add(globalVariable);
                        addThisGlobalVariableToNewList = false;
                    }
                    foreach (GlobalVariable draggedGlobalVariable in dragged.GlobalVariables)
                    {
                        if ((draggedGlobalVariable.Name != globalVariable.Name) || (putGlobalVariablesBeforeSelection))
                        {
                            newFolderContents.Add(draggedGlobalVariable);
                        }
                    }
                }
                foreach (GlobalVariable draggedGlobalVariable in dragged.GlobalVariables)
                {
                    if (globalVariable.Name == draggedGlobalVariable.Name)
                    {
                        addThisGlobalVariableToNewList = false;
                        break;
                    }
                }
                if (addThisGlobalVariableToNewList)
                {
                    newFolderContents.Add(globalVariable);
                }
            }
            return newFolderContents;
        }

        private void globalVariableList_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(GlobalVariableManagerDragDropData)))
            {
                e.Effect = DragDropEffects.Move;
            }
        }

        private void RemoveGlobalVariablesFromFolder(GlobalVariableFolder folder, List<GlobalVariable> globalVariablesToRemove)
        {
            foreach (GlobalVariable draggedGlobalVariable in globalVariablesToRemove)
            {
                folder.GlobalVariables.Remove(draggedGlobalVariable);
            }
        }

        private TreeNode GetMouseOverTreeNode(int screenX, int screenY)
        {
            Point locationInControl = folderList.PointToClient(new Point(screenX, screenY));

            return folderList.HitTest(locationInControl).Node;
        }

        private GlobalVariableFolder GetMouseOverFolder(int screenX, int screenY)
        {
            TreeNode draggedIntoFolder = GetMouseOverTreeNode(screenX, screenY);
            if (draggedIntoFolder == null)
            {
                return null;
            }
            return _nodeFolderMapping[draggedIntoFolder];
        }


        #region Folder Drag n Drop

        private void folderList_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(GlobalVariableManagerDragDropData)))
            {
                TreeNode target = GetMouseOverTreeNode(e.X, e.Y);
                if (target != null)
                {
                    e.Effect = DragDropEffects.Move;
                }
                else
                {
                    e.Effect = DragDropEffects.None;
                }
            }
        }

        private void folderList_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(GlobalVariableManagerDragDropData)))
            {
                GlobalVariableFolder draggedInto = GetMouseOverFolder(e.X, e.Y);
                GlobalVariableManagerDragDropData dragged = (GlobalVariableManagerDragDropData)e.Data.GetData(typeof(GlobalVariableManagerDragDropData));
                RemoveGlobalVariablesFromFolder(_currentFolder, dragged.GlobalVariables);
                foreach (GlobalVariable draggedGlobalVariable in dragged.GlobalVariables)
                {
                    draggedInto.GlobalVariables.Add(draggedGlobalVariable);
                }
                RefreshGlobalVariableDisplay();
            }
        }

        private void FolderList_ItemTryDrag(object sender, TreeItemTryDragEventArgs e)
        {
            e.AllowedEffect = DragDropEffects.Move;
        }

        private void FolderList_ItemDragOver(object sender, TreeItemDragEventArgs e)
        {
            if ((e.DragItem != null) && (e.DragItem != _rootTreeNode) && (e.DropTarget != null)
                && (!e.DropTarget.IsDescendantOf(e.DragItem)))
            {
                e.Effect = DragDropEffects.Move;
                e.ShowLine &= e.DropTarget != _rootTreeNode;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void FolderList_ItemDragDrop(object sender, TreeItemDragEventArgs e)
        {
            if (e.DropTarget == null)
                return; // can't drop into nowhere
            if (e.DropTarget.IsDescendantOf(e.DragItem))
                return; // can't drop into itself or its own descendants

            TargetDropZone zone = (e.DropTarget != _rootTreeNode) ? e.DropZone : TargetDropZone.Middle;

            switch (e.DropZone)
            {
                case TargetDropZone.Top:
                    MoveFolderBeforeFolder(e.DragItem, e.DropTarget);
                    break;
                case TargetDropZone.Bottom:
                    MoveFolderAfterFolder(e.DragItem, e.DropTarget);
                    break;
                default:
                    MoveFolderToFolder(e.DragItem, e.DropTarget);
                    break;
            }
        }

        private void MoveFolderBeforeFolder(TreeNode folder, TreeNode beforeFolder)
        {
            GlobalVariableFolder movedGlobalVariableFolder = _folders[folder.Name];
            GlobalVariableFolder beforeGlobalVariableFolder = _folders[beforeFolder.Name];
            GlobalVariableFolder sourceParent = _folders[folder.Parent.Name];
            GlobalVariableFolder destParent = _folders[beforeFolder.Parent.Name];
            sourceParent.SubFolders.Remove(movedGlobalVariableFolder);
            destParent.SubFolders.Insert(destParent.SubFolders.IndexOf(beforeGlobalVariableFolder), movedGlobalVariableFolder);

            folder.Remove();
            beforeFolder.Parent.Nodes.Insert(beforeFolder.Index, folder);
            folderList.SelectedNode = folder;
        }

        private void MoveFolderAfterFolder(TreeNode folder, TreeNode afterFolder)
        {
            GlobalVariableFolder movedGlobalVariableFolder = _folders[folder.Name];
            GlobalVariableFolder afterGlobalVariableFolder = _folders[afterFolder.Name];
            GlobalVariableFolder sourceParent = _folders[folder.Parent.Name];
            GlobalVariableFolder destParent = _folders[afterFolder.Parent.Name];
            sourceParent.SubFolders.Remove(movedGlobalVariableFolder);
            destParent.SubFolders.Insert(destParent.SubFolders.IndexOf(afterGlobalVariableFolder) + 1, movedGlobalVariableFolder);

            folder.Remove();
            afterFolder.Parent.Nodes.Insert(afterFolder.Index + 1, folder);
            folderList.SelectedNode = folder;
        }

        private void MoveFolderToFolder(TreeNode folder, TreeNode intoFolder)
        {
            if (folder.Parent == intoFolder)
                return; // already there

            GlobalVariableFolder movedGlobalVariableFolder = _folders[folder.Name];
            GlobalVariableFolder sourceParent = _folders[folder.Parent.Name];
            GlobalVariableFolder destParent = _folders[intoFolder.Name];
            sourceParent.SubFolders.Remove(movedGlobalVariableFolder);
            destParent.SubFolders.Add(movedGlobalVariableFolder);

            folder.Remove();
            intoFolder.Nodes.Add(folder);
            folderList.SelectedNode = folder;
        }

        #endregion // Folder Drag n Drop

        public void SelectGlobalVariable(string variableName)
        {
            ListViewItem item = globalVariableList.FindItemWithText(variableName);
            if (item == null)
            {
                Factory.GUIController.ShowMessage("Did not find " + variableName + "!", MessageBoxIcon.Exclamation);
                return;
            }
            globalVariableList.FocusedItem = item;
            globalVariableList.TopItem = item;
            item.Selected = true;
        }

        protected override string OnGetHelpKeyword()
        {
            return "Global variables";
        }
        
        protected override void OnPropertyChanged(string propertyName, object oldValue)
        {
            if (globalVariableList.SelectedItems.Count > 0)
            {
                UpdateListItemFromVariableObject(globalVariableList.SelectedItems[0]);
            }
        }

        private void PopulateScriptTypeList()
        {
            _apiTypes.Clear();
            string api = Resources.ResourceManager.GetResourceAsString("agsdefns.sh");
            var regex = new Regex(@"\b(?:(internalstring|autoptr|builtin|managed)\s+)+struct\s+(\w+)", RegexOptions.Compiled);
            for (Match m = regex.Match(api); m.Success; m = m.NextMatch())
            {
                bool is_string = false;
                bool is_managed = false;
                bool is_autoptr = false;
                foreach (Capture c in m.Groups[1].Captures)
                {
                    is_string |= c.Value == "internalstring";
                    is_managed |= c.Value == "managed";
                    is_autoptr |= c.Value == "autoptr";
                }
                _apiTypes.Add(new APITypeDef(m.Groups[2].Captures[0].Value, is_autoptr, is_managed, is_string));
            }
        }

        private ListViewItem CreateListItemFromVariable(GlobalVariable variable)
        {
            ListViewItem newItem = new ListViewItem(new string[] { string.Empty, string.Empty, string.Empty });
            FillListItemFromVariable(newItem, variable);
            return newItem;
        }

        private void FillListItemFromVariable(ListViewItem item, GlobalVariable variable)
        {
            string varType;
            switch (variable.ArrayType)
            {
                case GlobalVariableArrayType.Array: varType = $"{variable.Type}[{variable.ArraySize}]"; break;
                case GlobalVariableArrayType.DynamicArray: varType = $"{variable.Type}[]"; break;
                default: varType = variable.Type; break;
            }

            item.SubItems[0].Text = variable.Name;
            item.SubItems[1].Text = varType;
            item.SubItems[2].Text = variable.DefaultValue;
            item.Tag = variable;
        }

        private void UpdateListItemFromVariableObject(ListViewItem listItem)
        {
            GlobalVariable selectedWord = ((GlobalVariable)listItem.Tag);
            //listItem.SubItems[SUB_ITEM_INDEX_WORD_TEXT].Text = selectedWord.Name;
        }

        private void OnGlobalVariableChanged()
        {
            if (GlobalVariableChanged != null)
            {
                GlobalVariableChanged();
            }
        }
        
        private void TextParserEditor_SizeChanged(object sender, EventArgs e)
        {
            if (this.ClientSize.Height > mainFrame.Top + 40)
            {
                mainFrame.Size = new Size(mainFrame.Width, this.ClientSize.Height - mainFrame.Top - 5);
                globalVariableList.Size = new Size(globalVariableList.Width, mainFrame.ClientSize.Height - globalVariableList.Top - 10);
            }
        }

        private void ContextMenuEventHandler(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
            GlobalVariable selectedVariable = null;
            if (globalVariableList.SelectedItems.Count > 0)
            {
                selectedVariable = ((GlobalVariable)globalVariableList.SelectedItems[0].Tag);
            }
            if ((item.Name == MENU_ITEM_EDIT_WORD) && (selectedVariable != null))
            {
                EditSelectedVariable(selectedVariable);
            }
            else if ((item.Name == MENU_ITEM_FIND_WORD) && (selectedVariable != null))
            {
                FindSelectedVariable(selectedVariable);
            }
            else if (item.Name == MENU_ITEM_ADD_WORD)
            {
                GlobalVariable variable = new GlobalVariable();
                if (GlobalVariableDialog.Show(variable, _game, _apiTypes) == DialogResult.OK)
                {
                    AddNewVariableToList(variable);
                }
            }
            else if (item.Name == MENU_ITEM_DELETE_WORD)
            {
                if (Factory.GUIController.ShowQuestion("Are you sure you want to remove the variable '" + selectedVariable.Name + "'? If you are using it in your scripts, your game will no longer compile.", MessageBoxIcon.Warning) == DialogResult.Yes)
                {
                    globalVariableList.Items.RemoveAt(globalVariableList.SelectedIndices[0]);
                    _variables.Remove(selectedVariable);
                    OnGlobalVariableChanged();
                }
            }
        }

        private void EditSelectedVariable(GlobalVariable variable)
        {
            string nameWas = variable.Name;
            if (GlobalVariableDialog.Show(variable, _game, _apiTypes) == DialogResult.OK)
            {
                if (variable.Name != nameWas)
                {
                    _variables.VariableRenamed(variable, nameWas);
                }

                FillListItemFromVariable(globalVariableList.SelectedItems[0], variable);
                OnGlobalVariableChanged();
            }
        }

        private void FindSelectedVariable(GlobalVariable variable)
        {
            TextProcessing.FindAllUsages findAllUsages = new TextProcessing.FindAllUsages(null,
                null, null, AGSEditor.Instance);
            findAllUsages.Find(null, variable.Name);
        }

        private void AddNewVariableToList(GlobalVariable variable)
        {
            _variables.Add(variable);
            int addAtIndex = 0;
            if (globalVariableList.SelectedIndices.Count > 0)
            {
                addAtIndex = globalVariableList.SelectedIndices[0] + 1;
            }
            ListViewItem addedItem = globalVariableList.Items.Insert(addAtIndex, CreateListItemFromVariable(variable));
            addedItem.Selected = true;
            addedItem.EnsureVisible();
            OnGlobalVariableChanged();
        }

        private void ShowContextMenu(Point menuPosition)
        {
            EventHandler onClick = new EventHandler(ContextMenuEventHandler);
            ContextMenuStrip menu = new ContextMenuStrip();
            if (globalVariableList.SelectedItems.Count > 0)
            {
                menu.Items.Add(new ToolStripMenuItem("Edit...", null, onClick, MENU_ITEM_EDIT_WORD));
                menu.Items.Add(new ToolStripMenuItem("Delete", null, onClick, MENU_ITEM_DELETE_WORD));
                menu.Items.Add(new ToolStripMenuItem("Find All Usages", null, onClick, MENU_ITEM_FIND_WORD));
                menu.Items.Add(new ToolStripSeparator());
            }
            menu.Items.Add(new ToolStripMenuItem("Add new variable...", null, onClick, MENU_ITEM_ADD_WORD));

            menu.Show(globalVariableList, menuPosition);
        }

        private void globalVariableList_ItemActivate(object sender, EventArgs e)
        {
            if (globalVariableList.SelectedItems.Count > 0)
            {
                EditSelectedVariable((GlobalVariable)globalVariableList.SelectedItems[0].Tag);
            }
        }

        private class GlobalVariableComparer : IComparer
        {
            public int Compare(object x, object y)
            {
                return ((GlobalVariable)((ListViewItem)x).Tag).Name.CompareTo(((GlobalVariable)((ListViewItem)y).Tag).Name);
            }
        }

        private void globalVariableList_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                ShowContextMenu(e.Location);
            }
        }

        internal class GlobalVariableManagerDragDropData
        {
            public List<GlobalVariable> GlobalVariables = new List<GlobalVariable>();
        }

        private void LoadColorTheme(ColorTheme t)
        {
            t.ControlHelper(this, "global-variables-editor");
            t.GroupBoxHelper(mainFrame, "global-variables-editor/box");
            t.ListViewHelper(globalVariableList, "global-variables-editor/list");
        }

        private void GlobalVariablesEditor_Load(object sender, EventArgs e)
        {
            if (!DesignMode)
            {
                Factory.GUIController.ColorThemes.Apply(LoadColorTheme);
            }
        }
    }
}
