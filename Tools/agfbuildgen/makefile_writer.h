#ifndef AGS_TOOL_AGFBUILDGEN__MAKEFILE_WRITER_H
#define AGS_TOOL_AGFBUILDGEN__MAKEFILE_WRITER_H

#include <string>
#include <vector>
#include "util/string.h"
#include "util/filestream.h"
#include "text_writer.h"

using namespace AGS::Common;

class MakefileWriter: public Writer {
public:
    explicit MakefileWriter(const String &filename);
    void Comment(const String& text);
    void Variable(const String& key, const String& value);
    void Rule(const String& target, const std::vector<String>& dependencies, const String& command);
    void Phony(const String& target);
};

#endif // AGS_TOOL_AGFBUILDGEN__MAKEFILE_WRITER_H
