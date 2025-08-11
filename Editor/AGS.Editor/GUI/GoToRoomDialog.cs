using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AGS.Editor
{
    public partial class GoToRoomDialog : Form
    {
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

        private void SetLabelText()
        {
            lblRoomNumber.Text = String.Format("Room Number ({0} - {1}):", Minimum, Maximum);
        }
    }
}
