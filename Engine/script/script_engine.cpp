
/* 
Script Editor run-time engine component (c) 1998 Chris Jones
script chunk format:
00h  1 dword  version - should be 2
04h  1 dword  sizeof(scriptblock)
08h  1 dword  number of ScriptBlocks
0Ch  n STRUCTs ScriptBlocks

Adventure Game Studio source code Copyright 1999-2011 Chris Jones.
All rights reserved.

The AGS Editor Source Code is provided under the Artistic License 2.0
http://www.opensource.org/licenses/artistic-license-2.0.php

You MAY NOT compile your own builds of the engine without making it EXPLICITLY
CLEAR that the code has been altered from the Standard Version.
*/
#include <stdio.h>
#include <stdlib.h>
#include "util/wgt2allg.h"
#include "ac/roomstruct.h"
#include "util/filestream.h"
#include "script/cc_instance.h"
#include "script/cc_error.h"

using AGS::Common::DataStream;

char *scripteditruntimecopr = "Script Editor v1.2 run-time component. (c) 1998 Chris Jones";

#define SCRIPT_CONFIG_VERSION 1
extern void quit(char *);
extern int currentline; // in script/script_common

void cc_error_at_line(char *buffer, const char *error_msg)
{
    if (ccInstance::GetCurrentInstance() == NULL) {
        sprintf(ccErrorString, "Error (line %d): %s", currentline, error_msg);
    }
    else {
        sprintf(ccErrorString, "Error: %s\n", error_msg);
        ccInstance::GetCurrentInstance()->GetCallStack(ccErrorCallStack, 5);
    }
}

void save_script_configuration(DataStream *out)
{
    quit("ScriptEdit: run-time version can't save");
}

void load_script_configuration(DataStream *in)
{
    int aa;
    if (in->ReadInt32() != SCRIPT_CONFIG_VERSION)
        quit("ScriptEdit: invalid config version");

    int numvarnames = in->ReadInt32();
    for (aa = 0; aa < numvarnames; aa++) {
        int lenoft = in->ReadByte();
        in->Seek(Common::kSeekCurrent, lenoft);
    }
}

void save_graphical_scripts(DataStream *out, roomstruct * rss)
{
    quit("ScriptEdit: run-time version can't save");
}

char *scripttempn = "~acsc%d.tmp";
extern int route_script_link();

void load_graphical_scripts(DataStream *in, roomstruct * rst)
{
    long ct;

    if (route_script_link()) {
        quit("STOP IT.");
        exit(767);
        abort();
    }

    while (1) {
        ct = in->ReadInt32();
        if ((ct == -1) | (in->EOS() != 0))
            break;

        long lee;
        lee = in->ReadInt32();

        char thisscn[20];
        sprintf(thisscn, scripttempn, ct);
        DataStream *te = Common::File::CreateFile(thisscn);

        char *scnf = (char *)malloc(lee);
        // MACPORT FIX: swap size and nmemb
        in->Read(scnf, lee);
        te->Write(scnf, lee);
        delete te;

        free(scnf);
    }
}
