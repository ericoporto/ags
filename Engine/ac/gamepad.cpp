#include "ac/string.h"
#include "ac/gamestructdefines.h"
#include "ac/gamepad.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

//uint8_t Gamepad_Hat_AGStoSDL(eAGSGamepad_Hat ags_hat) {
//    switch (ags_hat) {
//        case eAGSGamepad_HatCentered: return SDL_HAT_CENTERED;
//        case eAGSGamepad_HatUp: return SDL_HAT_UP;
//        case eAGSGamepad_HatRight: return SDL_HAT_RIGHT;
//        case eAGSGamepad_HatDown: return SDL_HAT_DOWN;
//        case eAGSGamepad_HatLeft: return SDL_HAT_LEFT;
//        case eAGSGamepad_HatRightUp: return SDL_HAT_RIGHTUP;
//        case eAGSGamepad_HatRightDown: return SDL_HAT_RIGHTDOWN;
//        case eAGSGamepad_HatLeftUp: return SDL_HAT_LEFTUP;
//        case eAGSGamepad_HatLeftDown: return SDL_HAT_LEFTDOWN;
//    }
//    return SDL_HAT_CENTERED;
//}

// clamps to -1.0 to 1.0, taking into account a dead-zone from 0.0
float gamepad_clamp_val(float val, float dead_zone)
{
    dead_zone = std::max(dead_zone, 0.01f); // ignore negative values, but force minimal deadzone

    if(val < 0.0f) {
        val = val < -dead_zone ? val : 0.0f;
    } else {
        val = val > dead_zone ? val : 0.0f;
    }

    if (val < -0.99f) return -1.0f;
    if (val > 0.99f) return 1.0f;

    return val;
}

SDL_GameControllerAxis Gamepad_Axis_AGStoSDL( eAGSGamepad_Axis ags_axis) {
    switch (ags_axis) {
        case eAGSGamepad_AxisInvalid: return SDL_CONTROLLER_AXIS_INVALID;
        case eAGSGamepad_AxisLeftX: return SDL_CONTROLLER_AXIS_LEFTX;
        case eAGSGamepad_AxisLeftY: return SDL_CONTROLLER_AXIS_LEFTY;
        case eAGSGamepad_AxisRightX: return SDL_CONTROLLER_AXIS_RIGHTX;
        case eAGSGamepad_AxisRightY: return SDL_CONTROLLER_AXIS_RIGHTY;
        case eAGSGamepad_AxisTriggerLeft: return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
        case eAGSGamepad_AxisTriggerRight: return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
    }
    return SDL_CONTROLLER_AXIS_INVALID;
}

SDL_GameControllerButton Gamepad_Button_AGStoSDL(eAGSGamepad_Button ags_button) {
    switch (ags_button) {
        case eAGSGamepad_ButtonInvalid: return SDL_CONTROLLER_BUTTON_INVALID;
        case eAGSGamepad_ButtonA: return SDL_CONTROLLER_BUTTON_A;
        case eAGSGamepad_ButtonB: return SDL_CONTROLLER_BUTTON_B;
        case eAGSGamepad_ButtonX: return SDL_CONTROLLER_BUTTON_X;
        case eAGSGamepad_ButtonY: return SDL_CONTROLLER_BUTTON_Y;
        case eAGSGamepad_ButtonBack: return SDL_CONTROLLER_BUTTON_BACK;
        case eAGSGamepad_ButtonGuide: return SDL_CONTROLLER_BUTTON_GUIDE;
        case eAGSGamepad_ButtonStart: return SDL_CONTROLLER_BUTTON_START;
        case eAGSGamepad_ButtonLeftStick: return  SDL_CONTROLLER_BUTTON_LEFTSTICK;
        case eAGSGamepad_ButtonRightStick: return  SDL_CONTROLLER_BUTTON_RIGHTSTICK;
        case eAGSGamepad_ButtonLeftShoulder: return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        case eAGSGamepad_ButtonRightShoulder: return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case eAGSGamepad_ButtonDpadUp: return  SDL_CONTROLLER_BUTTON_DPAD_UP;
        case eAGSGamepad_ButtonDpadDown: return  SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        case eAGSGamepad_ButtonDpadLeft: return  SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        case eAGSGamepad_ButtonDpadRight: return  SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    }
    return SDL_CONTROLLER_BUTTON_INVALID;
}

//eAGSGamepad_Hat Gamepad_Hat_SDLtoAGS(uint8_t sdl_hat) {
//    switch (sdl_hat) {
//        case SDL_HAT_CENTERED: return eAGSGamepad_HatCentered;
//        case SDL_HAT_UP: return eAGSGamepad_HatUp;
//        case SDL_HAT_RIGHT: return eAGSGamepad_HatRight;
//        case SDL_HAT_DOWN: return eAGSGamepad_HatDown;
//        case SDL_HAT_LEFT: return eAGSGamepad_HatLeft;
//        case SDL_HAT_RIGHTUP: return eAGSGamepad_HatRightUp;
//        case SDL_HAT_RIGHTDOWN: return eAGSGamepad_HatRightDown;
//        case SDL_HAT_LEFTUP: return eAGSGamepad_HatLeftUp;
//        case SDL_HAT_LEFTDOWN: return eAGSGamepad_HatLeftDown;
//    }
//    return eAGSGamepad_HatCentered;
//}

