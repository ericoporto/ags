using System;
using System.Collections.Generic;
using System.Xml;

namespace AGS.Types
{
    public class ActionFolders : FolderListHybrid<Action, ActionFolder>
    {
        public ActionFolders() : base(new ActionFolder()) { }

        public ActionFolders(string name) : base(new ActionFolder(name)) { }

        public ActionFolders(XmlNode node, XmlNode parentNodeForBackwardsCompatability) :
            base(new ActionFolder(node, parentNodeForBackwardsCompatability))
        { }
    }


    public class ActionFolder : BaseFolderCollection<Action, ActionFolder>
    {
        public const string MAIN_ACTION_FOLDER_NAME = "Main";

        public ActionFolder(string name) : base(name) { }

        public ActionFolder() : this("Default") { }

        public ActionFolder(XmlNode node, XmlNode parentNodeForBackwardsCompatability) :
            base(node, parentNodeForBackwardsCompatability)
        { }

        private ActionFolder(XmlNode node) : base(node) { }

        public override ActionFolder CreateChildFolder(string name)
        {
            return new ActionFolder(name);
        }

        public Action FindActionByID(int actionID, bool recursive)
        {
            return FindItem(IsItem, actionID, recursive);
        }

        protected override ActionFolder CreateFolder(XmlNode node)
        {
            return new ActionFolder(node);
        }

        protected override Action CreateItem(XmlNode node)
        {
            return new Action(node);
        }

        private bool IsItem(Action action, int actionID)
        {
            return action.ID == actionID;
        }

    }
}
