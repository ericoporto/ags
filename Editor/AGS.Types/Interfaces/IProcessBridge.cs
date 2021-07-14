using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AGS.Types
{
    public delegate void ProcessEventHandler(object sender, ProcessEventArgs args);
    public interface IProcessBridge : IDisposable
    {
        Process StartProcess(string fileName, string arguments);
        void StopProcess();

        event ProcessEventHandler OnProcessOutput;
        event ProcessEventHandler OnProcessError;
        event ProcessEventHandler OnProcessInput;
        event ProcessEventHandler OnProcessExit;
        event EventHandler OnExit;
        bool IsProcessRunning
        {
            get;
        }
        Process Process
        {
            get;
        }
        string ProcessFileName
        {
            get;
        }
        string ProcessArguments
        {
            get;
        }
    }
}
