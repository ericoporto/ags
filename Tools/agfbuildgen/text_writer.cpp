// the idea of having the other writers take this is that we can make a small version that writes a string so we can write tests

#include "text_writer.h"

void OutputFileWriter::Write(const String &text)
{
    assert(_fstream);
    _fstream->Write(text.GetCStr(), text.GetLength());
    _fstream->Flush();
}

OutputFileWriter::OutputFileWriter(const String &filename)
{
    _fstream = File::OpenFile(filename, kFile_CreateAlways, kStream_Write);
    if (!_fstream)
    {
        printf("Unable to open for writing '%s'.", filename.GetCStr());
        return;
    }
}

void OutputFileWriter::Close() {
    _fstream->Flush();
    _fstream->Close();
    _fstream.reset();
}

// -- Writer Utils

String WriterUtils::Join(const String& sep, const std::vector<String>& words)
{
    if (words.empty())
        return String();
    if (words.size() == 1)
        return words[0];

    String joined;
    bool first = true;
    for (const auto& word : words)
    {
        if (!first) joined.Append(sep);
        joined.Append(word);
        first = false;
    }
    return joined;
}

String WriterUtils::Join(const String& sep, const String& plus_one, const std::vector<String>& words)
{
    String joined = plus_one;
    joined.Append(sep);
    joined.Append(Join(sep, words));
    return joined;
}

// -- Writer

// refactor to receive the OutputFileWriter
Writer::Writer(const String &filename) {
    _output.reset(new OutputFileWriter(filename));
}



void Writer::Write(const String &text) {
    _output->Write(text);
}

void Writer::Line(const String& text, int indent)
{
    if(indent > 0)
    {
        String leading_space;
        leading_space.FillString(_indent_char, indent * _indent_multiplier);
        Write(leading_space);
    }
    Write(text);
    Newline();
}

void Writer::Newline()
{
    Write("\n");
}

void Writer::Close() {
    _output->Close();
}

