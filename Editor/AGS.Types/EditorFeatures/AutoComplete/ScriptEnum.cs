using System;
using System.Collections.Generic;
using System.Text;

namespace AGS.Types.AutoComplete
{
    public class ScriptEnum : ScriptToken
    {
        public ScriptEnum(string name, int scriptCharacterIndex)
        {
            Name = name;
            EnumValues = new List<ScriptEnumValue>();
            StartsAtCharacterIndex = scriptCharacterIndex;
        }

        public string Name;
        public List<ScriptEnumValue> EnumValues;
    }
}
