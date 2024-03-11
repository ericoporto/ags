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
#include "touch.h"
#include "ac/dynobj/scriptuserobject.h"
#include "device/mousew32.h"

enum touch_pointer_state {
    touch_pointer_up,
    touch_pointer_motion,
    touch_pointer_down
};

struct touch_point {
    int x;
    int y;
    bool down;
};

struct _pp {
    static const int MAX_POINTERS = 11;
    std::array<touch_point, MAX_POINTERS> touch_points = {};
} _pp;

void on_touch_pointer(int pointer_id, Point position, touch_pointer_state state)
{
    _pp.touch_points[pointer_id].x = position.X;
    _pp.touch_points[pointer_id].y = position.Y;
    _pp.touch_points[pointer_id].down = state != touch_pointer_up;
}

void on_touch_pointer_down(int pointer_id, Point position)
{
    on_touch_pointer(pointer_id, position, touch_pointer_down);
}

void on_touch_pointer_motion(int pointer_id, Point position)
{
    on_touch_pointer(pointer_id, position, touch_pointer_motion);
}

void on_touch_pointer_up(int pointer_id, Point position)
{
    on_touch_pointer(pointer_id, position, touch_pointer_up);
}

int Touch_GetTouchPointCount()
{
    return _pp::MAX_POINTERS;
}

ScriptUserObject* Touch_GetTouchPoint(int pointerNum)
{
    if (pointerNum < 0 || pointerNum >= _pp::MAX_POINTERS)
        return nullptr;

    if (pointerNum == 0) {
        int down = ags_misbuttondown(eAGSMouseButton::kMouseLeft) ? 1 : 0;
        return ScriptStructHelpers::CreateTouchPoint(0, mousex, mousey, down);
    }

    touch_point p = _pp.touch_points[pointerNum];
    return ScriptStructHelpers::CreateTouchPoint(pointerNum, p.x, p.y, p.down);
}

//=============================================================================
//
// Script API Functions
//
//=============================================================================

#include "script/script_api.h"
#include "script/script_runtime.h"

// int ()
RuntimeScriptValue Sc_Touch_GetTouchPointCount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(Touch_GetTouchPointCount);
}

// Point* (int pointerNum)
RuntimeScriptValue Sc_Touch_GetTouchPoint(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_OBJAUTO_PINT(ScriptUserObject, Touch_GetTouchPoint);
}

void RegisterPointerAPI()
{
    ScFnRegister pointer_api[] = {
            { "Touch::get_TouchPointCount",           API_FN_PAIR(Touch_GetTouchPointCount) },
            { "Touch::geti_TouchPoint",               API_FN_PAIR(Touch_GetTouchPoint) },
    };

    ccAddExternalFunctions(pointer_api);
}
