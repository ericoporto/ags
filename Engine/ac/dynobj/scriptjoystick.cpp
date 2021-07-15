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

#include "ac/dynobj/scriptjoystick.h"
#include "ac/joystick.h"
#include "util/stream.h"

using namespace AGS::Common;

ScriptJoystick::ScriptJoystick(int id) : _id(id) {}

const char *ScriptJoystick::GetType() {
    return "Joystick";
}

int ScriptJoystick::Dispose(const char *address, bool force) {
    joystick_close(this);
    delete this;
    return 1;
}

size_t ScriptJoystick::CalcSerializeSize() {
    return sizeof(int32_t);
}

void ScriptJoystick::Serialize(const char *address, AGS::Common::Stream *out) {
    out->WriteInt32(_id);
}

void ScriptJoystick::Unserialize(int index, AGS::Common::Stream *in, size_t data_sz) {
    _id = in->ReadInt32();
    ccRegisterUnserializedObject(index, this, this);
}