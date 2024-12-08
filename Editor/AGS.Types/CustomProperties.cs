using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Xml;

namespace AGS.Types
{
    [Category("Custom Properties")]
    [DisplayName("Custom Properties")]
    [TypeConverter(typeof(ExpandableObjectConverter))]
    [EditorAttribute(typeof(CustomPropertiesUIEditor), typeof(System.Drawing.Design.UITypeEditor))]

    public class CustomProperties
    {
        private Dictionary<string,CustomProperty> _properties = new Dictionary<string,CustomProperty>();
        private CustomPropertyAppliesTo _appliesTo;

        static public CustomPropertySchema Schema;

        public CustomProperties(CustomPropertyAppliesTo appliesTo)
        {
            _appliesTo = appliesTo;
        }

        public Dictionary<string, CustomProperty> PropertyValues
        {
            get { return _properties; }
        }

        public CustomProperty this[string propertyName]
        {
            get { return _properties[propertyName]; }
        }

        public CustomProperties(XmlNode propertiesNode)
        {
            foreach (XmlNode child in propertiesNode.ChildNodes)
            {
                CustomProperty newProp = new CustomProperty(child);
                _properties.Add(newProp.Name, newProp);
            }
        }

        public void ToXml(XmlTextWriter writer)
        {
            writer.WriteStartElement("Properties");
            foreach (string key in _properties.Keys)
            {
                _properties[key].ToXml(writer);
            }
            writer.WriteEndElement();
        }

        public override string ToString()
        {
            return "(Properties)";
        }

        public PropertyDescriptorCollection AddCustomProperties(PropertyDescriptorCollection properties)
        {
            foreach (CustomPropertySchemaItem item in Schema.PropertyDefinitions)
            {
                if (((_appliesTo == CustomPropertyAppliesTo.Characters) && (item.AppliesToCharacters)) ||
                    ((_appliesTo == CustomPropertyAppliesTo.Hotspots) && (item.AppliesToHotspots)) ||
                    ((_appliesTo == CustomPropertyAppliesTo.InventoryItems) && (item.AppliesToInvItems)) ||
                    ((_appliesTo == CustomPropertyAppliesTo.Objects) && (item.AppliesToObjects)) ||
                    ((_appliesTo == CustomPropertyAppliesTo.Rooms) && (item.AppliesToRooms)))
                {
                    properties.Add(new CustomPropertyDescriptor(item, this));
                }
            }
            return properties;
        }
    }
}