eAGSGamepad_Axis Gamepad_Axis_SDLtoAGS(SDL_GameControllerAxis sdl_axis) {
    switch (sdl_axis) {
        case SDL_CONTROLLER_AXIS_INVALID: return eAGSGamepad_AxisInvalid;
        case SDL_CONTROLLER_AXIS_LEFTX: return eAGSGamepad_AxisLeftX;
        case SDL_CONTROLLER_AXIS_LEFTY: return eAGSGamepad_AxisLeftY;
        case SDL_CONTROLLER_AXIS_RIGHTX: return eAGSGamepad_AxisRightX;
        case SDL_CONTROLLER_AXIS_RIGHTY: return eAGSGamepad_AxisRightY;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return eAGSGamepad_AxisTriggerLeft;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return eAGSGamepad_AxisTriggerRight;
    }
    return eAGSGamepad_AxisInvalid;
}

eAGSGamepad_Button Gamepad_Button_SDLtoAGS(SDL_GameControllerButton sdl_button) {
    switch (sdl_button) {
        case SDL_CONTROLLER_BUTTON_INVALID: return eAGSGamepad_ButtonInvalid;
        case SDL_CONTROLLER_BUTTON_A: return eAGSGamepad_ButtonA;
        case SDL_CONTROLLER_BUTTON_B: return eAGSGamepad_ButtonB;
        case SDL_CONTROLLER_BUTTON_X: return eAGSGamepad_ButtonX;
        case SDL_CONTROLLER_BUTTON_Y: return eAGSGamepad_ButtonY;
        case SDL_CONTROLLER_BUTTON_BACK: return eAGSGamepad_ButtonBack;
        case SDL_CONTROLLER_BUTTON_GUIDE: return eAGSGamepad_ButtonGuide;
        case SDL_CONTROLLER_BUTTON_START: return eAGSGamepad_ButtonStart;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: return eAGSGamepad_ButtonLeftStick;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return eAGSGamepad_ButtonRightStick;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return eAGSGamepad_ButtonLeftShoulder;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return eAGSGamepad_ButtonRightShoulder;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: return eAGSGamepad_ButtonDpadUp;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return eAGSGamepad_ButtonDpadDown;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return eAGSGamepad_ButtonDpadLeft;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return eAGSGamepad_ButtonDpadRight;
    }
    return eAGSGamepad_ButtonInvalid;
}

int32_t Gamepad_GetCount()
{
    return SDL_NumJoysticks();
}

ScriptGamepad* Gamepad_Open(int i)
{
    if (!SDL_IsGameController(i)) return nullptr;

    SDL_GameController* sdlGameController = SDL_GameControllerOpen(i);

    if(sdlGameController == nullptr) return nullptr;

    ScriptGamepad *newGamepad = new ScriptGamepad(-1);
    newGamepad->sdl_con = sdlGameController;
    newGamepad->sdl_joy = SDL_GameControllerGetJoystick(sdlGameController);
    int id = ccRegisterManagedObject(newGamepad, newGamepad);
    newGamepad->SetID(id);
    return newGamepad;
}


const char* Gamepad_GetNameForIndex(int i) {
    const char* con_str = SDL_GameControllerNameForIndex(i);
    if(con_str == nullptr) return nullptr;
    return CreateNewScriptString(con_str);
}

void Gamepad_Close(ScriptGamepad* pad)
{
    if(pad->sdl_con != nullptr) {
        SDL_GameControllerClose(pad->sdl_con);
    }
    pad->sdl_joy = nullptr;
    pad->sdl_con = nullptr;
}

const char* Gamepad_GetName(ScriptGamepad* pad) {
    if(pad->sdl_con == nullptr) return nullptr;
    const char* con_str = SDL_GameControllerName(pad->sdl_con);
    if(con_str == nullptr) return nullptr;
    return CreateNewScriptString(con_str);
}

int Gamepad_IsConnected(ScriptGamepad* pad)
{
    if(pad->sdl_con == nullptr || pad->sdl_joy == nullptr) return 0;
    return SDL_GameControllerGetAttached(pad->sdl_con) == SDL_TRUE ? 1 : 0;
}

int Gamepad_IsButtonDown(ScriptGamepad* pad, int butt)
{
    if(pad->sdl_con == nullptr || pad->sdl_joy == nullptr) return 0;
    return SDL_GameControllerGetButton(pad->sdl_con, Gamepad_Button_AGStoSDL(static_cast<eAGSGamepad_Button>(butt)));
}

