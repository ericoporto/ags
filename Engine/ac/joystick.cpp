#include "ac/string.h"
#include "ac/gamestructdefines.h"
#include "ac/joystick.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"
#include "debug/debug_log.h"

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

float axis_to_float_with_deadzone(int axis_val_int, float dead_zone) {
    float axis_val = axis_val_int < 0 ? static_cast<float>(axis_val_int) / 32768.0f : static_cast<float>(axis_val_int) / 32767.0f;
    return gamepad_clamp_val(axis_val, dead_zone);
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

eAGSJoystick_Hat Joystick_Hat_SDLtoAGS(uint8_t sdl_hat) {
    switch (sdl_hat) {
        case SDL_HAT_CENTERED: return eAGSJoystick_HatCentered;
        case SDL_HAT_UP: return eAGSJoystick_HatUp;
        case SDL_HAT_RIGHT: return eAGSJoystick_HatRight;
        case SDL_HAT_DOWN: return eAGSJoystick_HatDown;
        case SDL_HAT_LEFT: return eAGSJoystick_HatLeft;
        case SDL_HAT_RIGHTUP: return eAGSJoystick_HatRightUp;
        case SDL_HAT_RIGHTDOWN: return eAGSJoystick_HatRightDown;
        case SDL_HAT_LEFTUP: return eAGSJoystick_HatLeftUp;
        case SDL_HAT_LEFTDOWN: return eAGSJoystick_HatLeftDown;
    }
    return eAGSJoystick_HatCentered;
}

uint8_t Joystick_Hat_AGStoSDL(eAGSJoystick_Hat ags_hat)
{
    switch (ags_hat) {
        case eAGSJoystick_HatCentered: return SDL_HAT_CENTERED;
        case eAGSJoystick_HatUp: return SDL_HAT_UP;
        case eAGSJoystick_HatRight: return SDL_HAT_RIGHT;
        case eAGSJoystick_HatDown: return SDL_HAT_DOWN;
        case eAGSJoystick_HatLeft: return  SDL_HAT_LEFT;
        case eAGSJoystick_HatRightUp: return  SDL_HAT_RIGHTUP;
        case eAGSJoystick_HatRightDown: return SDL_HAT_RIGHTDOWN;
        case eAGSJoystick_HatLeftUp: return SDL_HAT_LEFTUP;
        case eAGSJoystick_HatLeftDown: return SDL_HAT_LEFTDOWN;
    }
    return SDL_HAT_CENTERED;
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

std::vector<ScriptJoystick*> joysticks;

int32_t Joystick_GetLength()
{
    return joysticks.size();
}

ScriptJoystick* Joystick_Joysticks(int i)
{
    if(i < 0 || i > joysticks.size()) return nullptr;
    return joysticks[i];
}

void joystick_close(ScriptJoystick* joy)
{
    if(joy->sdl_con != nullptr) {
        SDL_GameControllerClose(joy->sdl_con);
        joy->sdl_joy = nullptr; // owned by the SDL_GameController. Do not call SDL_JoystickClose()
    }

    if(joy->sdl_joy != nullptr) {
        SDL_JoystickClose(joy->sdl_joy);
    }
    joy->sdl_joy = nullptr;
    joy->sdl_con = nullptr;
}

void joystick_device_added(int index) {
    SDL_GameController* sdlGameController = nullptr;
    SDL_Joystick* sdl_joy = nullptr;

    if(SDL_IsGameController(index)) {
        sdlGameController = SDL_GameControllerOpen(index);
        sdl_joy = SDL_GameControllerGetJoystick(sdlGameController);
    } else {
        sdl_joy = SDL_JoystickOpen(index);
    }

    if (sdl_joy == nullptr) return;

    SDL_JoystickID joy_id = SDL_JoystickInstanceID(sdl_joy);

    ScriptJoystick *newJoy = new ScriptJoystick(-1);
    newJoy->sdl_joy = sdl_joy;
    newJoy->sdl_con = sdlGameController;
    newJoy->instanceid = joy_id;
    int id = ccRegisterManagedObject(newJoy, newJoy);
    newJoy->SetID(id);
    joysticks.emplace_back(newJoy);

    // add internal reference
    ccAddObjectReference(id);
}

void joystick_device_removed(int instance_id) {
    size_t index = -1;
    for(size_t i=0; i<joysticks.size(); i++)
    {
        if(joysticks[i]->instanceid == instance_id)
        {
            index = i;
            break;
        }
    }

    if(index == -1) return;

    ScriptJoystick* joy = joysticks[index];

    joystick_close(joy);
    ccReleaseObjectReference(joy->GetID());
    joysticks.erase(joysticks.begin() + index);
}

int Joystick_IsConnected(ScriptJoystick* joy)
{
    if(joy->sdl_con == nullptr || joy->sdl_joy == nullptr) return 0;
    if(joy->sdl_con != nullptr ) return SDL_GameControllerGetAttached(joy->sdl_con) == SDL_TRUE ? 1 : 0;
    return SDL_JoystickGetAttached(joy->sdl_joy) == SDL_TRUE ? 1 : 0;
}

int Joystick_IsGamepad(ScriptJoystick* joy)
{
    return joy->sdl_con != nullptr;
}

const char* Joystick_GetName(ScriptJoystick* joy) {
    if(Joystick_IsConnected(joy) == 0) return nullptr;
    const char* name_str = nullptr;
    if(joy->sdl_con != nullptr) {
        name_str = SDL_GameControllerName(joy->sdl_con);
    } else {
        name_str = SDL_JoystickName(joy->sdl_joy);
    }
    if(name_str == nullptr) return nullptr;
    return CreateNewScriptString(name_str);
}

int Joystick_IsGamepadButtonDown(ScriptJoystick* joy, int butt)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    return SDL_GameControllerGetButton(joy->sdl_con, Gamepad_Button_AGStoSDL(static_cast<eAGSGamepad_Button>(butt)));
}

float Joystick_GetGamepadAxis(ScriptJoystick* joy, int axis, float dead_zone)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    int axis_val_int = SDL_GameControllerGetAxis(joy->sdl_con, Gamepad_Axis_AGStoSDL(static_cast<eAGSGamepad_Axis>(axis)));
    return axis_to_float_with_deadzone(axis_val_int, dead_zone);
}

