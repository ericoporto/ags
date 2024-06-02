//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2024 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "ac/game_version.h"
#include "script/cc_common.h"
#include "script/runtimescriptvalue.h"
#include "script/script_api.h"

using namespace AGS::Common;

enum FormatParseResult
{
    kFormatParseNone,
    kFormatParseInvalid,
    kFormatParseLiteralPercent,
    kFormatParseArgInteger,
    kFormatParseArgFloat,
    kFormatParseArgCharacter,
    kFormatParseArgString,
    kFormatParseArgPointer,

    kFormatParseArgFirst = kFormatParseArgInteger,
    kFormatParseArgLast = kFormatParseArgPointer
};

// Helper functions for getting parameter value either from script val array or va_list
inline int GetArgInt(const RuntimeScriptValue *sc_args, va_list *varg_ptr, int arg_idx)
{
    if (varg_ptr)
        return va_arg(*varg_ptr, int);
    else
        return sc_args[arg_idx].IValue;
}

inline float GetArgFloat(const RuntimeScriptValue *sc_args, va_list *varg_ptr, int arg_idx)
{
    // note that script variables store only floats, but va_list has floats promoted to double
    if (varg_ptr)
        return (float)va_arg(*varg_ptr, double);
    else
        return sc_args[arg_idx].FValue;
}

inline const char *GetArgPtr(const RuntimeScriptValue *sc_args, va_list *varg_ptr, int arg_idx)
{
    if (varg_ptr)
        return va_arg(*varg_ptr, const char*);
    else
        return reinterpret_cast<const char*>(sc_args[arg_idx].Ptr);
}


