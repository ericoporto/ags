using AGS.CScript.Compiler;
using AGS.Types;
using AGS.Types.AutoComplete;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace AGS.Editor
{
    public class AutoComplete
    {
        private const string AUTO_COMPLETE_IGNORE = "$AUTOCOMPLETEIGNORE$";
        private const string AUTO_COMPLETE_STATIC_ONLY = "$AUTOCOMPLETESTATICONLY$";
        private const string AUTO_COMPLETE_NO_INHERIT = "$AUTOCOMPLETENOINHERIT$";
        private static Script _scriptToUpdateInBackground = null;
        private static List<Script> _importedScripts = null;
        private static object _scriptLockObject = new object();

        public delegate void BackgroundCacheUpdateStatusChangedHandler(BackgroundAutoCompleteStatus status, Exception errorDetails);
        public static event BackgroundCacheUpdateStatusChangedHandler BackgroundCacheUpdateStatusChanged;

        static AutoComplete()
        {
            Thread thread = new Thread(new ThreadStart(UpdateAutocompleteCacheThread));
            thread.IsBackground = true;
            thread.Name = "AutoCompleteUpdateThread";
            thread.Priority = ThreadPriority.Lowest;
            thread.Start();
        }

        public static void RequestBackgroundCacheUpdate(Script scriptToUpdate, List<Script> importedScripts)
        {
            lock (_scriptLockObject)
            {
                _scriptToUpdateInBackground = scriptToUpdate;
                _importedScripts = importedScripts;
            }
        }

        private static void OnBackgroundCacheUpdateStatusChanged(BackgroundAutoCompleteStatus status, Exception errorDetails)
        {
            if (BackgroundCacheUpdateStatusChanged != null)
            {
                BackgroundCacheUpdateStatusChanged(status, errorDetails);
            }
        }

        private static void UpdateAutocompleteCacheThread()
        {
            while (true)
            {
                Thread.Sleep(50);
                if (_scriptToUpdateInBackground != null)
                {
                    Script scriptToUpdate;
                    List<Script> importedScripts;
                    lock (_scriptLockObject)
                    {
                        scriptToUpdate = _scriptToUpdateInBackground;
                        importedScripts = _importedScripts;
                        _scriptToUpdateInBackground = null;
                        _importedScripts = null;
                    }
                    try
                    {
                        OnBackgroundCacheUpdateStatusChanged(BackgroundAutoCompleteStatus.Processing, null);
                        ConstructCache(scriptToUpdate, importedScripts);
                        OnBackgroundCacheUpdateStatusChanged(BackgroundAutoCompleteStatus.Finished, null);
                    }
                    catch (Exception ex)
                    {
                        OnBackgroundCacheUpdateStatusChanged(BackgroundAutoCompleteStatus.Error, ex);
                    }
                }
            }
        }

        private static bool IncrementIndexToSkipAnyComments(FastString script, ref int index)
        {
            while (index < script.Length - 1 && (script[index] == '/'))
            {
                if ((script[index + 1] == '/'))
                {
                    index = IndexOfLineEnd(script, index + 2).Item2;
                    if (index < 0)
                    {
                        index = script.Length;
                    }
                }
                else
                {
                    if ((script[index + 1] == '*'))
                    {
                        index = script.IndexOf("*/", index + 2);
                        if (index < 0)
                        {
                            index = script.Length;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            return index == script.Length;
        }

        private static void SkipUntilMatchingClosingBrace(ref FastString script)
        {
            int braceIndent = 1, index = 1;
            while ((braceIndent > 0) && (index < script.Length))
            {
                if (IncrementIndexToSkipAnyComments(script, ref index))
                {
                    break;
                }

                if ((script[index] == '"') || (script[index] == '\''))
                {
                    char firstChar = script[index];
                    index++;
                    while (index < script.Length - 1)
                    {
                        if ((script[index] == firstChar) && 
                            ((script[index - 1] != '\\') || (script[index - 2] == '\\')))
                        {
                            break;
                        }
                        index++;
                    }
                }
                if (script[index] == '{')
                {
                    braceIndent++;
                }
                else if (script[index] == '}')
                {
                    braceIndent--;
                }
                index++;
            }
            script = script.Substring(index);
        }

        public static void ConstructCache(Script scriptToCache, IEnumerable<Script> importScripts)
        {
            string originalText = scriptToCache.Text;
            ScriptAutoCompleteData newCache = new ScriptAutoCompleteData();
            List<ScriptVariable> variables = newCache.Variables;
            List<ScriptFunction> functions = newCache.Functions;
            List<ScriptDefine> defines = newCache.Defines;
            List<ScriptEnum> enums = newCache.Enums;
            List<ScriptStruct> structs = newCache.Structs;
            variables.Clear();
            functions.Clear();
            defines.Clear();
            enums.Clear();
            structs.Clear();
            FastString script = originalText;
            AutoCompleteParserState state = new AutoCompleteParserState();
            ScriptFunction lastFunction = null;
            // Struct lookup will have both local and imported types
            List<ScriptStruct> structsLookup = new List<ScriptStruct>();
            if (importScripts != null)
            {
                foreach (var import in importScripts)
                    structsLookup.AddRange(import.AutoCompleteData.Structs);
            }

            while (script.Length > 0)
            {
                SkipWhitespace(ref script);
                state.CurrentScriptCharacterIndex = originalText.Length - script.Length;

                if (script.Length == 0)
                {
                    break;
                }
                if (script.StartsWith("//"))
                {
                    FastString scriptAtComment = script;
                    GoToNextLine(ref script);

                    if (scriptAtComment.StartsWith("///"))
                    {
                        FastString commentText = scriptAtComment.Substring(3, (scriptAtComment.Length - script.Length) - 3);
                        state.PreviousComment = commentText.ToString().Trim();
                    }
                    continue;
                }
                if (script.StartsWith("/*"))
                {
                    int endOfComment = script.IndexOf("*/");
                    if (endOfComment < 0)
                    {
                        break;
                    }
                    script = script.Substring(endOfComment + 2);
                    continue;
                }
                if (script.StartsWith("#"))
                {
                    ProcessPreProcessorDirective(defines, ref script, state);
                    continue;
                }
                if (script.StartsWith("{"))
                {
                    if (state.WordBeforeLast == "enum")
                    {
                        state.InsideEnumDefinition = new ScriptEnum(state.LastWord, state.InsideIfDefBlock, state.InsideIfNDefBlock, state.CurrentScriptCharacterIndex);
                    }
                    else if (state.WordBeforeLast == "extends")
                    {
                        // inherited struct
                        foreach (ScriptStruct baseStruct in structsLookup)
                        {
                            if (baseStruct.Name == state.LastWord)
                            {
                                state.InsideStructDefinition = CreateInheritedStruct(baseStruct, state);
                                functions = state.InsideStructDefinition.Functions;
                                variables = state.InsideStructDefinition.Variables;
                                break;
                            }
                        }
                    }
                    else if (state.WordBeforeLast == "struct")
                    {
                        state.InsideStructDefinition = new ScriptStruct(state.LastWord, state.InsideIfDefBlock, state.InsideIfNDefBlock, state.CurrentScriptCharacterIndex);
                        functions = state.InsideStructDefinition.Functions;
                        variables = state.InsideStructDefinition.Variables;
                    }
                    else
                    {
                        state.ClearPreviousWords();

                        SkipUntilMatchingClosingBrace(ref script);

                        if ((lastFunction != null) && (lastFunction.EndsAtCharacterIndex == 0))
                        {
                            lastFunction.EndsAtCharacterIndex = originalText.Length - script.Length;
                        }
                        continue;
                    }
                }

                string thisWord = GetNextWord(ref script);
                if (thisWord == "(")
                {
                    List<ScriptFunction> functionList = functions;
                    bool isStaticExtender = script.StartsWith("static ");
                    bool isExtenderMethod = isStaticExtender || script.StartsWith("this ");
                    if (isExtenderMethod)
                    {
                        ScriptStruct newStruct = null;
                        // NOTE: for extenders we DO NOT look up the global struct list;
                        // the reason is a bit complicated, but in a nutshell:
                        // we need to have a list of extender functions bound to the
                        // struct defs in the *local* script's autocomplete cache, not the
                        // imported script's cache. The struct defs may be duplicated this
                        // way, but that's mostly fine, as these may be merged together;
                        // e.g. see ScintillaWrapper.GetAllStructsWithMatchingName().
                        AdjustFunctionListForExtenderFunction(structs, ref functionList, ref newStruct, ref script);
                        if (newStruct != null)
                        {
                            structs.Add(newStruct);
                            structsLookup.Add(newStruct);
                        }
                    }
                    if (AddFunctionDeclaration(functionList, ref script, thisWord, state, isExtenderMethod, isStaticExtender, isStaticExtender))
                    {
                        lastFunction = functionList[functionList.Count - 1];
                    }
                    state.ClearPreviousWords();
                }
				else if ((thisWord == "[") && (PeekNextWord(script) == "]"))
				{
					GetNextWord(ref script);
					state.AddNextWord("[]");
				}
				else if ((thisWord == "=") || (thisWord == ";") ||
						 (thisWord == ",") || (thisWord == "["))
				{
					if (state.InsideEnumDefinition != null)
					{
						AddEnumValue(state.InsideEnumDefinition, script, state.LastWord, state);

                        if (thisWord == "=")
                        {
                            // skip whatever the value of the enum is
                            GetNextWord(ref script);
                        }
					}
					else
					{
						AddVariableDeclaration(variables, ref script, thisWord, state);
						if (thisWord == "=")
						{
							while ((thisWord != ";") && (thisWord != ",") && (script.Length > 0))
							{
								thisWord = GetNextWord(ref script);
							}
						}
						if (thisWord == ",")
						{
							// eg. "int x,y"; ensure "y" gets recorded next time round
							state.UndoLastWord();
							continue;
						}
						if (thisWord == "[")
						{
							// eg. "int a[10], b[10], c[10];"
							SkipWhitespace(ref script);
							if (script.StartsWith(","))
							{
								GetNextWord(ref script);
								state.UndoLastWord();
								continue;
							}
						}
					}
					state.ClearPreviousWords();
				}
				else if ((thisWord == "}") && (state.InsideEnumDefinition != null))
				{
                    // add the last value (unless it's an empty enum)
                    if (state.LastWord != "{")
                    {
                        AddEnumValue(state.InsideEnumDefinition, script, state.LastWord, state);
                    }
					enums.Add(state.InsideEnumDefinition);
					state.InsideEnumDefinition = null;
                    state.ClearPreviousWords();
				}
				else if ((thisWord == "}") && (state.InsideStructDefinition != null))
				{
					structs.Add(state.InsideStructDefinition);
                    structsLookup.Add(state.InsideStructDefinition);
                    // Restore struct member references to global script ones
                    functions = newCache.Functions;
                    variables = newCache.Variables;
                    state.InsideStructDefinition = null;
                    state.ClearPreviousWords();
				}
				else
				{
					state.AddNextWord(thisWord);
				}
            }

            GenerateDynamicArrayStructs(newCache, structsLookup);

            scriptToCache.AutoCompleteData.CopyFrom(newCache);
			scriptToCache.AutoCompleteData.Populated = true;
        }

        private static void GenerateDynamicArrayStructs(ScriptAutoCompleteData data, List<ScriptStruct> structsLookup)
        {
            GenerateDynamicArrayStructs(data.Variables, data.Structs, structsLookup);
        }

        private static void GenerateDynamicArrayStructs(List<ScriptVariable> variables, List<ScriptStruct> structs, List<ScriptStruct> structsLookup)
        {
            List<ScriptStruct> structsCopy = new List<ScriptStruct>(structs);

            GenerateDynamicArrayStructsIntern(variables, structs, structsLookup);
            foreach (var s in structsCopy)
                GenerateDynamicArrayStructsIntern(s.Variables, structs, structsLookup);
        }

        private static void GenerateDynamicArrayStructsForVarsOnly(List<ScriptVariable> variables, List<ScriptStruct> structs)
        {
            List<ScriptStruct> structsLookup = new List<ScriptStruct>(structs);
            GenerateDynamicArrayStructsIntern(variables, structs, structsLookup);
        }

        private static void GenerateDynamicArrayStructsIntern(List<ScriptVariable> variables, List<ScriptStruct> structs, List<ScriptStruct> structsLookup)
        {
            foreach (var variable in variables)
            {
                if (!variable.IsDynamicArray)
                    continue;

                string trueType = variable.Type;
                string fakeStructName = trueType + (variable.IsPointer ? "*[]" : "[]");
                variable.Type = fakeStructName;
                if (structsLookup.Find(s => s.Name == fakeStructName) != null)
                    continue;

                ScriptStruct dynArrStruct = new ScriptStruct(fakeStructName);
                ScriptVariable lengthVar = new ScriptVariable("Length", "int", false, false, false, null, null, false, false, false, false, true, 0);
                lengthVar.Description = "Returns length of this dynamic array.";
                dynArrStruct.Variables.Add(lengthVar);
                dynArrStruct.BaseType = trueType;
                dynArrStruct.FullDefinition = true;
                structs.Add(dynArrStruct);
                structsLookup.Add(dynArrStruct);
            }
        }

        private static void AdjustFunctionListForExtenderFunction(List<ScriptStruct> structsLookup,
            ref List<ScriptFunction> functionList, ref ScriptStruct newStruct, ref FastString script)
        {
            GetNextWord(ref script);
            string structName = GetNextWord(ref script);
            while ((script.Length > 0) && (script[0] != ',') && (script[0] != ')'))
            {
                script = script.Substring(1);
            }
            if ((script.Length > 0) && script[0] == ',')
            {
                script = script.Substring(1);
            }
            script = script.Trim();

            foreach (ScriptStruct struc in structsLookup)
            {
                if (struc.Name == structName)
                {
                    functionList = struc.Functions;
                    return;
                }
            }
            newStruct = new ScriptStruct(structName);
            functionList = newStruct.Functions;
            return;
        }

        private static ScriptStruct CreateInheritedStruct(ScriptStruct baseStruct, AutoCompleteParserState state)
        {
            ScriptStruct newStruct = new ScriptStruct(state.WordBeforeWordBeforeLast, state.InsideIfDefBlock, state.InsideIfNDefBlock, state.CurrentScriptCharacterIndex);
            foreach (ScriptFunction func in baseStruct.Functions)
            {
                if (!func.NoInherit)
                {
                    newStruct.Functions.Add(func);
                }
            }
            foreach (ScriptVariable var in baseStruct.Variables)
            {
                if (!var.NoInherit)
                {
                    newStruct.Variables.Add(var);
                }
            }
            return newStruct;
        }

        private static void ProcessPreProcessorDirective(List<ScriptDefine> defines, ref FastString script, AutoCompleteParserState state)
        {
            script = script.Substring(1);
            string preProcessorDirective = GetNextWord(ref script);
            if (preProcessorDirective == "define")
            {
                string macroName = GetNextWord(ref script);
                if (!string.IsNullOrEmpty(macroName) && (Char.IsLetter(macroName[0])) &&
                    (!DoesCurrentLineHaveToken(script, AUTO_COMPLETE_IGNORE)))
                {
                    defines.Add(new ScriptDefine(macroName, state.InsideIfDefBlock, state.InsideIfNDefBlock, state.CurrentScriptCharacterIndex));
                }
            }
            else if (preProcessorDirective == "undef")
            {
                string macroName = GetNextWord(ref script);
                if (!string.IsNullOrEmpty(macroName) && Char.IsLetter(macroName[0]))
                {
                    foreach (ScriptDefine define in defines)
                    {
                        if (define.Name == macroName)
                        {
                            defines.Remove(define);
                            break;
                        }
                    }
                }
            }
            else if (preProcessorDirective == "ifndef")
            {
                string macroName = GetNextWord(ref script);
                if (!string.IsNullOrEmpty(macroName) && Char.IsLetter(macroName[0]))
                {
                    state.InsideIfNDefBlock = macroName;
                }
            }
            else if (preProcessorDirective == "ifdef")
            {
                string macroName = GetNextWord(ref script);
                if (!string.IsNullOrEmpty(macroName) && Char.IsLetter(macroName[0]))
                {
                    state.InsideIfDefBlock = macroName;
                }
            }
            else if (preProcessorDirective == "else")
            {
                // Negate previous condition
                if (state.InsideIfDefBlock != null)
                {
                    state.InsideIfNDefBlock = state.InsideIfDefBlock;
                    state.InsideIfDefBlock = null;
                }
                else if (state.InsideIfNDefBlock != null)
                {
                    state.InsideIfDefBlock = state.InsideIfNDefBlock;
                    state.InsideIfNDefBlock = null;
                }
            }
            else if (preProcessorDirective == "endif")
            {
                state.InsideIfNDefBlock = null;
                state.InsideIfDefBlock = null;
            }
            GoToNextLine(ref script);
            state.ClearPreviousWords();
        }

        private static void AddEnumValue(ScriptEnum insideEnumDefinition, FastString script, string lastWord, AutoCompleteParserState state)
        {
            if ((lastWord.Length > 0) && (Char.IsLetter(lastWord[0])))
            {
                if (!DoesCurrentLineHaveToken(script, AUTO_COMPLETE_IGNORE))
                {
                    insideEnumDefinition.EnumValues.Add(new ScriptEnumValue(lastWord, insideEnumDefinition.Name, state.InsideIfDefBlock, state.InsideIfNDefBlock, state.CurrentScriptCharacterIndex));
                }
            }
        }

        /// <summary>
        /// Searches for any line-break (LF or CRLF), and returns a pair of character indexes:
        /// an index of line-break, and an index right after line-break.
        /// </summary>
        private static Tuple<int, int> IndexOfLineEnd(FastString script, int fromIndex = 0)
        {
            int indexofLF = script.IndexOf('\n', fromIndex);
            if (indexofLF < 0)
                return new Tuple<int, int>(-1, -1);
            if (indexofLF > 0 && script[indexofLF - 1] == '\r')
            {
                return new Tuple<int, int>(indexofLF - 1, indexofLF + 1);
            }
            else
            {
                return new Tuple<int, int>(indexofLF, indexofLF + 1);
            }
        }

        private static bool DoesCurrentLineHaveToken(FastString script, string tokenToCheckFor)
        {
            var indexOfNextLine = IndexOfLineEnd(script);
            if (indexOfNextLine.Item1 > 0)
            {
                if (script.Substring(0, indexOfNextLine.Item1).IndexOf(tokenToCheckFor) > 0)
                {
                    return true;
                }
            }
            return false;
        }

        private static bool AddFunctionDeclaration(List<ScriptFunction> functions, ref FastString script, string thisWord, AutoCompleteParserState state, bool isExtenderMethod, bool isStatic, bool isStaticOnly)
        {
            bool succeeded = false;

            if ((state.LastWord.Length > 0) && (state.WordBeforeLast.Length > 0))
            {
                if (!DoesCurrentLineHaveToken(script, AUTO_COMPLETE_IGNORE))
                {
                    string functionName = state.LastWord;
                    string type = state.WordBeforeLast;
                    bool isPointer = false, isNoInherit = false;
                    bool isProtected = false;
                    if (type == "::")
                    {
                        functionName = state.WordBeforeWordBeforeLast + "::" + functionName;
                        type = (state.PreviousWords.Length > 3) ? state.PreviousWords[3] : "unknown";
                    }
                    if (type == "*")
                    {
                        isPointer = true;
                        type = state.WordBeforeWordBeforeLast;
                    }
					if (type == "[]")
					{
						// get the type name and the []
						type = state.WordBeforeWordBeforeLast + "[]";
					}
                    if (state.IsWordInPreviousList("static"))
                    {
                        isStatic = true;
                    }
                    if (state.IsWordInPreviousList("protected"))
                    {
                        isProtected = true;
                    }
                    if (DoesCurrentLineHaveToken(script, AUTO_COMPLETE_STATIC_ONLY))
                    {
                        isStaticOnly = true;
                    }
                    if (DoesCurrentLineHaveToken(script, AUTO_COMPLETE_NO_INHERIT))
                    {
                        isNoInherit = true;
                    }

                    int parameterListEndIndex = script.IndexOf(')');
                    if (parameterListEndIndex >= 0)
                    {
                        string parameterList = script.Substring(0, parameterListEndIndex);
                        script = script.Substring(parameterListEndIndex + 1);
                        ScriptFunction newFunc = new ScriptFunction(functionName, type, parameterList, state.InsideIfDefBlock, state.InsideIfNDefBlock, isPointer, isStatic, isStaticOnly, isNoInherit, isProtected, isExtenderMethod, state.CurrentScriptCharacterIndex - 1);
                        if (!string.IsNullOrEmpty(state.PreviousComment))
                        {
                            newFunc.Description = state.PreviousComment;
                            state.PreviousComment = null;
                        }
                        functions.Add(newFunc);
                        succeeded = true;
                    }
                }
            }

            return succeeded;
        }

        private static void AddVariableDeclaration(List<ScriptVariable> variables, ref FastString script, string thisWord, AutoCompleteParserState state)
        {
            if ((state.LastWord.Length > 0) && (state.WordBeforeLast.Length > 0))
            {
                if (!DoesCurrentLineHaveToken(script, AUTO_COMPLETE_IGNORE))
                {
                    bool isAttribute = false;
                    bool isArray = false, isDynamicArray = false;
                    bool isPointer = false;
                    bool isStatic = false, isStaticOnly = false;
                    bool isNoInherit = false, isProtected = false;
                    bool isReadonly = false;
                    string type = state.WordBeforeLast;
                    int typeWordIndex = 1;
					string varName = state.LastWord;

                    isAttribute = state.IsWordInPreviousList("attribute");

                    if (thisWord == "[")
                    {
                        while ((script.Length > 0) && (GetNextWord(ref script) != "]")) ;
                        isArray = true;
                    }
					else
					{
                        // Dynarray brackets may be met in following cases:
                        // - indexed attribute (attribute type name[])
                        // - dynamic array (type name[])
                        // - attribute returning dynamic array (attribute type[] name)
                        // - indexed attribute returning dynamic array (attribute type[] name[])
                        if (varName == "[]")
                        {
                            // it's appended to the name
                            varName = state.WordBeforeLast;
                            type = state.WordBeforeWordBeforeLast;
                            typeWordIndex = 2;
                            if (isAttribute)
                            {
                                // indexed attribute
                            }
                            else
                            {
                                // regular variable of dynamic array type
                                isArray = true;
                                isDynamicArray = true;
                            }
                        }

                        if (type == "[]")
                        {
                            typeWordIndex++;
                            type = state.PreviousWords[typeWordIndex];
                            // it's appended to a type
                            if (isAttribute)
                            {
                                // attribute returning dynamic array
                                isArray = true;
                                isDynamicArray = true;
                            }
                            else
                            {
                                // bad syntax?
                            }
                        }
                    }

                    if (type == "*")
                    {
                        isPointer = true;
                        typeWordIndex++;
                        type = state.PreviousWords[typeWordIndex];
                    }
                    if (state.IsWordInPreviousList("static"))
                    {
                        isStatic = true;
                    }
                    if (state.IsWordInPreviousList("protected"))
                    {
                        isProtected = true;
                    }
                    if (state.IsWordInPreviousList("readonly"))
                    {
                        isReadonly = true;
                    }
                    if (DoesCurrentLineHaveToken(script, AUTO_COMPLETE_STATIC_ONLY))
                    {
                        isStaticOnly = true;
                    }
                    if (DoesCurrentLineHaveToken(script, AUTO_COMPLETE_NO_INHERIT))
                    {
                        isNoInherit = true;
                    }
                    // ignore "struct GUI;" prototypes
                    if (type != "struct")
                    {
                        //if (varName == "{") System.Diagnostics.Debugger.Break();
                        ScriptVariable newVar = new ScriptVariable(varName, type, isArray, isDynamicArray, isPointer, state.InsideIfDefBlock, state.InsideIfNDefBlock, isStatic, isStaticOnly, isNoInherit, isProtected, isReadonly, state.CurrentScriptCharacterIndex);

                        if (!string.IsNullOrEmpty(state.PreviousComment))
                        {
                            newVar.Description = state.PreviousComment;
                            state.PreviousComment = null;
                        }

                        variables.Add(newVar);
                    }
                }
			}
        }

        private static void GoToNextLine(ref FastString script)
        {
            var indexOfNextLine = IndexOfLineEnd(script);
            if (indexOfNextLine.Item1 < 0)
            {
                script = string.Empty;
            }
            else
            {
                script = script.Substring(indexOfNextLine.Item2);
            }
        }

        private static void SkipWhitespace(ref FastString script)
        {
			if (script.Length == 0)
			{
				return;
			}

			int index = 0;
			while ((script[index] == ' ') || (script[index] == '\t')
                || (script[index] == '\r') || (script[index] == '\n'))
            {
                index++;
                if (index >= script.Length)
                {
                    script = string.Empty;
                    return;
                }
            }
            if (index > 0)
            {
                script = script.Substring(index);
            }
        }

		private static string PeekNextWord(FastString script)
		{
			FastString tester = script;
			return GetNextWord(ref tester);
		}

        private static string GetNextWord(ref FastString script)
        {
            SkipWhitespace(ref script);
            if (script.Length == 0)
            {
                return string.Empty;
            }
            int index = 0;
            while (script[index].IsScriptWordChar())
            {
                index++;
                if (index >= script.Length)
                {
                    break;
                }
            }

            if (index == 0)
            {
                index++;
            }

            if ((script[0] == ':') && (script.Length > 1) && (script[1] == ':'))
            {
                // Make :: into one word
                index++;
            }

            string nextWord = script.Substring(0, index);
            script = script.Substring(index);
            return nextWord;
        }

        public static List<ScriptVariable> GetLocalVariableDeclarations(ScriptFunction func, List<ScriptStruct> localStructs, string scriptToParse, int relativeCharacterIndex)
        {
            List<ScriptVariable> variables = new List<ScriptVariable>();
            GetFunctionParametersAsVariableList(func, variables);
            GetLocalVariableDeclarationsFromScriptExtract(scriptToParse, relativeCharacterIndex, variables);
            GenerateDynamicArrayStructsForVarsOnly(variables, localStructs);
            return variables;
        }

        private static void GetFunctionParametersAsVariableList(ScriptFunction func, List<ScriptVariable> variables)
        {
            if (func.ParamList.Length == 0)
            {
                return;
            }
            string[] parameters = func.ParamList.Split(',');
            foreach (string thisParam in parameters)
            {
                FastString param = thisParam.Trim();
                if (param.StartsWith("optional "))
                {
                    param = param.Substring(9).Trim();
                }
                int index = param.Length - 1;

                bool isDynamicArray = false;
                if (index >= 0 && param[index] == ']')
                {
                    isDynamicArray = true;
                    while (index >= 0 && param[index--] != '[');
                    param = param.Substring(0, index + 1).Trim();
                }

                while ((index >= 0) &&
                       (Char.IsLetterOrDigit(param[index]) || param[index] == '_'))
                {
                    index--;
                }
                FastString paramName = param.Substring(index + 1);
                FastString paramType = param.Substring(0, index + 1).Trim();
                bool isPointer = false;
                // FIXME: implement FastString EndsWith
                if ((paramType.Length > 0) && paramType[paramType.Length - 1] == '*')
                {
                    isPointer = true;
                    paramType = paramType.Substring(0, paramType.Length - 1).Trim();
                }
                if ((paramName.Length > 0) && (paramType.Length > 0))
                {
                    variables.Add(new ScriptVariable(paramName.ToString(), paramType.ToString(), false, isDynamicArray, isPointer, null, null, false, false, false, false, false, func.StartsAtCharacterIndex));
                }
            }
            return;
        }

        private static void GetLocalVariableDeclarationsFromScriptExtract(string scriptToParse, int relativeCharacterIndex, List<ScriptVariable> variables)
        {
            FastString script = scriptToParse;
            string lastWord = string.Empty;
            while (script.Length > 0)
            {
                string nextWord = GetNextWord(ref script);
                if (nextWord.Length == 0)
                {
                    continue;
                }
                if ((Char.IsLetter(nextWord[0])) || (nextWord == "*") || (nextWord[0] == '_'))
                {
                    if ((lastWord.Length > 0) && (script.Length > 0))
                    {
                        bool isPointer = false;
                        if (nextWord == "*") 
                        {
                            isPointer = true;
                            nextWord = GetNextWord(ref script);
                            if ((script.Length == 0) || (!Char.IsLetter(nextWord[0])))
                            {
                                lastWord = string.Empty;
                                continue;
                            }
                        }
                        string variableName = nextWord;
                        nextWord = GetNextWord(ref script);
                        bool isArray = false;
                        bool isDynamicArray = false;
                        if (nextWord == "[") 
                        {
                            isArray = true;
                            if (PeekNextWord(script) == "]")
                                isDynamicArray = true;

                            while ((script.Length > 0) && (GetNextWord(ref script) != "]")) ;
							nextWord = GetNextWord(ref script);
                        }

                        if (((nextWord == "=") || (nextWord == ";") || (nextWord == ",")) &&
                            (lastWord != "return") && (lastWord != "else"))
                        {
							variables.Add(new ScriptVariable(variableName, lastWord, isArray, isDynamicArray, isPointer, null, null, false, false, false, false, false, (scriptToParse.Length - script.Length) + relativeCharacterIndex));
                        }
                        if (nextWord != ",")
                        {
                            lastWord = string.Empty;
                        }
                    }
                    else
                    {
                        lastWord = nextWord;
                    }
                }
                else
                {
                    lastWord = string.Empty;
                }
            }
            return;
        }
    }
}
