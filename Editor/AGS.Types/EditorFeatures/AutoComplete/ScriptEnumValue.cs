using System;
using System.Collections.Generic;
using System.Text;

namespace AGS.Types.AutoComplete
{
    public class ScriptEnumValue : ScriptToken
    {
        public ScriptEnumValue(string name, string enumtype, int scriptCharacterIndex)
        {
            Name = name;
            Type = enumtype;
            StartsAtCharacterIndex = scriptCharacterIndex;
        }

        public string Name;
        public string Type;
    }
}
