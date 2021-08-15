#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <alfont.h>
#include "gfx/allegrobitmap.h"
#include "util/string.h"
#include "util/path.h"
#include "util/cmdlineopts.h"
#include "core/def_version.h"

using namespace AGS::Common;
using namespace AGS::Common::CmdLineOpts;

const char *HELP_STRING = R"EOS(Usage: visualtest [options] <OUTPUT.bmp>
-f <f.ttf>, --font <f.ttf>   Sets font to use
-t <str>, --text <str>       Sets text to write
-s <16>, --size <16>         Sets text size
-h, --help                   Print this usage message
)EOS";

struct ParsedOptions {
    String Font;
    String Text;
    int FontSize;
    String Output;
    bool Exit = false;
    int ErrorCode = 0;
    ParsedOptions() = default;
    explicit ParsedOptions(int error_code) { Exit = true; ErrorCode = error_code; }
    void dump() {
        printf("Font: %s\nText: %s\nFontSize: %d\nOutput: %s\n", Font.GetCStr(), Text.GetCStr(), FontSize, Output.GetCStr());
    }
};


ParsedOptions parser_to_vt_opts(const ParseResult& parseResult)
{
    String Font;
    int TextSize = 16;
    String Text = "This is a test";

    if(parseResult.HelpRequested) {
        printf("%s", HELP_STRING);
        return ParsedOptions(0); // display help and bail out
    }

    if(parseResult.PosArgs.size() < 1) {
        std::cerr << "Error: not enough arguments" << std::endl;
        printf("%s", HELP_STRING);
        return ParsedOptions(-1);
    }

    for(const auto& opt_with_value : parseResult.OptWithValue)
    {
        if(opt_with_value.first == "-f" || opt_with_value.first == "--font")
        {
            Font = opt_with_value.second;
            continue;
        }

        if(opt_with_value.first == "-s" || opt_with_value.first == "--size")
        {
            TextSize = opt_with_value.second.ToInt();
            continue;
        }

        if(opt_with_value.first == "-t" || opt_with_value.first == "--text")
        {
            Text = opt_with_value.second;
            continue;
        }
    }

    ParsedOptions parsedOptions;
    parsedOptions.Output = parseResult.PosArgs[0];
    parsedOptions.Text = Text;
    parsedOptions.FontSize = TextSize;
    parsedOptions.Font = Font;
    parsedOptions.dump();
    return parsedOptions;
}

int main(int argc, char *argv[])
{
    printf(R"EOS(visualtest v0.1.0 - A tester for drawing with AGS primitives
Copyright (c) 2021 AGS Team and contributors
    )EOS"
    );

    ParseResult parseResult = Parse(argc,argv,{"-f", "--font"});
    ParsedOptions parsedOptions = parser_to_vt_opts(parseResult);

    if(parsedOptions.Exit) return parsedOptions.ErrorCode;

    if (install_allegro(SYSTEM_NONE, &errno, atexit))
    {
        std::cerr << "ERROR: unable to initialize stripped Allegro 4 library." << std::endl;
        return -1;
    }
    alfont_init();
    alfont_text_mode(-1);

    String Font = parsedOptions.Font;
    ALFONT_FONT* alfontFont = alfont_load_font(Font.GetCStr());


    alfont_set_font_size(alfontFont, parsedOptions.FontSize);

    Bitmap bmp = Bitmap(320, 240, 32);
    bmp.Fill(320);
    alfont_textout(bmp.GetAllegroBitmap(), alfontFont, parsedOptions.Text.GetCStr(), 8, 8, 65535);
    alfont_destroy_font(alfontFont);

    String output_fp = Path::MakeAbsolutePath(parsedOptions.Output);

    if(!bmp.SaveToFile(output_fp, nullptr)){
        std::cerr << "ERROR: could not save to file: " << output_fp.GetCStr() << std::endl;
        return -1;
    }

    alfont_exit();
}