//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2026 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#ifndef __AGS_TOOL_DATA__LOGUTILS_H
#define __AGS_TOOL_DATA__LOGUTILS_H

#include "util/string.h"

namespace AGS
{
namespace DataUtil
{
	
using AGS::Common::String;

void InitToolOutput(const String& ToolName);

inline void PrintToStdOut(AGS_FORMAT_STRING_ARG const char* fmt, ...) AGS_FORMAT_STRING(1);

void LogInfo(const String& msg);
void LogInfo(AGS_FORMAT_STRING_ARG const char* fmt, ...) AGS_FORMAT_STRING(1);
void LogInfo(AGS_FORMAT_STRING_ARG const char* fmt, va_list argptr) AGS_FORMAT_STRING_V(1);

void LogWarn(const String& msg);
void LogWarn(AGS_FORMAT_STRING_ARG const char* fmt, ...) AGS_FORMAT_STRING(1);
void LogWarn(AGS_FORMAT_STRING_ARG const char* fmt, va_list argptr) AGS_FORMAT_STRING_V(1);

void LogError(const String& msg);
void LogError(AGS_FORMAT_STRING_ARG const char* fmt, ...) AGS_FORMAT_STRING(1);
void LogError(AGS_FORMAT_STRING_ARG const char* fmt, va_list argptr) AGS_FORMAT_STRING_V(1);

} // namespace DataUtil
} // namespace AGS


#endif // __AGS_TOOL_DATA__LOGUTILS_H
