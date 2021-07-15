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

#include "ac/dynobj/scriptgamepad.h"
#include "ac/gamepad.h"
#include "util/stream.h"

using namespace AGS::Common;

ScriptGamepad::ScriptGamepad(int id) : _id(id) {}

const char *ScriptGamepad::GetType() {
    return "Gamepad";
}

int ScriptGamepad::Dispose(const char *address, bool force) {
    Gamepad_Close(this);
    delete this;
    return 1;
}

size_t ScriptGamepad::CalcSerializeSize() {
    return sizeof(int32_t);
}

void ScriptGamepad::Serialize(const char *address, AGS::Common::Stream *out) {
    out->WriteInt32(_id);
}

void ScriptGamepad::Unserialize(int index, AGS::Common::Stream *in, size_t data_sz) {
    _id = in->ReadInt32();
    ccRegisterUnserializedObject(index, this, this);
}