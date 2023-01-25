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

#ifndef __AC_SCRIPTJOYSTICK_H
#define __AC_SCRIPTJOYSTICK_H

#include "ac/dynobj/cc_agsdynamicobject.h"
#include "SDL_gamecontroller.h"

struct ScriptJoystick final : AGSCCDynamicObject {
public:
    ScriptJoystick(int id);
    // Get gamepad index; negative means the gamepad was deleted
    int GetID() const { return _id; }
    void SetID(int id) { _id = id; }
    // Reset gamepad index to indicate that this reference is no longer valid
    void Invalidate() { _id = -1; }

    const char *GetType() override;
    int Dispose(const char *address, bool force) override;
    void Unserialize(int index, AGS::Common::Stream *in, size_t data_sz) override;

    SDL_GameController* sdl_con = nullptr;
    SDL_Joystick* sdl_joy = nullptr;
    SDL_JoystickID instanceid = -1;

protected:
    // Calculate and return required space for serialization, in bytes
    size_t CalcSerializeSize() override;
    // Write object data into the provided stream
    void Serialize(const char *address, AGS::Common::Stream *out) override;

private:
    int _id = -1;
};

#endif // __AC_SCRIPTJOYSTICK_H