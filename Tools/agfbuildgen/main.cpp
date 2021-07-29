#include <string>
#include <map>
#include <iostream>
#include "util/path.h"
#include "util/cmdlineopts.h"

using namespace AGS::Common;
using namespace AGS::Common::CmdLineOpts;

const char *HELP_STRING = R"EOS(Usage: agfbuildgen [options] <Game.agf> <OutputDir>
Generates a build.ninja from Game.agf and places it in OutputDir directory
-g, --gen <ninja/bat>        Changes the generator              (default:ninja)
-h, --help                   Print this usage message
)EOS";

enum Generator {
    kNinja,
    kBatch
};

struct ParsedOptions {
    Generator gen = Generator::kNinja;
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

    if(parseResult.PosArgs.size() < 2) {
        std::cerr << "Error: not enough arguments" << std::endl;
        printf("%s", HELP_STRING);
        return ParsedOptions(-1);
    }

    ParsedOptions parsedOptions;

    for(const auto& opt_with_value : parseResult.OptWithValue)
    {
        if(opt_with_value.first == "-g" || opt_with_value.first == "--gen")
        {
            if(opt_with_value.second == "bat") parsedOptions.gen = Generator::kBatch;
            else if(opt_with_value.second == "ninja") parsedOptions.gen = Generator::kNinja;
        }
    }
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


    return 0;
}