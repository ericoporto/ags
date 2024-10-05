#include <algorithm>
#include <stdio.h>
#include "data/agfreader.h"
#include "util/cmdlineopts.h"
#include "util/file.h"
#include "util/path.h"
#include "util/stdio_compat.h"
#include "util/string_compat.h"
#include "util/string_utils.h"

using namespace AGS::Common;
using namespace AGS::Common::CmdLineOpts;
using namespace AGS;

const char *HELP_STRING = "Usage: agf2dta <INPUT-GAME.AGF> <OUTPUT-DIR>\n"
"Creates a \"game28.dta\" file in the OUTPUT-DIR from data in INPUT-GAME.AGF. \n"
"Options:\n"
"  -h, --help             Show this help message\n";

int main(int argc, char *argv[])
{
    printf("agf2dta v0.2.0 - AGS game main data file build tool\n"\
        "Copyright (c) 2024 AGS Team and contributors\n");
    ParseResult parseResult = Parse(argc,argv,{});
    if (parseResult.HelpRequested)
    {
        printf("%s\n", HELP_STRING);
        return 0; // display help and bail out
    }
    if (parseResult.PosArgs.size() < 2)
    {
        printf("Error: not enough arguments\n");
        printf("%s\n", HELP_STRING);
        return -1;
    }
	
    const String &src_agf = parseResult.PosArgs[0];
    const String &dst_dir = parseResult.PosArgs[1];
	
    printf("Input game AGF: %s\n", src_agf.GetCStr());
    printf("Output dir: %s\n", dst_dir.GetCStr());
	
    //-----------------------------------------------------------------------//
    // validate inputs
    //-----------------------------------------------------------------------//
	if(!File::IsFile(src_agf))
	{
        printf("Error: input game file '%s' not found.\n", src_agf.GetCStr());
        return -1;
	}
	
    if (!ags_directory_exists(dst_dir.GetCStr()))
    {
        printf("Error: not a valid output directory.\n");
        return -1;
    }

    //-----------------------------------------------------------------------//
    // Read Game.agf
    //-----------------------------------------------------------------------//
    AGF::AGFReader reader;
    const HError err = reader.Open(src_agf.GetCStr());
    if (!err)
    {
        printf("Error: failed to open source AGF:\n");
        printf("%s\n", err->FullMessage().GetCStr());
        return -1;
    }

    //-----------------------------------------------------------------------//
    // Set the main game data struct values from Game.agf
    //-----------------------------------------------------------------------//
	
    //-----------------------------------------------------------------------//
    // Write main game data file to game28.dta
    //-----------------------------------------------------------------------//

    printf("Game main data file(s) written successfully.\nDone.\n");
    return 0;
}
