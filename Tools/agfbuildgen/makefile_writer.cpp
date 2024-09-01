#include "makefile_writer.h"

using namespace AGS::Common;
using namespace WriterUtils;

MakefileWriter::MakefileWriter(const String &filename) : Writer(filename)
{
    _indent_char = '\t';
    _indent_multiplier = 1;
}

void MakefileWriter::Comment(const String& text)
{
    Write("# ");
    Write(text);
    Newline();
}

void MakefileWriter::Variable(const String& key, const String& value)
{
    if (!value.IsNullOrSpace())
    {
        Line(String::FromFormat("%s = %s", key.GetCStr(), value.GetCStr()), 0);
    }
}

void MakefileWriter::Rule(const String& target, const std::vector<String>& dependencies, const String& command)
{
    Line(String::FromFormat("%s: %s", target.GetCStr(), Join(" ", dependencies).GetCStr()));
    Line(command, 1);
}

void MakefileWriter::Phony(const String& target)
{
    Line(String::FromFormat(".PHONY: %s", target.GetCStr()));
}

