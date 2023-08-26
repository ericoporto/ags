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

        private string _fileName;
        private int _id;
        private string _name;
        private bool _showTextParser;
        private string _text;
        private bool _scriptChangedSinceLastCompile;
        private string _cachedConvertedScript;
        private bool _modified = false;
        private List<DialogOption> _options = new List<DialogOption>();
        private bool _isBeingSaved = false;
        private DateTime _lastSavedAt = DateTime.MinValue;

        /// <summary>
        /// Current global text encoding for scripts.
        /// TODO: store per-script, assign from the game setting when it is changed?
        /// </summary>
        public static Encoding TextEncoding = Utilities.UTF8;

        public Dialog(string fileName, string text)
        {
            _fileName = fileName;
            _text = text ?? string.Empty;
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

        [ReadOnly(true)]
        [Category("Setup")]
        [Description("File name that the script is stored in")]
        public string FileName
        {
            get { return _fileName; }
            set { _fileName = value; }
        }

        [Browsable(false)]
        public string Text
        {
            get { return _text; }
            set { _text = value ?? string.Empty; _modified = true; }
        }

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
            get { return _text; }
            set 
            {
                if (_text != value)
                {
                    _scriptChangedSinceLastCompile = true;
                }
                _text = value; 
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

        [Browsable(false)]
        public string NameForLabelEdit
        {
            get { return Path.GetFileNameWithoutExtension(_fileName); }
            set { _fileName = value + Path.GetExtension(_fileName); }
        }

        [Browsable(false)]
        public bool Modified
        {
            get { return _modified; }
            set { _modified = value; }
        }

        [Browsable(false)]
        public bool IsBeingSaved
        {
            get { return _isBeingSaved; }
        }

        [Browsable(false)]
        public DateTime LastSavedAt
        {
            get { return _lastSavedAt; }
        }

        public void SaveToDisk()
        {
            SaveToDisk(false);
        }

        public void SaveToDisk(bool force)
        {
            if (_modified || force)
            {
                _isBeingSaved = true;
                try
                {
                    byte[] bytes = TextEncoding.GetBytes(_text);
                    using (BinaryWriter binWriter = new BinaryWriter(File.Open(_fileName, FileMode.Create)))
                    {
                        binWriter.Write(bytes);
                        _lastSavedAt = DateTime.Now;
                    }
                }
                finally
                {
                    _isBeingSaved = false;
                }
                _modified = false;
            }
        }
        public void LoadFromDisk()
        {
            try
            {
                using (BinaryReader reader = new BinaryReader(File.Open(_fileName, FileMode.Open, FileAccess.Read)))
                {
                    byte[] bytes = reader.ReadBytes((int)reader.BaseStream.Length);
                    _text = TextEncoding.GetString(bytes) ?? string.Empty;
                }
            }
            catch (Exception)
            {
                // TODO: add warning? would require changes to report system
                _text = string.Empty;
            }
            _modified = false;
        }

        public Dialog(XmlNode node)
        {
            _scriptChangedSinceLastCompile = true;
            _id = Convert.ToInt32(SerializeUtils.GetElementString(node, "ID"));
            _name = SerializeUtils.GetElementString(node, "Name");
            _showTextParser = Boolean.Parse(SerializeUtils.GetElementString(node, "ShowTextParser"));
            XmlNode scriptNode = node.SelectSingleNode("Script");
            
            foreach (XmlNode child in SerializeUtils.GetChildNodes(node, "DialogOptions"))
            {
                _options.Add(new DialogOption(child));
            }

            LoadFromDisk();

            _modified = false;
        }

        public void ToXml(XmlTextWriter writer)
        {
            writer.WriteStartElement("Dialog");
            writer.WriteElementString("ID", ID.ToString());
            writer.WriteElementString("Name", _name);
            writer.WriteElementString("ShowTextParser", _showTextParser.ToString());
            writer.WriteStartElement("Script");

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
