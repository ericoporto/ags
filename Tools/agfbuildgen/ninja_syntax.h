//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
// Ninja Syntax for build generators
//
//=============================================================================
#ifndef __AGS_TOOL_AGFBUILDGEN__NINJA_SYNTAX_H
#define __AGS_TOOL_AGFBUILDGEN__NINJA_SYNTAX_H

#include <vector>
#include <memory>
#include "util/string.h"
#include "util/filestream.h"
typedef AGS::Common::String AGString;

class OutputWriterInterface {
public:
    virtual void Write(const AGString& text) = 0;
    virtual void Close() = 0;
};

class OutputFileWriter: public OutputWriterInterface {
    std::unique_ptr<AGS::Common::FileStream> _fstream;
public:
    explicit OutputFileWriter(const AGString& filename);
    ~OutputFileWriter();
    void Write(const AGString& text) override;
    void Close() override;
};

class NinjaWriter {
private:
    OutputWriterInterface* _output;
    int _width;
    void Line(const AGString& text, int indent=0);
public:
    explicit NinjaWriter(OutputWriterInterface* output, int width=78);
    ~NinjaWriter() = default;
    void Newline();
    void Comment(const AGString& text);
    void Variable(const AGString& key, const AGString& value, int indent=0);
    void Pool(const AGString& name, int depth);
    void Rule(const AGString& name, const AGString& command, const AGString& description="", const AGString& depfile="",
              bool generator=false, const AGString& pool="", bool restat=false, const AGString& rspfile="",
              const AGString& rspfile_content="", const AGString& deps="");
    void Build(const std::vector<AGString>& outputs, const AGString& rule, const std::vector<AGString>& inputs={}, const std::vector<AGString>& implicit={}, const std::vector<AGString>& order_only={},
               const std::vector<std::pair<AGString, AGString>>& variables={}, const std::vector<AGString>& implicit_outputs={}, const AGString& pool="", const AGString& dyndep="");
    void Include(const AGString& path);
    void Subninja(const AGString& path);
    void Default(const std::vector<AGString>& paths);
    void Close();
};


#endif // __AGS_TOOL_AGFBUILDGEN__NINJA_SYNTAX_H