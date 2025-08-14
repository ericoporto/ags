using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AGS.Editor
{
    public class NodeItemInfo
    {
        public NodeItemInfo(int id, string name, string description)
        {
            ID = id;
            Name = name;
            Description = description;
        }

        public int ID;
        public string Name;
        public string Description;
    }
}
