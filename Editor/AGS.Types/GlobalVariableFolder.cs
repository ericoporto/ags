using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace AGS.Types
{
    public class GlobalVariableFolder : IGlobalVariableFolder
    {
        private string _name;
        private IList<GlobalVariable> _globalVariables;
        private List<IGlobalVariableFolder> _subFolders;

        public GlobalVariableFolder(string name)
        {
            _name = name;
            _globalVariables = new List<GlobalVariable>();
            _subFolders = new List<IGlobalVariableFolder>();
        }

        public string Name
        {
            get { return _name; }
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException("Name");
                }
                _name = value;
            }
        }

        public IList<IGlobalVariableFolder> SubFolders
        {
            get { return _subFolders; }
        }

        public IList<GlobalVariable> GlobalVariables
        {
            get { return _globalVariables; }
            set { _globalVariables = value; }
        }

        public int CountGlobalVariablesInAllSubFolders()
        {
            int count = GlobalVariables.Count;
            foreach (IGlobalVariableFolder folder in SubFolders)
            {
                count += folder.CountGlobalVariablesInAllSubFolders();
            }
            return count;
        }

        public IList<GlobalVariable> GetAllGlobalVariablesFromAllSubFolders()
        {
            List<GlobalVariable> globalVariables = new List<GlobalVariable>();
            globalVariables.AddRange(GlobalVariables);

            foreach (IGlobalVariableFolder folder in SubFolders)
            {
                globalVariables.AddRange(folder.GetAllGlobalVariablesFromAllSubFolders());
            }

            return globalVariables;
        }

        public GlobalVariableFolder(XmlNode node)
        {
            if (node.Name != "GlobalVariableFolder")
            {
                throw new InvalidDataException("Incorrect node passed to GlobalVariableFolder");
            }
            _name = node.Attributes["Name"].InnerText;
            _globalVariables = new List<GlobalVariable>();
            _subFolders = new List<IGlobalVariableFolder>();

            foreach (XmlNode childNode in SerializeUtils.GetChildNodes(node, "SubFolders"))
            {
                _subFolders.Add(new GlobalVariableFolder(childNode));
            }

            foreach (XmlNode childNode in SerializeUtils.GetChildNodes(node, "GlobalVariables"))
            {
                _globalVariables.Add(new GlobalVariable(childNode));
            }
        }

        public void ToXml(XmlTextWriter writer)
        {
            writer.WriteStartElement("GlobalVariableFolder");
            writer.WriteAttributeString("Name", _name);

            writer.WriteStartElement("SubFolders");
            foreach (GlobalVariableFolder folder in _subFolders)
            {
                folder.ToXml(writer);
            }
            writer.WriteEndElement();

            writer.WriteStartElement("GlobalVariables");
            foreach (GlobalVariable globalVariable in _globalVariables)
            {
                globalVariable.ToXml(writer);
            }
            writer.WriteEndElement();

            writer.WriteEndElement();
        }
    }
}
