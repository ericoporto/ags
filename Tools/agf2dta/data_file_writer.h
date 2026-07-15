//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Temporary game28.dta serializer.
// Most of the actual block serialization lives in the .cpp as file-local
// helpers to keep the class surface small.
//
//=============================================================================
#ifndef AGS_COMMON_DATA_FILE_WRITER_H
#define AGS_COMMON_DATA_FILE_WRITER_H

#include <vector>
#include "util/string.h"
#include "util/stream.h"
#include "data/game_utils.h"

namespace AGS {
namespace DataUtil {

using AGS::Common::String;
using AGS::Common::Stream;

class DataFileWriter
{
public:
    bool WriteGame28(const GameRef &game, Stream *out, String &error);

private:
    void WriteString(Stream *out, const String &text);
    void WriteFixedString(Stream *out, const String &text, size_t length);

    void WriteHeader(const GameRef &game, Stream *out);
    void WriteFonts(const GameRef &game, Stream *out);
    void WriteInventory(const GameRef &game, Stream *out);
    void WriteViews(const GameRef &game, Stream *out);
    void WriteGUIs(const GameRef &game, Stream *out);
};

} // namespace DataUtil
} // namespace AGS

#endif
