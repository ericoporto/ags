using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AGS.Editor
{
    public class NodeItemInfoWithSprite : NodeItemInfo
    {
        public NodeItemInfoWithSprite(int id, string name, string description, int sprite) : base(id, name, description)
        {
            Sprite = sprite;
        }

        public int Sprite;
    }
}
