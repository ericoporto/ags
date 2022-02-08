using System;
using System.ComponentModel;
using System.Xml;

namespace AGS.Types
{
    [DefaultProperty("VolumeReductionWhileSpeechPlaying")]
    public class BuildTargetType
    {
        [ReadOnly(true)]
        [Description("The internal ID number of this build target type")]
        [Category("Design")]
        public int TypeID { get; set; }

        [Description("The name of this build target type")]
        [Category("Design")]
        [RefreshProperties(RefreshProperties.All)]
        public string Name { get; set; }

        public BuildTargetType(int typeID, string name)
        {
            this.TypeID = typeID;
            this.Name = name;
        }

        public BuildTargetType(XmlNode node)
        {
            SerializeUtils.DeserializeFromXML(this, node);
        }

        public void ToXml(XmlTextWriter writer)
        {
            SerializeUtils.SerializeToXML(this, writer);
        }
    }
}
