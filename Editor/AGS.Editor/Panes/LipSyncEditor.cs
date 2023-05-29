using AGS.Types;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace AGS.Editor
{
    public partial class LipSyncEditor : EditorContentPanel
    {
        private const int TEXT_BOX_START_X = 10;
        private const int TEXT_BOX_START_Y = 10;
        private LipSync _lipSync;

        public LipSyncEditor(LipSync lipSync)
        {
            InitializeComponent();
            _lipSync = lipSync;
            this.AutoScroll = true;
            panelLipSyncList.AutoScroll = true;

            int x = TEXT_BOX_START_X, y = TEXT_BOX_START_Y;

            for (int i = 0; i < _lipSync.CharactersPerFrame.Length; i++)
            {
                Label label = new Label();
                label.Left = x;
                label.Top = y + 2;
                label.AutoSize = true;
                label.Text = i.ToString();
                panelLipSyncList.Controls.Add(label);

                int height_text = label.Height + label.Margin.Top + label.Margin.Bottom + 2;

                TextBox textBox = new TextBox();
                textBox.Left = x + 40;
                textBox.Top = y;
                textBox.Size = new Size(150, height_text);
                textBox.Tag = i;
                textBox.Text = _lipSync.CharactersPerFrame[i];
                textBox.TextChanged += new EventHandler(textBox_TextChanged);

                panelLipSyncList.Controls.Add(textBox);
                y += height_text + 8;
                if (i % 10 == 9)
                {
                    x += 220;
                    y = TEXT_BOX_START_Y;
                }
            }
            UpdateControlsEnabled();
        }

        public LipSync EditingLipSync
        {
            get { return _lipSync; }
        }

        protected override string OnGetHelpKeyword()
        {
            return "Lip sync";
        }

        protected override void OnPropertyChanged(string propertyName, object oldValue)
        {
            UpdateControlsEnabled();
        }

        private void textBox_TextChanged(object sender, EventArgs e)
        {
            int frameIndex = (int)((TextBox)sender).Tag;
            _lipSync.CharactersPerFrame[frameIndex] = ((TextBox)sender).Text;
        }

        private void UpdateControlsEnabled()
        {
            bool shouldBeEnabled = true;
            if (_lipSync.Type == LipSyncType.None)
            {
                shouldBeEnabled = false;
            }
            foreach (Control control in this.Controls)
            {
                if (control is TextBox)
                {
                    control.Enabled = shouldBeEnabled;
                }
            }
        }

        public void LoadColorTheme(ColorTheme t)
        {
            t.ControlHelper(this, "lip-sync-editor");

            foreach (Control control in Controls)
            {
                TextBox textBox = control as TextBox;

                if (textBox != null)
                {
                    t.TextBoxHelper(textBox, "lip-sync-editor/text-boxes");
                }
            }
        }

        private void LipSyncEditor_Load(object sender, EventArgs e)
        {
            if (!DesignMode)
            {
                Factory.GUIController.ColorThemes.Apply(LoadColorTheme);
            }
        }

        private void flowLayoutPanel1_Layout(object sender, LayoutEventArgs e)
        {
            labelLipSyncIntro1.MaximumSize = new Size((sender as Control).ClientSize.Width - labelLipSyncIntro1.Left, 64);
            labelLipSyncIntro2.MaximumSize = new Size((sender as Control).ClientSize.Width - labelLipSyncIntro2.Left, 64);
        }
    }
}
