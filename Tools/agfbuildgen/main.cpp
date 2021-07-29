#include <string>
#include <map>
#include <iostream>
#include "util/path.h"
#include "util/cmdlineopts.h"
#include "data/agfreader.h"

using namespace AGS::Common;
using namespace AGS::Common::CmdLineOpts;
using namespace AGS::DataUtil;
namespace AGF = AGS::AGF;

const char *HELP_STRING = R"EOS(Usage: agfbuildgen [options] <Game.agf>
Generates a build.ninja from Game.agf and places it in OutputDir directory
-o, --output <FILE>          Outputs to file
--output-dir <DIR>           Outputs to directory with default generator name
-g, --gen <ninja/bat>        Changes the generator              (default:ninja)
-h, --help                   Print this usage message
)EOS";

enum Generator {
    kNinja,
    kBatch
};

struct ParsedOptions {
    Generator Gen = Generator::kNinja;
    String GameAgf;
    String OutputDir {};
    String OutputFile {};
    bool OutputStdout = false;
    bool Exit = false;
    int ErrorCode = 0;
    ParsedOptions() = default;
    explicit ParsedOptions(int error_code) { Exit = true; ErrorCode = error_code; }
};

ParsedOptions parser_to_gen_opts(const ParseResult& parseResult)
{
    if(parseResult.HelpRequested) {
        printf("%s", HELP_STRING);
        return ParsedOptions(0); // display help and bail out
    }

    if(parseResult.PosArgs.empty()) {
        std::cerr << "Error: not enough arguments" << std::endl;
        printf("%s", HELP_STRING);
        return ParsedOptions(-1);
    }

    ParsedOptions parsedOptions;

    for(const auto& opt_with_value : parseResult.OptWithValue)
    {
        if(opt_with_value.first == "-g" || opt_with_value.first == "--gen")
        {
            if(opt_with_value.second == "bat") parsedOptions.Gen = Generator::kBatch;
            else if(opt_with_value.second == "ninja") parsedOptions.Gen = Generator::kNinja;
        }

        if(opt_with_value.first == "-o" || opt_with_value.first == "--ouput")
        {
            parsedOptions.OutputFile = opt_with_value.second;
        }

        if(opt_with_value.first == "--ouput-dir")
        {
            parsedOptions.OutputDir = opt_with_value.second;
        }
    }

    parsedOptions.GameAgf = parseResult.PosArgs[0];

    if(!parsedOptions.OutputDir.IsNullOrSpace()) {
        switch (parsedOptions.Gen) {
            case kNinja:
                parsedOptions.OutputFile = Path::ConcatPaths( parsedOptions.OutputDir, "build.ninja");
                break;
            case kBatch:
                parsedOptions.OutputFile = Path::ConcatPaths( parsedOptions.OutputDir, "build.bat");
                break;
        }
    }

    if(parsedOptions.OutputFile.IsNullOrSpace()) parsedOptions.OutputStdout = true;

    return parsedOptions;
}

int main(int argc, char* argv[])
{
    printf(R"EOS(agfbuildgen v0.1.0 - A Build Generator for AGF Project Files
Copyright (c) 2021 AGS Team and contributors
    )EOS"
    );

    ParseResult parseResult = Parse(argc,argv,{"-g", "--gen"});
    ParsedOptions parsedOptions = parser_to_gen_opts(parseResult);

    if(parsedOptions.Exit) return parsedOptions.ErrorCode;

    //-----------------------------------------------------------------------//
    // Read Game.agf
    //-----------------------------------------------------------------------//
    AGF::AGFReader reader;
    HError err = reader.Open(parsedOptions.GameAgf.GetCStr());
    if (!err)
    {
        printf("Error: failed to open source AGF:\n");
        printf("%s\n", err->FullMessage().GetCStr());
        return -1;
    }

    std::vector<String> scripts;
    AGF::ReadScriptList(scripts, reader.GetGameRoot());


    std::vector<std::pair<int, String>> rooms;
    AGF::ReadRoomList(rooms, reader.GetGameRoot());


    return 0;
}