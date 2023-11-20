using AGS.Types;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;


namespace AGS.Editor
{
    class ThingsEditorFilter<T> : IRoomEditorFilter where T : class
    {
        protected readonly GUIController.PropertyObjectChangedHandler _propertyObjectChangedDelegate;
        protected readonly Panel _panel;
        protected readonly RoomSettingsEditor _editor;
        protected readonly Room _room;
        protected readonly Game _game;

        protected bool _isOn = false;
        protected T _selectedObject = null;
        protected bool _movingObjectWithMouse = false;
        protected int _mouseOffsetX, _mouseOffsetY;
        protected bool _movingObjectWithKeyboard = false;
        protected int _movingKeysDown = 0;
        protected Timer _movingHintTimer = new Timer();

        protected SortedDictionary<string, T> RoomItemRefs { get; private set; }
        protected SortedDictionary<string, DesignTimeProperties> DesignItems { get; private set; }

        public event EventHandler OnItemsChanged;
        public event EventHandler<SelectedRoomItemEventArgs> OnSelectedItemChanged;
        public event EventHandler<RoomFilterContextMenuArgs> OnContextMenu;

        public ThingsEditorFilter(Panel displayPanel, RoomSettingsEditor editor, Room room, Game game)
        {
            _room = room;
            _panel = displayPanel;
            _game = game;
            _editor = editor;
            _propertyObjectChangedDelegate = new GUIController.PropertyObjectChangedHandler(GUIController_OnPropertyObjectChanged);

            RoomItemRefs = new SortedDictionary<string, T>();
            DesignItems = new SortedDictionary<string, DesignTimeProperties>();
            InitGameEntities();

            _movingHintTimer.Interval = 2000;
            _movingHintTimer.Tick += MovingHintTimer_Tick;
        }

        protected virtual void InitGameEntities()
        {
            RoomItemRefs = InitItemRefs();
            DesignItems.Clear();
            foreach (var item in RoomItemRefs)
                DesignItems.Add(item.Key, new DesignTimeProperties());
        }

        protected virtual void SetPropertyGridList()
        {
            Dictionary<string, object> list = new Dictionary<string, object>();
            list.Add(_room.PropertyGridTitle, _room);
            foreach (var item in RoomItemRefs)
            {
                list.Add(GetItemName(item.Key), item.Value);
            }
            Factory.GUIController.SetPropertyGridObjectList(list, _editor.ContentDocument, _room);
        }

        protected virtual void SetPropertyGridObject(object obj)
        {
            Factory.GUIController.SetPropertyGridObject(obj, _editor.ContentDocument);
        }

        protected virtual void GUIController_OnPropertyObjectChanged(object newPropertyObject)
        {
            if (newPropertyObject is T)
            {
                T obj = (T)newPropertyObject;
                SetSelectedObject(obj);
                _panel.Invalidate();
            }
            else if (newPropertyObject is Room)
            {
                _selectedObject = null;
                _panel.Invalidate();
            }
        }

        protected virtual void SetSelectedObject(T obj)
        {
            _selectedObject = obj;
            if (OnSelectedItemChanged != null)
            {
                OnSelectedItemChanged(this, new SelectedRoomItemEventArgs(GetItemName(GetItemID(obj))));
            }
            ClearMovingState();
        }

        protected virtual void ClearMovingState()
        {
            _movingObjectWithMouse = false;
            _movingObjectWithKeyboard = false;
            _movingKeysDown = 0;
            _movingHintTimer.Stop();
        }

        protected virtual void MovingHintTimer_Tick(object sender, EventArgs e)
        {
            ClearMovingState();
            Invalidate();
        }

        public string Name
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public string DisplayName
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public RoomAreaMaskType MaskToDraw
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public int ItemCount
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public int SelectedArea
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public string HelpKeyword
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public bool ShowTransparencySlider
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public bool SupportVisibleItems
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public bool Modified
        {
            get
            {
                throw new NotImplementedException();
            }

            set
            {
                throw new NotImplementedException();
            }
        }

        public bool Enabled
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public bool Visible
        {
            get
            {
                throw new NotImplementedException();
            }

            set
            {
                throw new NotImplementedException();
            }
        }

        public bool Locked
        {
            get
            {
                throw new NotImplementedException();
            }

            set
            {
                throw new NotImplementedException();
            }
        }

        SortedDictionary<string, DesignTimeProperties> IRoomEditorFilter.DesignItems
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        protected virtual SortedDictionary<string, T> InitItemRefs()
        {
            return new SortedDictionary<string, T>();
        }

        protected virtual string GetItemName(string id)
        {
            return null;
        }

        protected virtual void UpdateObjectRef(T obj, string oldID)
        {
        }

        protected virtual string GetItemID(T obj)
        {
            return null;
        }

        protected virtual void AddObjectRef(T obj)
        {
        }

        protected virtual void RemoveObjectRef(T obj)
        {
        }

        protected virtual void MoveObject(int newX, int newY)
        {
        }

        public virtual bool MouseDown(MouseEventArgs e, RoomEditorState state)
        {
            return false;
        }

        public virtual bool MouseMove(int x, int y, RoomEditorState state)
        {
            return false;
        }

        public virtual bool MouseUp(MouseEventArgs e, RoomEditorState state)
        {
            return false;
        }

        public virtual void PaintToHDC(IntPtr hdc, RoomEditorState state)
        {
        }

        public virtual void Paint(Graphics graphics, RoomEditorState state)
        {
        }

        public virtual Cursor GetCursor(int x, int y, RoomEditorState state)
        {
            return null;
        }

        public virtual bool AllowClicksInterception()
        {
            return false;
        }

        public virtual void Invalidate()
        {
        }

        public virtual bool KeyPressed(Keys key)
        {
            return false;
        }

        public virtual bool KeyReleased(Keys key)
        {
            return false;
        }

        public virtual void Dispose()
        {
        }

        public virtual bool DoubleClick(RoomEditorState state)
        {
            return false;
        }

        public virtual void CommandClick(string command)
        {
        }

        public void FilterOn()
        {
            throw new NotImplementedException();
        }

        public void FilterOff()
        {
            throw new NotImplementedException();
        }

        string IRoomEditorFilter.GetItemName(string id)
        {
            throw new NotImplementedException();
        }

        public void SelectItem(string id)
        {
            throw new NotImplementedException();
        }
    }
}
