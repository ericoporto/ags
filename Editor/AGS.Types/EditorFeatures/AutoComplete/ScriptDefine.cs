using System;
using System.Collections.Generic;
using System.Text;

namespace AGS.Types.AutoComplete
{
    public class ScriptDefine : ScriptToken
    {
        public ScriptDefine(string name, int scriptCharacterIndex)
        {
            Name = name;
            StartsAtCharacterIndex = scriptCharacterIndex;
        }

        public string Name;
    }
}