float Gamepad_GetAxis(ScriptGamepad* pad, int axis, float dead_zone)
{
    if(pad->sdl_con == nullptr || pad->sdl_joy == nullptr) return 0;

    int axis_val_int = SDL_GameControllerGetAxis(pad->sdl_con, Gamepad_Axis_AGStoSDL(static_cast<eAGSGamepad_Axis>(axis)));
    float axis_val = axis_val_int < 0 ? static_cast<float>(axis_val_int)/32768.0f : static_cast<float>(axis_val_int)/32767.0f;

    return gamepad_clamp_val(axis_val, dead_zone);
}

//int Gamepad_GetHat(ScriptGamepad* pad, int butt)
//{
//    if(pad->sdl_con == nullptr || pad->sdl_joy == nullptr) return 0;
//    return SDL_JoystickGetHat(pad->sdl_joy, Gamepad_Hat_AGStoSDL(static_cast<eAGSGamepad_Hat>(butt)));
//}

//=============================================================================
//
// Script API Functions
//
//=============================================================================

#include "debug/out.h"
#include "script/script_api.h"
#include "script/script_runtime.h"
#include "ac/dynobj/scriptstring.h"

extern ScriptString myScriptStringImpl;

// int ScriptGamepad::()
RuntimeScriptValue Sc_Gamepad_GetCount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(Gamepad_GetCount);
}

// ScriptGamepad* ScriptGamepad::(int index)
RuntimeScriptValue Sc_Gamepad_Open(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_OBJAUTO_PINT(ScriptGamepad, Gamepad_Open);
}

// String* ScriptGamepad::(int index)
RuntimeScriptValue Sc_Gamepad_GetNameForIndex(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_OBJ_PINT(const char, myScriptStringImpl, Gamepad_GetNameForIndex);
}

// String* (ScriptGamepad *pad)
RuntimeScriptValue Sc_Gamepad_GetName(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_OBJ(ScriptGamepad, const char, myScriptStringImpl, Gamepad_GetName);
}

// void (ScriptGamepad *pad)
RuntimeScriptValue Sc_Gamepad_Close(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_VOID(ScriptGamepad, Gamepad_Close);
}

// int (ScriptGamepad *pad)
RuntimeScriptValue Sc_Gamepad_IsConnected(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT(ScriptGamepad, Gamepad_IsConnected);
}

// int (ScriptGamepad *pad, int button)
RuntimeScriptValue Sc_Gamepad_IsButtonDown(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT_PINT(ScriptGamepad, Gamepad_IsButtonDown);
}

// float (ScriptGamepad *pad, int axis)
RuntimeScriptValue Sc_Gamepad_GetAxis(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_FLOAT_PINT_PFLOAT(ScriptGamepad, Gamepad_GetAxis);
}

// int (ScriptGamepad *pad, int hat)
//RuntimeScriptValue Sc_Gamepad_GetHat(void *self, const RuntimeScriptValue *params, int32_t param_count)
//{
//    API_OBJCALL_INT_PINT(ScriptGamepad, Gamepad_GetHat);
//}

void RegisterGamepadAPI()
{
    ccAddExternalStaticFunction("Gamepad::GetCount^0",            Sc_Gamepad_GetCount);
    ccAddExternalStaticFunction("Gamepad::Open^1",                Sc_Gamepad_Open);
    ccAddExternalStaticFunction("Gamepad::GetName^1",             Sc_Gamepad_GetNameForIndex);
    ccAddExternalObjectFunction("Gamepad::get_Name",              Sc_Gamepad_GetName);
    ccAddExternalObjectFunction("Gamepad::Close^0",               Sc_Gamepad_Close);
    ccAddExternalObjectFunction("Gamepad::IsConnected^0",         Sc_Gamepad_IsConnected);
    ccAddExternalObjectFunction("Gamepad::IsButtonDown^1",        Sc_Gamepad_IsButtonDown);
    ccAddExternalObjectFunction("Gamepad::GetAxis^2",             Sc_Gamepad_GetAxis);
//    ccAddExternalObjectFunction("Gamepad::GetHat^1",              Sc_Gamepad_GetHat);

    /* ----------------------- Registering unsafe exports for plugins -----------------------*/

    ccAddExternalFunctionForPlugin("Gamepad::GetCount^0",       (void*) Gamepad_GetCount);
    ccAddExternalFunctionForPlugin("Gamepad::Open^1",           (void*) Gamepad_Open);
    ccAddExternalFunctionForPlugin("Gamepad::GetName^1",        (void*) Gamepad_GetNameForIndex);
    ccAddExternalFunctionForPlugin("Gamepad::get_Name",         (void*) Gamepad_GetName);
    ccAddExternalFunctionForPlugin("Gamepad::Close^0",          (void*) Gamepad_Close);
    ccAddExternalFunctionForPlugin("Gamepad::IsConnected^0",    (void*) Gamepad_IsConnected);
    ccAddExternalFunctionForPlugin("Gamepad::IsButtonDown^1",   (void*) Gamepad_IsButtonDown);
    ccAddExternalFunctionForPlugin("Gamepad::GetAxis^2",        (void*) Gamepad_GetAxis);
//    ccAddExternalFunctionForPlugin("Gamepad::GetHat^1",         (void*) Gamepad_GetHat);
}