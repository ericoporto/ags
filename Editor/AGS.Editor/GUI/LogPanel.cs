using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using AGS.Editor;
using AGS.Types.Enums;

namespace AGS.Editor
{
    public partial class LogPanel : DockContent
    {
        private LogBuffer _logBuffer = new LogBuffer();
        private bool _bufferNeedsSync = false;
        public LogPanel()
        {
            InitializeComponent();
            comboBox_Game_LogLevel.SelectedIndex = 6;
            comboBox_Script_LogLevel.SelectedIndex = 6;
            _logBuffer.ValueChanged += new System.EventHandler(this.BufferChanged);
            timerLogBufferSync.Start();
        }

        delegate void SetTextCallback(string text);

        private void SetText(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.logTextBox.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.logTextBox.Text = text;
                this.logTextBox.SelectionStart = this.logTextBox.TextLength;
                this.logTextBox.ScrollToCaret();
                Show();
            }
        }

        private void BufferChanged(object sender, EventArgs e)
        {
            _bufferNeedsSync = true;
        }

        public void Clear()
        {
            _logBuffer.Clear();
        }
        public void WriteLogMessage(string message, LogGroup group, LogLevel level)
        {
            _logBuffer.Append(message, group, level);
        }

        private void comboBox_Game_LogLevel_SelectionChangeCommitted(object sender, EventArgs e)
        {
            LogLevel level = (LogLevel)comboBox_Game_LogLevel.SelectedIndex;
            _logBuffer.SetLogLevel(LogGroup.Game, level);
            _bufferNeedsSync = true;
        }

        private void timerLogBufferSync_Tick(object sender, EventArgs e)
        {
            if (!_bufferNeedsSync) return;

            SetText(_logBuffer.ToString());
            _bufferNeedsSync = false;
        }

        private void comboBox_Script_LogLevel_SelectionChangeCommitted(object sender, EventArgs e)
        {
            LogLevel level = (LogLevel)comboBox_Script_LogLevel.SelectedIndex;
            _logBuffer.SetLogLevel(LogGroup.Script, level);
            _bufferNeedsSync = true;
        }
    }
}
