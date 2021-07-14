using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AGS.Editor.Console
{
    public class ConsoleEventArgs : EventArgs
    {
        public ConsoleEventArgs()
        {
        }

        public ConsoleEventArgs(string content)
        {
            //  Set the content.
            Content = content;
        }

        public string Content
        {
            get;
            private set;
        }
    }
}
