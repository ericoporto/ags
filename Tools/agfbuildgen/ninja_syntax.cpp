
#include <util/filestream.h>
#include "agfbuildgen/ninja_syntax.h"
#include "util/memory_compat.h"
using namespace AGS::Common;

NinjaWriter::NinjaWriter(OutputWriterInterface* output, int width) {
    _output = output;
    _width = width;
}

void NinjaWriter::Newline() {
    _output->Write("\n");
}

void NinjaWriter::Comment(const String& text) {

}

void NinjaWriter::Variable(const String& key, const String& value, int indent) {

}

void NinjaWriter::Pool(const String& name, int depth) {

}

void NinjaWriter::Rule(const String& name, const String& command, const String& description, const String& depfile, bool generator,
                       const String& pool, bool restat, const String& rspfile, const String& rspfile_content, const String& deps) {

}

void NinjaWriter::Build(const std::vector<AGString>& outputs, const AGString& rule, const std::vector<AGString>& inputs,
                        const std::vector<AGString>& implicit, const std::vector<AGString>& order_only,
                        const std::vector<AGString>& variables, const std::vector<AGString>& implicit_outputs, const AGString& pool,
                        const AGString& dyndep) {

}

void NinjaWriter::Include(const AGString &path) {

}

void NinjaWriter::Subninja(const AGString &path) {

}

void NinjaWriter::Default(const std::vector<AGString> &paths) {

}

void NinjaWriter::Close() {
    _output->Close();
}


void OutputFileWriter::Write(const AGString &text) {
    text.Write(_fstream.get());
}

void OutputFileWriter::Close() {
    _fstream->Close();
}

OutputFileWriter::OutputFileWriter(const AGString &filename) {
    _fstream = std::make_unique <FileStream>(FileStream(filename,FileOpenMode::kFile_CreateAlways,FileWorkMode::kFile_Write));
}

OutputFileWriter::~OutputFileWriter() {
    if(_fstream != nullptr) _fstream->Close();
}

