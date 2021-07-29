
#include <util/filestream.h>
#include "agfbuildgen/ninja_syntax.h"
#include "util/memory_compat.h"
using namespace AGS::Common;

String escape_path(const String& word)
{
    String escaped = word;
    escaped.Replace("$ ","$$ ");
    escaped.Replace(" ","$ ");
    escaped.Replace(":","$:");
    return escaped;
}

std::vector<String> escape_paths(const std::vector<String>& words)
{
    std::vector<String> escaped_paths;
    escaped_paths.reserve(words.size());
    for(const auto& word : words)
    {
        escaped_paths.emplace_back(escape_path(word));
    }
    return escaped_paths;
}

String join(const String& sep, const std::vector<String>& words)
{
    String joined;
    bool dosep = false;
    for(const auto& word : words)
    {
        if(dosep) joined.Append(sep);
        joined.Append(word);
        dosep = true;
    }
    return joined;
}

String join(const String& sep, const String& plus_one, const std::vector<String>& words)
{
    String joined = plus_one;
    joined.Append(sep);
    joined.Append(join(sep, words));
    return joined;
}


NinjaWriter::NinjaWriter(OutputWriterInterface* output, int width)
{
    _output = output;
    _width = width;
}

void NinjaWriter::Newline()
{
    _output->Write("\n");
}

void NinjaWriter::Comment(const String& text)
{
    // TODO: actually wrap lines
    _output->Write("# ");
    _output->Write(text);
    Newline();
}

void NinjaWriter::Variable(const String& key, const String& value, int indent) {
    if(value.IsNullOrSpace()) return;
    Line(String::FromFormat("%s = %s", key.GetCStr(), value.GetCStr()), indent);
}

void NinjaWriter::Pool(const String& name, int depth) {
    Line(String::FromFormat("pool %s", name.GetCStr()));
    Variable("depth",String::FromFormat("%d", depth), 1);
}

void NinjaWriter::Rule(const String& name, const String& command, const String& description, const String& depfile, bool generator,
                       const String& pool, bool restat, const String& rspfile, const String& rspfile_content, const String& deps) {
    Line(String::FromFormat("rule %s", name.GetCStr()));
    Variable("command", command, 1);
    if(!description.IsNullOrSpace()) Variable("description", description, 1);
    if(!depfile.IsNullOrSpace()) Variable("depfile", depfile, 1);
    if(generator) Variable("generator", "1", 1);
    if(!pool.IsNullOrSpace()) Variable("pool", pool, 1);
    if(restat) Variable("restat", "1", 1);
    if(!rspfile.IsNullOrSpace()) Variable("rspfile",rspfile,1);
    if(!rspfile_content.IsNullOrSpace()) Variable("rspfile_content", rspfile_content, 1);
    if(!deps.IsNullOrSpace()) Variable("deps",deps,1);
}

void NinjaWriter::Build(const std::vector<String>& outputs, const String& rule, const std::vector<String>& inputs,
                        const std::vector<String>& implicit, const std::vector<String>& order_only,
                        const std::vector<std::pair<String, String>>& variables, const std::vector<String>& implicit_outputs, const String& pool,
                        const String& dyndep)
{
    auto out_outputs = escape_paths(outputs);
    auto all_inputs = escape_paths(inputs);

    if(!implicit.empty()) {
        auto implicits = escape_paths(implicit);
        all_inputs.emplace_back("|");
        for(const auto& imp : implicits){
            all_inputs.emplace_back(imp);
        }
    }
    if(!order_only.empty()) {
        auto order_onlys = escape_paths(order_only);
        all_inputs.emplace_back("||");
        for(const auto& ord : order_onlys){
            all_inputs.emplace_back(ord);
        }
    }
    if(!implicit_outputs.empty()) {
        auto implicit_outs = escape_paths(implicit_outputs);
        out_outputs.emplace_back("|");
        for(const auto& impo : implicit_outs){
            all_inputs.emplace_back(impo);
        }
    }

    Line(String::FromFormat("build %s: %s", join(" ", out_outputs).GetCStr(), join(" ", rule,all_inputs).GetCStr()));

    if(!pool.IsNullOrSpace()) Line(String::FromFormat("  pool = %s", pool.GetCStr()));
    if(!dyndep.IsNullOrSpace()) Line(String::FromFormat("  dyndep = %s", dyndep.GetCStr()));

    if(!variables.empty())
    {
        for(const auto& var : variables)
        {
            Variable(var.first,var.second,1);
        }
    }
}

void NinjaWriter::Include(const AGString &path)
{
    Line(String::FromFormat("include %s",path.GetCStr()));
}

void NinjaWriter::Subninja(const AGString &path)
{
    Line(String::FromFormat("subninja %s",path.GetCStr()));
}

void NinjaWriter::Default(const std::vector<AGString> &paths)
{
    Line(String::FromFormat("default  %s",join(" ", paths).GetCStr()));
}

void NinjaWriter::Close()
{
    _output->Close();
}

void NinjaWriter::Line(const AGString &text, int indent)
{
    String leading_space;
    leading_space.FillString(' ', indent*2);
    _output->Write(leading_space);
    _output->Write(text);
    Newline();
}


void OutputFileWriter::Write(const AGString &text)
{
    text.Write(_fstream.get());
}

void OutputFileWriter::Close()
{
    _fstream->Close();
}

OutputFileWriter::OutputFileWriter(const AGString &filename)
{
    _fstream = std::make_unique <FileStream>(FileStream(filename,FileOpenMode::kFile_CreateAlways,FileWorkMode::kFile_Write));
}

OutputFileWriter::~OutputFileWriter()
{
    if(_fstream != nullptr) _fstream->Close();
}

