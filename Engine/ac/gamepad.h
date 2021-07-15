//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
//
//
//=============================================================================
#ifndef __AGS_EE_AC__GAMEPAD_H
#define __AGS_EE_AC__GAMEPAD_H

#include "ac/dynobj/scriptgamepad.h"
#include "SDL_gamecontroller.h"

enum eAGSGamepad_Hat
{
    eAGSGamepad_HatInvalid,
    eAGSGamepad_HatCentered,
    eAGSGamepad_HatUp,
    eAGSGamepad_HatRight,
    eAGSGamepad_HatDown,
    eAGSGamepad_HatLeft,
    eAGSGamepad_HatRightUp,
    eAGSGamepad_HatRightDown,
    eAGSGamepad_HatLeftUp,
    eAGSGamepad_HatLeftDown,
    eAGSGamepad_HatCount
};

enum eAGSGamepad_Axis
{
    eAGSGamepad_AxisInvalid,
    eAGSGamepad_AxisLeftX,
    eAGSGamepad_AxisLeftY,
    eAGSGamepad_AxisRightX,
    eAGSGamepad_AxisRightY,
    eAGSGamepad_AxisTriggerLeft,
    eAGSGamepad_AxisTriggerRight,
    eAGSGamepad_AxisCount
};

enum eAGSGamepad_Button
{
    eAGSGamepad_ButtonInvalid,
    eAGSGamepad_ButtonA,
    eAGSGamepad_ButtonB,
    eAGSGamepad_ButtonX,
    eAGSGamepad_ButtonY,
    eAGSGamepad_ButtonBack,
    eAGSGamepad_ButtonGuide,
    eAGSGamepad_ButtonStart,
    eAGSGamepad_ButtonLeftStick,
    eAGSGamepad_ButtonRightStick,
    eAGSGamepad_ButtonLeftShoulder,
    eAGSGamepad_ButtonRightShoulder,
    eAGSGamepad_ButtonDpadUp,
    eAGSGamepad_ButtonDpadDown,
    eAGSGamepad_ButtonDpadLeft,
    eAGSGamepad_ButtonDpadRight,
    eAGSGamepad_ButtonCount
};

enum eAGSGamepad_InputType
{
    eAGSGamepad_InputTypeNone,
    eAGSGamepad_InputTypeAxis,
    eAGSGamepad_InputTypeButton,
    eAGSGamepad_InputTypeHat,
    eAGSGamepad_InputTypeCount,
};

struct GamepadInput
{
    int gamepad_index = -1;
    eAGSGamepad_Button Button = eAGSGamepad_Button::eAGSGamepad_ButtonInvalid;
    eAGSGamepad_Axis Axis = eAGSGamepad_Axis::eAGSGamepad_AxisInvalid;
    eAGSGamepad_InputType Type = eAGSGamepad_InputType::eAGSGamepad_InputTypeNone;
};

void Gamepad_Close(ScriptGamepad* pad);

uint8_t Gamepad_Hat_AGStoSDL(eAGSGamepad_Hat ags_hat);
SDL_GameControllerAxis Gamepad_Axis_AGStoSDL(eAGSGamepad_Axis ags_axis);
SDL_GameControllerButton Gamepad_Button_AGStoSDL(eAGSGamepad_Button ags_button);
eAGSGamepad_Hat Gamepad_Hat_SDLtoAGS(uint8_t sdl_hat);
eAGSGamepad_Axis Gamepad_Axis_SDLtoAGS(SDL_GameControllerAxis sdl_axis);
eAGSGamepad_Button Gamepad_Button_SDLtoAGS(SDL_GameControllerButton sdl_button);

#endif // __AGS_EE_AC__GAMEPAD_H