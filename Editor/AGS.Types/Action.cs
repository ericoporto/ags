using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace AGS.Types
{
    public class Action : IToXml, IComparable<Action>
    {
        private string _name;
        private int _id;
        private string _triggerEventHandler = string.Empty;

        public Action()
        {
            _name = string.Empty;
        }

        [Description("Script function to run when the action is triggered")]
        [Category("Events")]
        [Browsable(false)]
        [AGSEventProperty()]
        [ScriptFunctionParameters("Action *action, bool state")]
        [EditorAttribute(typeof(ScriptFunctionUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public string OnTrigger
        {
            get { return _triggerEventHandler; }
            set { _triggerEventHandler = value; }
        }

        [Description("The ID number of the action")]
        [Category("Design")]
        [ReadOnly(true)]
        public int ID
        {
            get { return _id; }
            set { _id = value; }
        }

        [Description("The script name of the action")]
        [Category("Design")]
        public string Name
        {
            get { return _name; }
            set { _name = Utilities.ValidateScriptName(value); }
        }

        #region IComparable
        public int CompareTo(Action other)
        {
            return ID.CompareTo(other.ID);
        }
        #endregion IComparable

        public Action(XmlNode node)
        {
            SerializeUtils.DeserializeFromXML(this, node);
        }

        #region IToXml
        public void ToXml(XmlTextWriter writer)
        {
            throw new NotImplementedException();
        }
        #endregion IToXml
    }
}