float Joystick_GetAxis(ScriptJoystick* joy, int axis, float dead_zone)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    int axis_count = SDL_JoystickNumAxes(joy->sdl_joy);
    if(axis < 0 || axis >= axis_count) {
        debug_script_warn("Warning: joystick's (id %d) axis %d is not in range (0:%d), returned 0",
                          joy->instanceid, axis, axis_count);
        return 0;
    }
    int axis_val_int = SDL_JoystickGetAxis(joy->sdl_joy, axis);
    return axis_to_float_with_deadzone(axis_val_int, dead_zone);

}

int Joystick_IsButtonDown(ScriptJoystick* joy, int butt)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    int button_count = SDL_JoystickNumButtons(joy->sdl_joy);
    if(butt < 0 || butt >= button_count) {
        debug_script_warn("Warning: joystick's (id %d) button %d is not in range (0:%d), returned false",
                          joy->instanceid, butt, button_count);
        return 0;
    }
    return SDL_JoystickGetButton(joy->sdl_joy, butt);
}


int Joystick_GetHat(ScriptJoystick* joy, int hat)
{
    if(Joystick_IsConnected(joy) == 0) return eAGSJoystick_Hat::eAGSJoystick_HatCentered;
    int hat_count = SDL_JoystickNumHats(joy->sdl_joy);
    if(hat < 0 || hat >= hat_count) {
        debug_script_warn("Warning: joystick's (id %d) hat %d is not in range (0:%d), returned HatCentered",
                          joy->instanceid, hat, hat_count);
        return eAGSJoystick_Hat::eAGSJoystick_HatCentered;
    }
    return SDL_JoystickGetHat(joy->sdl_joy, Joystick_Hat_AGStoSDL(static_cast<eAGSJoystick_Hat>(hat)));
}

int Joystick_GetAxisCount(ScriptJoystick* joy)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    return SDL_JoystickNumAxes(joy->sdl_joy);
}

int Joystick_GetButtonCount(ScriptJoystick* joy)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    return SDL_JoystickNumButtons(joy->sdl_joy);
}

int Joystick_GetHatCount(ScriptJoystick* joy)
{
    if(Joystick_IsConnected(joy) == 0) return 0;
    return SDL_JoystickNumHats(joy->sdl_joy);
}

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

// int ScriptJoystick::()
RuntimeScriptValue Sc_Joystick_GetLength(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(Joystick_GetLength);
}

// ScriptJoystick* ScriptJoystick::(int index)
RuntimeScriptValue Sc_Joystick_Joysticks(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_OBJAUTO_PINT(ScriptJoystick, Joystick_Joysticks);
}

// String* (ScriptJoystick *joy)
RuntimeScriptValue Sc_Joystick_GetName(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_OBJ(ScriptJoystick, const char, myScriptStringImpl, Joystick_GetName);
}

// int (ScriptJoystick *joy)
RuntimeScriptValue Sc_Joystick_IsConnected(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT(ScriptJoystick, Joystick_IsConnected);
}

// int (ScriptJoystick *joy)
RuntimeScriptValue Sc_Joystick_IsGamepad(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT(ScriptJoystick, Joystick_IsGamepad);
}

