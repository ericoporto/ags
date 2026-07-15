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
#ifndef __AGS_TOOL_DATA__DATAFILEWRITER_H
#define __AGS_TOOL_DATA__DATAFILEWRITER_H

#include <vector>
#include "util/string.h"
#include "util/stream.h"
#include "data/game_utils.h"

namespace AGS {
namespace DataUtil {

using AGS::Common::String;
using AGS::Common::Stream;

// Serializes the game data to the game28.dta format.
class DataFileWriter
{
public:
    bool WriteGame28(const GameData &game, Stream *out, String &error);

private:
    void WriteString(Stream *out, const String &text);
    void WriteFixedString(Stream *out, const String &text, size_t length);

    void WriteHeader(const GameData &game, Stream *out);
    void WriteFonts(const GameData &game, Stream *out);
    void WriteInventory(const GameData &game, Stream *out);
    void WriteViews(const GameData &game, Stream *out);
    void WriteGUIs(const GameData &game, Stream *out);
};

} // namespace DataUtil
} // namespace AGS

#endif // __AGS_TOOL_DATA__DATAFILEWRITER_H
