//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2024 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
//
//
//
//=============================================================================
#ifndef __AGS_EE_SCRIPT__EXECUTINGSCRIPT_H
#define __AGS_EE_SCRIPT__EXECUTINGSCRIPT_H

#include <vector>
#include "script/cc_instance.h"
#include "gfx/bitmap.h"

#define MAX_SCRIPT_EVT_PARAMS  4

// A general script type, used to search for or run a function.
// NOTE: "game" type may mean either "global script" or any script module,
// depending on other circumstances.
// TODO: get rid of this later, remains of old event function logic
enum ScriptType
{
    kScTypeNone,
    kScTypeGame,    // game script modules
    kScTypeRoom     // room script
};

// ScriptFunctionRef - represents a *unresolved* reference to the script function.
// This means that it has only script and function names, but not the actual
// pointer to a loaded bytecode.
struct ScriptFunctionRef
{
    AGS::Common::String ModuleName;
    AGS::Common::String FuncName;
 
    ScriptFunctionRef() = default;
    ScriptFunctionRef(const AGS::Common::String &fn_name)
        : FuncName(fn_name) {}
    ScriptFunctionRef(const AGS::Common::String &module_name, const AGS::Common::String &fn_name)
        : ModuleName(module_name), FuncName(fn_name) {}
    bool IsEmpty() const { return FuncName.IsEmpty(); }
    operator bool() const { return FuncName.IsEmpty(); }
};

struct QueuedScript
{
    ScriptType         ScType = kScTypeNone;
    ScriptFunctionRef  Function;
    size_t             ParamCount = 0u;
    RuntimeScriptValue Params[MAX_SCRIPT_EVT_PARAMS];

    QueuedScript() = default;
};

enum PostScriptActionType
{
    ePSAUndefined,
    ePSANewRoom,
    ePSAInvScreen,
    ePSARestoreGame,
    ePSARestoreGameDialog,
    ePSARunAGSGame,
    ePSARunDialog,
    ePSARestartGame,
    ePSASaveGame,
    ePSASaveGameDialog
};

struct PostScriptAction
{
    PostScriptActionType Type = ePSAUndefined;
    int Data = 0;
    Common::String Name;
    Common::String Description;
    mutable std::unique_ptr<Common::Bitmap> Image;
    ScriptPosition Position;

    PostScriptAction() = default;
    PostScriptAction(PostScriptActionType type, int data, const Common::String &name)
        : Type(type), Data(data), Name(name) {}
    PostScriptAction(PostScriptActionType type, int data, const Common::String &name, const Common::String &desc)
        : Type(type), Data(data), Name(name), Description(desc) {}
    PostScriptAction(PostScriptActionType type, int data, const Common::String &name, const Common::String &desc,
        std::unique_ptr<Common::Bitmap> &&image)
        : Type(type), Data(data), Name(name), Description(desc), Image(std::move(image)) {}
};

struct ExecutingScript
{
    // Instance refers either to one of the global instances,
    // or a ForkedInst created for this purpose
    ccInstance *Inst = nullptr;
    // owned fork; CHECKME: this seem unused in the current engine
    std::unique_ptr<ccInstance> ForkedInst{};
    std::vector<PostScriptAction> PostScriptActions;
    std::vector<QueuedScript> ScFnQueue;

    ExecutingScript() = default;
    void QueueAction(PostScriptAction &&act);
    void RunAnother(ScriptType scinst, const AGS::Common::String &fn_name,
        size_t param_count, const RuntimeScriptValue *params);
    void RunAnother(ScriptType scinst, const ScriptFunctionRef &fn_ref,
        size_t param_count, const RuntimeScriptValue *params);
};

#endif // __AGS_EE_SCRIPT__EXECUTINGSCRIPT_H
