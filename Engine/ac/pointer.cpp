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

#include <array>
#include "pointer.h"
#include "ac/dynobj/scriptuserobject.h"
#include "device/mousew32.h"

enum pointer_state {
    pointer_up,
    pointer_motion,
    pointer_down
};

struct pointer {
    int x;
    int y;
    bool down;
};

struct _pp {
    static const int MAX_POINTERS = 11;
    std::array<pointer, MAX_POINTERS> pointers = {};
} _pp;

void on_pointer(int pointer_id, Point position, pointer_state state)
{
    _pp.pointers[pointer_id].x = position.X;
    _pp.pointers[pointer_id].y = position.Y;
    _pp.pointers[pointer_id].down = state != pointer_up;
}

void on_pointer_down(int pointer_id, Point position)
{
    on_pointer(pointer_id, position, pointer_down);
}

void on_pointer_motion(int pointer_id, Point position)
{
    on_pointer(pointer_id, position, pointer_motion);
}

void on_pointer_up(int pointer_id, Point position)
{
    on_pointer(pointer_id, position, pointer_up);
}

int Pointer_GetCount()
{
    return _pp::MAX_POINTERS;
}

ScriptUserObject* Pointer_GetPosition(int pointerNum)
{
    if (pointerNum < 0 || pointerNum >= _pp::MAX_POINTERS)
        return nullptr;

    if (pointerNum == 0) {
        return ScriptStructHelpers::CreatePoint(mousex, mousey);
    }

    pointer p = _pp.pointers[pointerNum];
    return ScriptStructHelpers::CreatePoint(p.x, p.y);
}

int Pointer_GetIsDown(int pointerNum)
{
    if (pointerNum < 0 || pointerNum >= _pp::MAX_POINTERS)
        return 0;

    if (pointerNum == 0) {
        return ags_misbuttondown(eAGSMouseButton::kMouseLeft) ? 1 : 0;
    }

    pointer p = _pp.pointers[pointerNum];
    return p.down;
}

//=============================================================================
//
// Script API Functions
//
//=============================================================================

#include "script/script_api.h"
#include "script/script_runtime.h"

// int ()
RuntimeScriptValue Sc_Pointer_GetCount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(Pointer_GetCount);
}

// Point* (int pointerNum)
RuntimeScriptValue Sc_Pointer_GetPosition(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_OBJAUTO_PINT(ScriptUserObject, Pointer_GetPosition);
}

// int (int pointerNum)
RuntimeScriptValue Sc_Pointer_GetIsDown(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT(Pointer_GetIsDown);
}

void RegisterPointerAPI()
{
    ScFnRegister pointer_api[] = {
            { "Pointer::get_Count",                   API_FN_PAIR(Pointer_GetCount) },
            { "Pointer::geti_Position",               API_FN_PAIR(Pointer_GetPosition) },
            { "Pointer::geti_IsDown",                 API_FN_PAIR(Pointer_GetIsDown) },
    };

    ccAddExternalFunctions(pointer_api);
}