// int (ScriptJoystick *joy, int button)
RuntimeScriptValue Sc_Joystick_IsGamepadButtonDown(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT_PINT(ScriptJoystick, Joystick_IsGamepadButtonDown);
}

// float (ScriptJoystick *joy, int axis)
RuntimeScriptValue Sc_Joystick_GetGamepadAxis(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_FLOAT_PINT_PFLOAT(ScriptJoystick, Joystick_GetGamepadAxis);
}

RuntimeScriptValue Sc_Joystick_GetAxis(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_FLOAT_PINT_PFLOAT(ScriptJoystick, Joystick_GetAxis);
}

RuntimeScriptValue Sc_Joystick_IsButtonDown(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT_PINT(ScriptJoystick, Joystick_IsButtonDown);
}

//int (ScriptJoystick *joy, int hat)
RuntimeScriptValue Sc_Joystick_GetHat(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT_PINT(ScriptJoystick, Joystick_GetHat);
}

// int (ScriptJoystick *joy)
RuntimeScriptValue Sc_Joystick_GetAxisCount(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT(ScriptJoystick, Joystick_GetAxisCount);
}

// int (ScriptJoystick *joy)
RuntimeScriptValue Sc_Joystick_GetButtonCount(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT(ScriptJoystick, Joystick_GetButtonCount);
}

// int (ScriptJoystick *joy)
RuntimeScriptValue Sc_Joystick_GetHatCount(void *self, const RuntimeScriptValue *params, int32_t param_count)
{
    API_OBJCALL_INT(ScriptJoystick, Joystick_GetHatCount);
}

void RegisterJoystickAPI()
{
    ccAddExternalStaticFunction("Joystick::get_Length",            Sc_Joystick_GetLength);
    ccAddExternalStaticFunction("Joystick::geti_Joysticks",        Sc_Joystick_Joysticks);
    ccAddExternalObjectFunction("Joystick::get_Name",              Sc_Joystick_GetName);
    ccAddExternalObjectFunction("Joystick::get_IsConnected",       Sc_Joystick_IsConnected);
    ccAddExternalObjectFunction("Joystick::get_IsGamepad",         Sc_Joystick_IsGamepad);
    ccAddExternalObjectFunction("Joystick::IsGamepadButtonDown^1", Sc_Joystick_IsGamepadButtonDown);
    ccAddExternalObjectFunction("Joystick::GetGamepadAxis^2",      Sc_Joystick_GetGamepadAxis);
    ccAddExternalObjectFunction("Joystick::GetAxis^2",             Sc_Joystick_GetAxis);
    ccAddExternalObjectFunction("Joystick::IsButtonDown^1",        Sc_Joystick_IsButtonDown);
    ccAddExternalObjectFunction("Joystick::GetHat^1",              Sc_Joystick_GetHat);
    ccAddExternalObjectFunction("Joystick::get_AxisCount",         Sc_Joystick_GetAxisCount);
    ccAddExternalObjectFunction("Joystick::get_ButtonCount",       Sc_Joystick_GetButtonCount);
    ccAddExternalObjectFunction("Joystick::get_HatCount",          Sc_Joystick_GetHatCount);

    /* ----------------------- Registering unsafe exports for plugins -----------------------*/

    ccAddExternalFunctionForPlugin("Joystick::get_Length",           (void*) Joystick_GetLength);
    ccAddExternalFunctionForPlugin("Joystick::geti_Joysticks",       (void*) Joystick_Joysticks);
    ccAddExternalFunctionForPlugin("Joystick::get_Name",             (void*) Joystick_GetName);
    ccAddExternalFunctionForPlugin("Joystick::get_IsConnected",      (void*) Joystick_IsConnected);
    ccAddExternalFunctionForPlugin("Joystick::get_IsGamepad",        (void*) Joystick_IsGamepad);
    ccAddExternalFunctionForPlugin("Joystick::IsGamepadButtonDown^1",(void*) Joystick_IsGamepadButtonDown);
    ccAddExternalFunctionForPlugin("Joystick::GetGamepadAxis^2",     (void*) Joystick_GetGamepadAxis);
    ccAddExternalFunctionForPlugin("Joystick::GetAxis^2",            (void*) Joystick_GetAxis);
    ccAddExternalFunctionForPlugin("Joystick::IsButtonDown^1",       (void*) Joystick_IsButtonDown);
    ccAddExternalFunctionForPlugin("Joystick::GetHat^1",             (void*) Joystick_GetHat);
    ccAddExternalFunctionForPlugin("Joystick::get_AxisCount",        (void*) Joystick_GetAxisCount);
    ccAddExternalFunctionForPlugin("Joystick::get_ButtonCount",      (void*) Joystick_GetButtonCount);
    ccAddExternalFunctionForPlugin("Joystick::get_HatCount",         (void*) Joystick_GetHatCount);
}