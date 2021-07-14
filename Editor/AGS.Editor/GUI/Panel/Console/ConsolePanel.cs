using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Diagnostics;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using AGS.Editor.Console;
using AGS.Types;

namespace AGS.Editor
{
    public delegate void ConsoleEventHandler(object sender, ConsoleEventArgs args);
    public partial class ConsolePanel : DockContent
    {
        public ConsolePanel()
        {
            InitializeComponent();
            ShowDiagnostics = false;
            IsInputEnabled = true;
            //  Handle process events.
            processBridge.OnProcessOutput += processInterace_OnProcessOutput;
            processBridge.OnProcessError += processInterace_OnProcessError;
            processBridge.OnProcessInput += processInterace_OnProcessInput;
            processBridge.OnProcessExit += processInterace_OnProcessExit;
             //  Wait for key down messages on the rich text box.
            richTextConsole.KeyDown += richTextBoxConsole_KeyDown;
        }

        void processInterace_OnProcessError(object sender, ProcessEventArgs args)
        {
            WriteOutput(args.Content, Color.Red);
            FireConsoleOutputEvent(args.Content);
        }

        void processInterace_OnProcessOutput(object sender, ProcessEventArgs args)
        {
            WriteOutput(args.Content, Color.White);
            FireConsoleOutputEvent(args.Content);
        }

        void processInterace_OnProcessInput(object sender, ProcessEventArgs args)
        {
            throw new NotImplementedException();
        }

        void processInterace_OnProcessExit(object sender, ProcessEventArgs args)
        {
            if (ShowDiagnostics)
            {
                WriteOutput(Environment.NewLine + processBridge.ProcessFileName + " exited.", Color.FromArgb(255, 0, 255, 0));
            }

            if (!this.IsHandleCreated)
                return;
            //  Read only again.
            Invoke((Action)(() =>
            {
                richTextConsole.ReadOnly = true;
            }));
        }

        void richTextBoxConsole_KeyDown(object sender, KeyEventArgs e)
        {
            var isInReadOnlyZone = richTextConsole.SelectionStart < inputStart;

            //  If we're at the input point and it's backspace, bail.
            if ((richTextConsole.SelectionStart <= inputStart) && e.KeyCode == Keys.Back) e.SuppressKeyPress = true;

            //  Are we in the read-only zone?
            if (isInReadOnlyZone)
            {
                //  Allow arrows and Ctrl-C.
                if (!(e.KeyCode == Keys.Left ||
                    e.KeyCode == Keys.Right ||
                    e.KeyCode == Keys.Up ||
                    e.KeyCode == Keys.Down ||
                    (e.KeyCode == Keys.C && e.Control)))
                {
                    e.SuppressKeyPress = true;
                }
            }

            //  Write the input if we hit return and we're NOT in the read only zone.
            if (e.KeyCode == Keys.Return && !isInReadOnlyZone)
            {
                String input = null;
                if (inputStart >= 0)
                {
                    input = richTextConsole.Text.Substring(inputStart, (richTextConsole.SelectionStart) - inputStart);
                }
                if (!String.IsNullOrEmpty(input))
                {
                    WriteInput(input, Color.White, false); //  Write the input (without echoing).
                }
            }
        }

        public void WriteOutput(string output, Color color)
        {
            if (string.IsNullOrEmpty(lastInput) == false &&
                (output == lastInput || output.Replace("\r\n", "") == lastInput))
                return;

            if (!this.IsHandleCreated)
                return;

            Invoke((Action)(() =>
            {
                //  Write the output.
                richTextConsole.SelectionColor = color;
                richTextConsole.SelectedText += output;
                inputStart = richTextConsole.SelectionStart;
                richTextConsole.ScrollToCaret();
            }));
        }

        public void ClearOutput()
        {
            richTextConsole.Clear();
            inputStart = 0;
        }

        public void WriteInput(string input, Color color, bool echo)
        {
            Invoke((Action)(() =>
            {
                //  Are we echoing?
                if (echo)
                {
                    richTextConsole.SelectionColor = color;
                    richTextConsole.SelectedText += input;
                    inputStart = richTextConsole.SelectionStart;
                }

                lastInput = input;
                processBridge.WriteInput(input);
                FireConsoleInputEvent(input);
            }));
        }

        public void StartProcess(string fileName, string arguments)
        {
            //  Are we showing diagnostics?
            if (ShowDiagnostics)
            {
                WriteOutput("Preparing to run " + fileName, Color.FromArgb(255, 0, 255, 0));
                if (!string.IsNullOrEmpty(arguments))
                    WriteOutput(" with arguments " + arguments + "." + Environment.NewLine, Color.FromArgb(255, 0, 255, 0));
                else
                    WriteOutput("." + Environment.NewLine, Color.FromArgb(255, 0, 255, 0));
            }

            //  Start the process.
            processBridge.StartProcess(fileName, arguments);

            //  If we enable input, make the control not read only.
            if (IsInputEnabled)
                richTextConsole.ReadOnly = false;
        }

        public void StartProcess(ProcessStartInfo processStartInfo)
        {
            //  Are we showing diagnostics?
            if (ShowDiagnostics)
            {
                WriteOutput("Preparing to run " + processStartInfo.FileName, Color.FromArgb(255, 0, 255, 0));
                if (!string.IsNullOrEmpty(processStartInfo.Arguments))
                    WriteOutput(" with arguments " + processStartInfo.Arguments + "." + Environment.NewLine, Color.FromArgb(255, 0, 255, 0));
                else
                    WriteOutput("." + Environment.NewLine, Color.FromArgb(255, 0, 255, 0));
            }

            //  Start the process.
            processBridge.StartProcess(processStartInfo);

            //  If we enable input, make the control not read only.
            if (IsInputEnabled)
                richTextConsole.ReadOnly = false;
        }

        public void StopProcess()
        {
            //  Stop the interface.
            processBridge.StopProcess();
        }

        private void FireConsoleOutputEvent(string content)
        {
            //  Get the event.
            var theEvent = OnConsoleOutput;
            if (theEvent != null)
                theEvent(this, new ConsoleEventArgs(content));
        }

        private void FireConsoleInputEvent(string content)
        {
            //  Get the event.
            var theEvent = OnConsoleInput;
            if (theEvent != null)
                theEvent(this, new ConsoleEventArgs(content));
        }

        private readonly ProcessBridge processBridge = new ProcessBridge();

        int inputStart = -1;
        private bool isInputEnabled = true;
        private string lastInput;

        public IProcessBridge ProcessBridge
        {
            get { return processBridge; }
        }

        public event ConsoleEventHandler OnConsoleOutput;
        public event ConsoleEventHandler OnConsoleInput;

        public bool ShowDiagnostics
        {
            get;
            set;
        }

        public bool IsInputEnabled
        {
            get { return isInputEnabled; }
            set
            {
                isInputEnabled = value;
                if (IsProcessRunning)
                    richTextConsole.ReadOnly = !value;
            }
        }

        public bool IsProcessRunning
        {
            get { return processBridge.IsProcessRunning; }
        }

        public RichTextBox InternalRichTextBox
        {
            get { return richTextConsole; }
        }

    }
}
