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

#ifndef __AC_DEBUGGER_H
#define __AC_DEBUGGER_H

#include "util/string.h"

struct IAGSEditorDebugger;
struct ScriptPosition;

extern int editor_debugging_enabled;
extern int editor_debugging_initialized;
extern char editor_debugger_instance_token[100];
extern IAGSEditorDebugger *editor_debugger;
extern int break_on_next_script_step;

int check_for_messages_from_editor();
bool send_message_to_editor(const char *msg);
bool send_exception_to_editor(const char *qmsg);
void check_debug_keys();

enum DebugFlags {
    kfDbg_NoIface       = 0x0001,
    kfDbg_NoDrawSprites = 0x0002,
    kfDbg_NoObjects     = 0x0004,
    kfDbg_NoUpdate      = 0x0008,
    kfDbg_NoSfx         = 0x0010,
    kfDbg_NoMusic       = 0x0020,
    kfDbg_NoScript      = 0x0040,
//  kfDbg_DbgScript     = 0x0080, // unused
    kfDbg_DebugMode     = 0x0100,
    kfDbg_RegOnly       = 0x0200,
    kfDbg_NoVideo       = 0x0400
};

enum FPSDisplayMode
{
    kFPS_Hide = 0,    // hid by the script/user command
    kFPS_Display = 1, // shown by the script/user command
    kFPS_Forced = 2   // forced shown by the engine arg
};

extern float fps;
extern FPSDisplayMode display_fps;
extern int debug_flags;

#endif // __AC_DEBUGGER_H
