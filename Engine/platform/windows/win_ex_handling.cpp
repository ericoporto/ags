//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2025 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#include "core/platform.h"

// TODO: port exception handling to mingw
#if AGS_PLATFORM_OS_WINDOWS && !AGS_PLATFORM_WINDOWS_MINGW
#include <new.h>
#include <cinttypes>
#include <stdio.h>
#include "platform/windows/windows.h"
#include <Psapi.h>
#include "ac/common.h" // quit
#include "ac/common_defines.h"
#include "debug/out.h"
#include "util/ini_util.h"
#include "main/main.h"
#include "platform/base/sys_main.h"
#include "script/cc_common.h"
#include "ac/spritecache.h"

#if !AGS_PLATFORM_DEBUG
#define USE_CUSTOM_EXCEPTION_HANDLER
#endif

using namespace AGS::Common;

extern int eip_guinum;
extern int eip_guiobj;
extern int proper_exit;
extern SpriteCache spriteset;

char tempmsg[2048] = "";
#define PRINT_WORKSPACE_SIZE (7000u)
char *printfworkingspace;

#ifdef USE_CUSTOM_EXCEPTION_HANDLER
void CreateMiniDump(EXCEPTION_POINTERS *pep);

extern int CustomExceptionHandler(LPEXCEPTION_POINTERS exinfo);
extern EXCEPTION_RECORD excinfo;
extern int miniDumpResultCode;

static void DisplayException()
{
    const auto &sc_error = cc_get_error();
    snprintf(printfworkingspace, PRINT_WORKSPACE_SIZE, "An exception 0x%X occurred in ACWIN.EXE at EIP = 0x%0*" PRIXPTR "; program pointer is %+d, engine version %s, gtags (%d,%d)\n\n"
        "AGS cannot continue, this exception was fatal. Please note down the numbers above, remember what you were doing at the time and contact the game author for support "
        "or post these details on the AGS Technical Forum.\n\n%s\n\n"
        "Most versions of Windows allow you to press Ctrl+C now to copy this entire message to the clipboard for easy reporting.\n\n%s (code %d)",
        excinfo.ExceptionCode, (int)sizeof(intptr_t) * 2, (intptr_t)excinfo.ExceptionAddress, get_our_eip(), EngineVersion.LongString.GetCStr(), eip_guinum, eip_guiobj, sc_error.CallStack.GetCStr(),
        (miniDumpResultCode == 0) ? "An error file CrashInfo.dmp has been created. You may be asked to upload this file when reporting this problem on the AGS Forums." :
        "Unable to create an error dump file.", miniDumpResultCode);
    MessageBoxA((HWND)sys_win_get_window(), printfworkingspace, "Illegal exception", MB_ICONSTOP | MB_OK);
}

int initialize_engine_with_exception_handling(
    int (initialize_engine)(const AGS::Common::ConfigTree &startup_opts),
    const ConfigTree &startup_opts)
{
    __try
    {
        Debug::Printf(kDbgMsg_Info, "Installing exception handler");
        return initialize_engine(startup_opts);
    }
    __except (CustomExceptionHandler(GetExceptionInformation()))
    {
        DisplayException();
        proper_exit = 1;
    }
    return EXIT_CRASH;
}
#endif // USE_CUSTOM_EXCEPTION_HANDLER


int malloc_fail_handler(size_t amountwanted)
{
    char *cur = tempmsg;
    char const* const end = tempmsg + sizeof tempmsg;
    const size_t MB = 1024*1024;
    const size_t KB = 1024;
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    CreateMiniDump(NULL);
#endif
    free(printfworkingspace);

    cur += snprintf(cur, end-cur, "Out of memory: failed to allocate %zu bytes (at PP=%d)\n\n", amountwanted,
                    get_our_eip());

    const size_t total_normspr = spriteset.GetCacheSize();
    const size_t total_lockspr = spriteset.GetLockedSize();
    const size_t total_extspr = spriteset.GetExternalSize();
    const size_t max_normspr = spriteset.GetMaxCacheSize();
    const unsigned norm_spr_filled = max_normspr > 0 ?  ((uint64_t)total_normspr * 100 / max_normspr) : 0;

    cur += snprintf(cur, end-cur, "Sprite cache KB: %zu / %zu (%u%%), locked: %zu, ext: %zu\n\n",
                    total_normspr/KB, max_normspr/KB, norm_spr_filled, total_lockspr/KB, total_extspr/KB);


    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        cur += snprintf(cur, end - cur, "WorkingSetSize: %zu MB\n", (size_t)pmc.WorkingSetSize / MB);
        cur += snprintf(cur, end - cur, "QuotaPagedPoolUsage: %zu MB (peak %zu MB)\n", (size_t)pmc.QuotaPagedPoolUsage / MB, (size_t)pmc.QuotaPeakPagedPoolUsage / MB);
        cur += snprintf(cur, end - cur, "QuotaNonPagedPoolUsage: %zu MB (peak %zu MB)\n", (size_t)pmc.QuotaNonPagedPoolUsage / MB, (size_t)pmc.QuotaPeakNonPagedPoolUsage / MB);
        cur += snprintf(cur, end - cur, "PagefileUsage: %zu MB (peak %zu MB)\n\n", (size_t)pmc.PagefileUsage / MB, (size_t)pmc.PeakPagefileUsage / MB);
    }

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof statex;
    if (GlobalMemoryStatusEx(&statex)) {
        cur += snprintf(cur, end-cur, "%lu%% memory in use\n", statex.dwMemoryLoad);
        cur += snprintf(cur, end-cur, "Physical Memory: %llu MB free / %llu MB total\n", statex.ullAvailPhys/MB, statex.ullTotalPhys/MB);
        cur += snprintf(cur, end-cur, "Virtual Memory: %llu MB free / %llu MB total\n", statex.ullAvailVirtual/MB, statex.ullTotalVirtual/MB);
        cur += snprintf(cur, end-cur, "Extended Memory: %llu MB free\n", statex.ullAvailExtendedVirtual/MB);
    }


    quit(tempmsg);
    return 0;
}

void setup_malloc_handling()
{
    _set_new_handler(malloc_fail_handler);
    _set_new_mode(1);
    printfworkingspace = (char*)malloc(PRINT_WORKSPACE_SIZE);
}

#endif // AGS_PLATFORM_OS_WINDOWS