int ScriptSprintfImpl(char *buffer, size_t buf_length, const char *format, const RuntimeScriptValue *sc_args, int32_t sc_argc, va_list *varg_ptr) {
// Expected format character count:
// percent sign:    1
// flag:            1
// field width      10 (an uint32 number)
// precision sign   1
// precision        10 (an uint32 number)
// length modifier  2
// type             1
// NOTE: although width and precision will
// not likely be defined by a 10-digit
// number, such case is theoretically valid.
    const size_t fmtbuf_size = 27;
    char       fmtbuf[fmtbuf_size];
    char       *fmt_bufptr;
    char       *fmt_bufendptr = &fmtbuf[fmtbuf_size - 1];

    size_t total_length = 0;
    bool calculate_length_only = (buffer == nullptr);
    char dummy_buffer[1] = {0};
    char       *out_ptr    = calculate_length_only ? dummy_buffer : buffer;
    // save 1 character for null terminator
    const char *out_endptr = calculate_length_only ? dummy_buffer : (buffer + buf_length - 1);
    const char *fmt_ptr    = format;
    int32_t    arg_idx     = 0;

    ptrdiff_t  avail_outbuf;
    int        snprintf_res;
    FormatParseResult fmt_done;

    // Parse the format string, looking for argument placeholders
    while (*fmt_ptr && (calculate_length_only || out_ptr != out_endptr))
    {
        // Try to put argument into placeholder
        if (*fmt_ptr == '%')
        {
            avail_outbuf = out_endptr - out_ptr;
            fmt_bufptr = fmtbuf;
            *(fmt_bufptr++) = '%';
            snprintf_res = 0;
            fmt_done = kFormatParseNone;

            // Parse placeholder
            while (*(++fmt_ptr) && fmt_done == kFormatParseNone && fmt_bufptr != fmt_bufendptr)
            {
                *(fmt_bufptr++) = *fmt_ptr;
                switch (*fmt_ptr)
                {
                case 'd':
                case 'i':
                case 'o':
                case 'u':
                case 'x':
                case 'X':
                    fmt_done = kFormatParseArgInteger;
                    break;
                case 'c':
                    fmt_done = kFormatParseArgCharacter;
                    break;
                case 'e':
                case 'E':
                case 'f':
                case 'F':
                case 'g':
                case 'G':
                case 'a':
                case 'A':
                    fmt_done = kFormatParseArgFloat;
                    break;
                case 'p':
                    fmt_done = kFormatParseArgPointer;
                    break;
                case 's':
                    fmt_done = kFormatParseArgString;
                    break;
                case '%':
                    // This may be a literal percent sign ('%%')
                    if (fmt_bufptr - fmtbuf == 2)
                    {
                        fmt_done = kFormatParseLiteralPercent;
                    }
                    // ...Otherwise we reached the next placeholder
                    else
                    {
                        fmt_ptr--;
                        fmt_bufptr--;
                        fmt_done = kFormatParseInvalid;
                    }
                    break;
                }
            }

            // Deal with the placeholder parsing results
            if (fmt_done == kFormatParseLiteralPercent)
            {
                // literal percent sign
                if (!calculate_length_only)
                {
                    *(out_ptr++) = '%';
                }
                total_length++;
                continue;
            }
            else if (fmt_done >= kFormatParseArgFirst && fmt_done <= kFormatParseArgLast &&
                (varg_ptr || arg_idx < sc_argc))
            {
                // Print the actual value
                // NOTE: snprintf is called with avail_outbuf + 1 here, because we let it use our reserved
                // character for null-terminator, in case we are at the end of the buffer
                *fmt_bufptr = 0; // terminate the format buffer, we are going to use it
                switch (fmt_done)
                {
                case kFormatParseArgInteger:
                    snprintf_res = snprintf(calculate_length_only ? nullptr : out_ptr, calculate_length_only ? 0 : (avail_outbuf + 1), fmtbuf, GetArgInt(sc_args, varg_ptr, arg_idx)); break;
                case kFormatParseArgFloat:
                    snprintf_res = snprintf(calculate_length_only ? nullptr : out_ptr, calculate_length_only ? 0 : (avail_outbuf + 1), fmtbuf, GetArgFloat(sc_args, varg_ptr, arg_idx)); break;
                case kFormatParseArgCharacter:
                {
                    int chr = GetArgInt(sc_args, varg_ptr, arg_idx);
                    char cbuf[5]{};
                    usetc(cbuf, chr);
                    snprintf_res = snprintf(calculate_length_only ? nullptr : out_ptr, calculate_length_only ? 0 : (avail_outbuf + 1), "%s", cbuf);
                    break;
                }
                case kFormatParseArgString:
                {
                    const char *p = GetArgPtr(sc_args, varg_ptr, arg_idx);
                    // Do extra checks for %s placeholder
                    if (fmt_done == kFormatParseArgString && !p)
                    {
                        if (loaded_game_file_version < kGameVersion_320)
                        {
                            // explicitly put "(null)" into the placeholder
                            p = "(null)";
                        }
                        else
                        {
                            cc_error("!ScriptSprintf: formatting argument %d is expected to be a string, but it is a null pointer", arg_idx + 1);
                            return -1;
                        }
                    }
                    else if (fmt_done == kFormatParseArgString && p == buffer)
                    {
                        cc_error("!ScriptSprintf: formatting argument %d is a pointer to output buffer", arg_idx + 1);
                        return -1;
                    }
                    snprintf_res = snprintf(calculate_length_only ? nullptr : out_ptr, calculate_length_only ? 0 : (avail_outbuf + 1), fmtbuf, p);
                    break;
                }
                case kFormatParseArgPointer:
                    snprintf_res = snprintf(calculate_length_only ? nullptr : out_ptr, calculate_length_only ? 0 : (avail_outbuf + 1), fmtbuf, GetArgPtr(sc_args, varg_ptr, arg_idx)); break;
                    default: /* should not happen */ break;
                }

                arg_idx++;
                if (snprintf_res >= 0)
                {
                    int added_length = snprintf_res;
                    if(!calculate_length_only)
                    {
                        // snprintf returns maximal number of characters, so limit it with buffer size
                        added_length = std::min<ptrdiff_t>(added_length, avail_outbuf);
                        out_ptr += added_length;
                    }
                    total_length += added_length;
                    continue;
                }
                // -- pass further to invalid format case
            }

            // If format was not valid, or there are no available
            // parameters, just copy stored format buffer as it is
            ptrdiff_t remaining_size_arg = std::min<ptrdiff_t>(fmt_bufptr - fmtbuf, fmtbuf_size - 1);
            size_t copy_len = static_cast<size_t>(remaining_size_arg);
            if (!calculate_length_only)
            {
                copy_len = std::min(remaining_size_arg, avail_outbuf);
                memcpy(out_ptr, fmtbuf, copy_len);
                out_ptr += copy_len;
            }
            total_length += copy_len;
        }
        // If there's no placeholder, simply copy the character to output buffer
        else
        {
            if (!calculate_length_only)
            {
                *(out_ptr++) = *(fmt_ptr++);
            }
            else
            {
                fmt_ptr++;
            }
            total_length++;
        }
    }
    total_length++; // space for null character
    // Terminate the string
    if (!calculate_length_only) {
        *out_ptr = 0;
    }
    return total_length;
}

// TODO: this implementation can be further optimised by either not calling
// snprintf but formatting values ourselves, or by using some library method
// that supports customizing, such as getting arguments in a custom way.
const char *ScriptSprintf(char *buffer, size_t buf_length, const char *format,
                          const RuntimeScriptValue *sc_args, int32_t sc_argc, va_list *varg_ptr)
{
    // Fix-me: Not sure what to do here yet, perhaps moving the internals to an impl and leaving this on outside?
    if (!buffer || buf_length == 0)
    {
        cc_error("Internal error in ScriptSprintf: buffer is null");
        return "";
    }
    if (!format)
    {// NOTE: interpreter (usually) catches null-pointer sent as format at some stage earlier
        cc_error("Internal error in ScriptSprintf: format string is null");
        return "";
    }
    if (!varg_ptr && sc_argc > 0 && !sc_args)
    {
        cc_error("Internal error in ScriptSprintf: args pointer is null");
        return "";
    }
    int buffer_length = ScriptSprintfImpl(nullptr, 0, format, sc_args, sc_argc, varg_ptr);
    if(buffer_length < 0) {
        return "";
    }

    char *final_buffer = new char[buffer_length];
    ScriptSprintfImpl(final_buffer, buffer_length, format, sc_args, sc_argc, varg_ptr);
    return final_buffer;
}
