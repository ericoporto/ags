using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AGS.Controls
{

    public class PropertyGridEx : PropertyGrid
    {
        private TextBox _searchBox;
        private Timer _hideTimer;
        private object _originalSelectedObject;

        public PropertyGridEx()
        {
            InitializeSearchBox();
        }

        override public Color BackColor
        {
            get
            {
                return base.BackColor;
            }
            set
            {
                base.BackColor = value;
                _searchBox.BackColor = value;
            }
        }

        override public Color ForeColor
        {
            get
            {
                return base.ForeColor;
            }
            set
            {
                base.ForeColor = value;
                _searchBox.ForeColor = value;
            }
        }

        private void InitializeSearchBox()
        {
            _searchBox = new TextBox
            {
                Dock = DockStyle.Bottom,
            };

            if (this.Parent != null)
            {
                this.Parent.Controls.Add(_searchBox);
                this.Parent.Controls.SetChildIndex(_searchBox, 0);
            }

            _searchBox.TextChanged += SearchBox_TextChanged;
        }

        private void SearchBox_TextChanged(object sender, EventArgs e)
        {
            RefreshFilteredProperties();
        }

        private void RefreshFilteredProperties()
        {
            if (_originalSelectedObject == null)
                return;

            string filter = _searchBox.Text.ToLowerInvariant();

            if (string.IsNullOrEmpty(filter))
            {
                base.SelectedObject = _originalSelectedObject;
            }
            else
            {
                // Filter properties based on the search query
                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(_originalSelectedObject);
                var filteredProperties = properties.Cast<PropertyDescriptor>()
                                                   .Where(p => p.DisplayName.ToLowerInvariant().Contains(filter))
                                                   .ToArray();

                // Use a TypeDescriptorProvider to create a filtered wrapper object
                var wrapper = new FilteredTypeDescriptor(_originalSelectedObject, filteredProperties);
                base.SelectedObject = wrapper;
            }
        }

        public new object SelectedObject
        {
            get
            {
                return base.SelectedObject;
            }
            set
            {
                _originalSelectedObject = value;
                base.SelectedObject = value;
                RefreshFilteredProperties();
            }
        }

        protected override void OnParentChanged(EventArgs e)
        {
            base.OnParentChanged(e);
            if (_searchBox != null && this.Parent != null)
            {
                this.Parent.Controls.Add(_searchBox);
                this.Parent.Controls.SetChildIndex(_searchBox, 0);
            }
        }

        private class FilteredTypeDescriptor : ICustomTypeDescriptor
        {
            private readonly object _instance;
            private readonly PropertyDescriptorCollection _properties;

            public FilteredTypeDescriptor(object instance, PropertyDescriptor[] properties)
            {
                _instance = instance;
                _properties = new PropertyDescriptorCollection(properties);
            }

            public AttributeCollection GetAttributes() => TypeDescriptor.GetAttributes(_instance);
            public string GetClassName() => TypeDescriptor.GetClassName(_instance);
            public string GetComponentName() => TypeDescriptor.GetComponentName(_instance);
            public TypeConverter GetConverter() => TypeDescriptor.GetConverter(_instance);
            public EventDescriptor GetDefaultEvent() => TypeDescriptor.GetDefaultEvent(_instance);
            public PropertyDescriptor GetDefaultProperty() => TypeDescriptor.GetDefaultProperty(_instance);
            public object GetEditor(Type editorBaseType) => TypeDescriptor.GetEditor(_instance, editorBaseType);
            public EventDescriptorCollection GetEvents(Attribute[] attributes) => TypeDescriptor.GetEvents(_instance, attributes);
            public EventDescriptorCollection GetEvents() => TypeDescriptor.GetEvents(_instance);
            public PropertyDescriptorCollection GetProperties(Attribute[] attributes) => _properties;
            public PropertyDescriptorCollection GetProperties() => _properties;
            public object GetPropertyOwner(PropertyDescriptor pd) => _instance;
        }

    }

}
