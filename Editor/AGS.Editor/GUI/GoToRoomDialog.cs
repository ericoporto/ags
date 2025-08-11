using AGS.Editor.Components;
using AGS.Types;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AGS.Editor
{
    public partial class GoToRoomDialog : Form
    {
        private List<IRoom> _rooms;

        public GoToRoomDialog()
        {
            InitializeComponent();
            (this.upDownRoomNumber.Controls[1] as TextBox).Enter += upDownLineNumber_Controls1_Enter;
        }

        private delegate void Action();

        private void upDownLineNumber_Controls1_Enter(object sender, EventArgs e)
        {
            BeginInvoke((Action)(() =>
            {
                (this.upDownRoomNumber.Controls[1] as TextBox).SelectAll();
            }));
        }

        public int RoomNumber
        {
            get { return (int)upDownRoomNumber.Value; }
            set { upDownRoomNumber.Value = value; }
        }

        public int Minimum
        {
            get { return (int)upDownRoomNumber.Minimum; }
            set 
            { 
                upDownRoomNumber.Minimum = value;
                SetLabelText();
            }
        }

        public int Maximum
        {
            get { return (int)upDownRoomNumber.Maximum; }
            set
            {
                upDownRoomNumber.Maximum = value;
                SetLabelText();
            }
        }

        public List<IRoom> Rooms
        {
            get { return _rooms; }
            set 
            {  
                _rooms = value; 
                lstRooms.Items.Clear();
                foreach (IRoom room in _rooms)
                {
                    lstRooms.Items.Add(room);
                }
            }
        }

        private void SetLabelText()
        {
            lblRoomNumber.Text = String.Format("Room Number ({0} - {1}):", Minimum, Maximum);
        }

        private IRoom GetRoomByNumber(int roomNumber)
        {
            return RoomsComponent.GetRoomByNumber(roomNumber);
        }

        private bool ExistRoom (int roomNumber)
        {
            return GetRoomByNumber(roomNumber) != null;
        }

        private void syncFromUpDownToListBox()
        {
            if (ExistRoom(RoomNumber))
            {
                btnOk.Enabled = true;
                IRoom selectedRoom = GetRoomByNumber(RoomNumber);
                if (selectedRoom != lstRooms.SelectedItem)
                {
                    lstRooms.SelectedItem = selectedRoom;
                }
            }
            else
            {
                btnOk.Enabled = false;
                lstRooms.SelectedItem = null;
                lstRooms.Invalidate();
            }
        }

        private void syncFromListBoxToUpDown()
        {
            if (lstRooms.SelectedItem is IRoom)
            {
                IRoom room = (IRoom)lstRooms.SelectedItem;
                if (upDownRoomNumber.Value != room.Number)
                {
                    upDownRoomNumber.Value = room.Number;
                }
            }
        }

        private void upDownRoomNumber_ValueChanged(object sender, EventArgs e)
        {
            syncFromUpDownToListBox();
        }

        private void upDownRoomNumber_KeyUp(object sender, KeyEventArgs e)
        {
            syncFromUpDownToListBox();
        }

        private void lstRooms_SelectedValueChanged(object sender, EventArgs e)
        {
            syncFromListBoxToUpDown();
        }

        private void lstRooms_SelectedIndexChanged(object sender, EventArgs e)
        {
            syncFromListBoxToUpDown();
        }

        private void lstRooms_Format(object sender, ListControlConvertEventArgs e)
        {
            if (e.ListItem is IRoom)
            {
                IRoom room = (IRoom)e.ListItem;
                e.Value = string.Format("{0}: {1}", room.Number, room.Description);
            }
            else
            {
                e.Value = "Unknown room";
            }
        }

    }
}
