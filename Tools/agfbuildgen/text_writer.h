//
// Created by erico on 9/6/2024.
//

#ifndef AGS_TOOL_AGFBUILDGEN__TEXT_WRITER_H
#define AGS_TOOL_AGFBUILDGEN__TEXT_WRITER_H

#include "util/filestream.h"
#include "util/string.h"

using namespace AGS::Common;

class OutputWriterInterface {
public:
    virtual void Write(const String& text) = 0;
    virtual ~OutputWriterInterface() = default;
    virtual void Close() = 0;
};

class OutputFileWriter: public OutputWriterInterface {
private:
    std::unique_ptr<Stream> _fstream;
public:
    explicit OutputFileWriter(const String& filename);
    virtual ~OutputFileWriter() = default;
    void Write(const String& text) override;
    void Close() override;
};

namespace WriterUtils {
    String Join(const String &sep, const std::vector<String> &words);
    String Join(const String& sep, const String& plus_one, const std::vector<String>& words);
}

class Writer {
public:
    explicit Writer(const String& filename);
    virtual ~Writer() = default;
    void Close();

private:
    std::unique_ptr<OutputWriterInterface> _output;

protected:
    void Write(const String& text);
    void Newline();
    void Line(const String &text, int indent = 0);

    size_t _indent_multiplier = 2;
    char _indent_char = ' ';
};

#endif // AGS_TOOL_AGFBUILDGEN__TEXT_WRITER_H
