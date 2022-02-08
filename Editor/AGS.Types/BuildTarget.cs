using System;
using System.ComponentModel;
using System.IO;
using System.Xml;

namespace AGS.Types
{
    public class BuildTarget : IToXml, IComparable<BuildTarget>
    {
        private int _id;
        private string _name;
        // FixedID is a clip's UID which never change, even if the list got reordered
        private int _fixedID;
        private int _typeID;
        private BuildTargetPlatformType _platform;
        private InheritableBool _chosen = InheritableBool.Inherit;
        private InheritableBool _actualChosen = InheritableBool.Inherit;

        // The value of a "no sound reference"
        public const int FixedIndexNoValue = 0;
        // The value of a "first index"
        public const int FixedIndexBase = 1;

        public BuildTarget(string name)
        {
            _name = name;
        }

        [DisplayName("Name")]
        [Description("The Name number of the build target")]
        [ReadOnly(true)]
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [DisplayName("ID")]
        [Description("The ID number of the clip")]
        [ReadOnly(true)]
        public int ID
        {
            get { return _id; }
            set { _id = value; }
        }

        // This is a "Fixed Index" that is used as a stable reference the clip,
        // regardless of any clip list rearrangements.
        [Browsable(false)]
        public int Index
        {
            get { return _fixedID; }
            set { _fixedID = value; }
        }

        [Description("Which type of audio does this clip contain")]
      //  [TypeConverter(typeof(BuildTargetTypeTypeConverter))]
        public int Type
        {
            get { return _typeID; }
            set { _typeID = value; }
        }

        [Description("Whether this target will build by default.")]
        public InheritableBool DefaultChosen
        {
            get { return _chosen; }
            set { _chosen = value; }
        }


        [Description("Platform it targets")]
        public BuildTargetPlatformType Platform
        {
            get { return _platform; }
            set { _platform = value; }
        }

        [AGSNoSerialize]
        [Browsable(false)]
        public InheritableBool ActualChosen
        {
            get { return _actualChosen; }
            set { _actualChosen = value; }
        }

        public BuildTarget(XmlNode node)
        {
            SerializeUtils.DeserializeFromXML(this, node);
        }

        public void ToXml(XmlTextWriter writer)
        {
            SerializeUtils.SerializeToXML(this, writer, false);
            writer.WriteEndElement();
        }

        #region IComparable<BuildTarget> Members

        public int CompareTo(BuildTarget other)
        {
            return Name.CompareTo(other.Name);
        }

        #endregion
    }
}
