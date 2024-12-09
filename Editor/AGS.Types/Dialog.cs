using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;
using System.Xml;
using AGS.Types.Interfaces;

namespace AGS.Types
{
    [DefaultProperty("Name")]
    public class Dialog : IScript, IToXml, IComparable<Dialog>
    {
        public const int MAX_OPTIONS_PER_DIALOG = 30;

        private int _id;
        private string _name;
        private bool _showTextParser;
        private string _script;
        private bool _scriptChangedSinceLastCompile;
        private string _cachedConvertedScript;
        private List<DialogOption> _options = new List<DialogOption>();
        private CustomProperties _properties = new CustomProperties();
        private DateTime _lastSavedAt = DateTime.MinValue;
        private const string DIALOG_DIR = "Dialogs";
        private const string EXT = ".asd";

        private readonly string default_text = "// Dialog script file" + Environment.NewLine +
                "@S  // Dialog startup entry point" + Environment.NewLine +
                "return" + Environment.NewLine;

        public static Encoding TextEncoding = Utilities.UTF8;

        public Dialog()
        {
            _script = default_text;
            _cachedConvertedScript = null;
            _scriptChangedSinceLastCompile = true;
        }

        [Description("The ID number of the dialog")]
        [Category("Design")]
        [ReadOnly(true)]
        public int ID
        {
            get { return _id; }
            set { _id = value; }
        }

        [Description("The script name of the dialog")]
        [Category("Design")]
        public string Name
        {
            get { return _name; }
            set { _name = Utilities.ValidateScriptName(value); }
        }

        [Browsable(false)]
        public string FileName { get { return Path.Combine(DIALOG_DIR, "Dialog" + ID + EXT); } }

        [Browsable(false)]
        public string Text { get { return _script; } }

        [Browsable(false)]
        public ScriptAutoCompleteData AutoCompleteData { get { return null; } }

        [Description("Whether to show a text box along with the options so that the user can type in custom text")]
        [Category("Appearance")]
        public bool ShowTextParser
        {
            get { return _showTextParser; }
            set { _showTextParser = value; }
        }

        [Browsable(false)]
        public string Script
        {
            get { return _script; }
            set 
            {
                if (_script != value)
                {
                    _scriptChangedSinceLastCompile = true;
                }
                _script = value; 
            }
        }

        [Browsable(false)]
        [AGSNoSerialize]
        public bool ScriptChangedSinceLastConverted
        {
            get { return _scriptChangedSinceLastCompile; }
            set { _scriptChangedSinceLastCompile = value; }
        }

        [Browsable(false)]
        public string CachedConvertedScript
        {
            get { return _cachedConvertedScript; }
            set { _cachedConvertedScript = value; }
        }

        [Browsable(false)]
        public List<DialogOption> Options
        {
            get { return _options; }
        }

        [Browsable(false)]
        public string WindowTitle
        {
            get { return string.IsNullOrEmpty(this.Name) ? ("Dialog " + this.ID) : ("Dialog: " + this.Name); }
        }

        [AGSSerializeClass()]
        [Description("Custom properties for this dialog")]
        [Category("Properties")]
        [EditorAttribute(typeof(CustomPropertiesUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public CustomProperties Properties
        {
            get { return _properties; }
            protected set { _properties = value; }
        }

        [Browsable(false)]
        public DateTime LastSavedAt
        {
            get { return _lastSavedAt; }
        }

        public void SaveToDisk()
        {
            if (!Directory.Exists(DIALOG_DIR))
            {
                Directory.CreateDirectory(DIALOG_DIR);
            }
            byte[] bytes = Dialog.TextEncoding.GetBytes(_script);
            using (BinaryWriter binWriter = new BinaryWriter(File.Open(FileName, FileMode.Create)))
            {
                binWriter.Write(bytes);
                _lastSavedAt = DateTime.Now;
            }
        }

        public void LoadFromDisk()
        {
            try
            {
                using (BinaryReader reader = new BinaryReader(File.Open(FileName, FileMode.Open, FileAccess.Read)))
                {
                    byte[] bytes = reader.ReadBytes((int)reader.BaseStream.Length);
                    _script = Dialog.TextEncoding.GetString(bytes) ?? string.Empty;
                }
            }
            catch (FileNotFoundException)
            {
                _script = default_text;
            }
        }

        public Dialog(XmlNode node)
        {
            _scriptChangedSinceLastCompile = true;
            _id = Convert.ToInt32(SerializeUtils.GetElementString(node, "ID"));
            _name = SerializeUtils.GetElementString(node, "Name");
            _showTextParser = Boolean.Parse(SerializeUtils.GetElementString(node, "ShowTextParser"));

            foreach (XmlNode child in SerializeUtils.GetChildNodes(node, "DialogOptions"))
            {
                _options.Add(new DialogOption(child));
            }

            bool dialogInFile = true;
            XmlNode scriptNode = node.SelectSingleNode("Script");
            if (scriptNode != null)
            {
                // we need to get the dialog script from CData
                _script = scriptNode.InnerText;
                if (!string.IsNullOrEmpty(_script))
                {
                    dialogInFile = false;
                    // there was some dialog we had not upgraded yet
                    // it will be cleared later when we save it
                }
            }

            if (dialogInFile)
            {
                LoadFromDisk();
            }
        }

        public void ToXml(XmlTextWriter writer)
        {
            writer.WriteStartElement("Dialog");
            writer.WriteElementString("ID", ID.ToString());
            writer.WriteElementString("Name", _name);
            writer.WriteElementString("ShowTextParser", _showTextParser.ToString());
            writer.WriteStartElement("Script");
            writer.WriteCData(string.Empty);
            writer.WriteEndElement();

            writer.WriteStartElement("DialogOptions");
            foreach (DialogOption option in _options)
            {
                option.ToXml(writer);
            }
            writer.WriteEndElement();

            writer.WriteEndElement();

            SaveToDisk();
        }


        #region IComparable<Dialog> Members

        public int CompareTo(Dialog other)
        {
            return ID.CompareTo(other.ID);
        }

        #endregion
    }
}
