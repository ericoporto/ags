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

void LogInfo(const String& msg);
void LogInfo(const char* fmt, ...);
void LogInfo(const char* fmt, va_list argptr);

void LogWarn(const String& msg);
void LogWarn(const char* fmt, ...);
void LogWarn(const char* fmt, va_list argptr);

void LogError(const String& msg);
void LogError(const char* fmt, ...);
void LogError(const char* fmt, va_list argptr);

} // namespace DataUtil
} // namespace AGS


#endif // __AGS_TOOL_DATA__LOGUTILS_H
