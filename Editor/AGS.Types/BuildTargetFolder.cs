using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Xml;

namespace AGS.Types
{
    public class BuildTargetFolders : FolderListHybrid<BuildTarget, BuildTargetFolder>
    {
        public BuildTargetFolders() : base(new BuildTargetFolder()) { }

        public BuildTargetFolders(string name) : base(new BuildTargetFolder(name)) { }

        public BuildTargetFolders(XmlNode node) :
            base(new BuildTargetFolder(node))
        { }
    }

    public class BuildTargetFolder : BaseFolderCollection<BuildTarget, BuildTargetFolder>
    {
        private InheritableBool _chosen;

        public BuildTargetFolder(string name)
            : base(name)
        {
        }

        public BuildTargetFolder() : this("Default") { }

        public BuildTargetFolder(XmlNode node) : base(node)
        {
            SerializeUtils.DeserializeFromXML(this, node.SelectSingleNode(this.GetType().Name));
        }

        [Description("If the targets should build when Create Executables is ran.")]
        public InheritableBool DefaultChosen
        {
            get { return _chosen; }
            set { _chosen = value; }
        }

        public IList<BuildTarget> GetAllBuildTargetsFromAllSubFolders()
        {
            List<BuildTarget> allClips = new List<BuildTarget>();

            this.AddAllClipsFromThisAndSubFoldersToList(allClips);

            return allClips;
        }

        public override BuildTargetFolder CreateChildFolder(string name)
        {
            BuildTargetFolder childFolder = new BuildTargetFolder(name);
            childFolder.DefaultChosen = DefaultChosen;
            return childFolder;
        }

        private void AddAllClipsFromThisAndSubFoldersToList(List<BuildTarget> clipList)
        {
            clipList.AddRange(this.Items);

            foreach (BuildTargetFolder subFolder in this.SubFolders)
            {
                subFolder.AddAllClipsFromThisAndSubFoldersToList(clipList);
            }
        }

        protected override void ToXmlExtend(XmlTextWriter writer)
        {
            SerializeUtils.SerializeToXML(this, writer);
        }

        protected override BuildTargetFolder CreateFolder(XmlNode node)
        {
            return new BuildTargetFolder(node);
        }

        protected override BuildTarget CreateItem(XmlNode node)
        {
            return new BuildTarget(node);
        }
    }
}
