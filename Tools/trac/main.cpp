#include <stdio.h>
#include "data/tra_utils.h"
#include "data/log_utils.h"
#include "util/cmdlineopts.h"
#include "util/file.h"
#include "util/path.h"
#include "util/string_utils.h"

using namespace AGS::Common;
using namespace AGS::Common::CmdLineOpts;
using namespace AGS::DataUtil;


//------------------------------------------------------------------------------|
static const char *HELP_STRING = "Usage:\n"
"  trac <input.trs> [<output.tra>] [--gamename <name>][--uniqueid <idnum>]\n"
"  trac -u <input.tra> [<output.trs>]\n";

static int Command_Compile(const String &src, const String &dst, const String *game_name, const int *game_uid)
{

    LogInfo("Input translation source: %s\n", src.GetCStr());
    LogInfo("Output compiled translation: %s\n", dst.GetCStr());
    if (game_name)
        LogInfo("Game name: %s\n", game_name->GetCStr());
    if (game_uid)
        LogInfo("Game uniqueid: %d\n", *game_uid);

    //-----------------------------------------------------------------------//
    // Read TRS
    //-----------------------------------------------------------------------//
    auto in = File::OpenFileRead(src);
    if (!in)
    {
        LogError("failed to open source TRS for reading.\n");
        return -1;
    }

    Translation tra;
    HError err = ReadTRS(tra, std::move(in));
    if (!err)
    {
        LogError("failed to read source TRS:\n");
        LogError("%s\n", err->FullMessage().GetCStr());
        return -1;
    }

    if (game_uid)
        tra.GameUid = *game_uid;
    if (game_name)
        tra.GameName = *game_name;

    //-----------------------------------------------------------------------//
    // Write TRA
    //-----------------------------------------------------------------------//
    auto out = File::CreateFile(dst);
    if (!out)
    {
        LogError("failed to open output TRA for writing.\n");
        return -1;
    }
    err = WriteTRA(tra, std::move(out));
    if (!err)
    {
        LogError("failed to compile TRA:\n");
        LogError("%s\n", err->FullMessage().GetCStr());
        return -1;
    }
    LogInfo("Compiled translation written successfully.\nDone.\n");
    return 0;
}

static int Command_Decompile(const String &src, const String &dst)
{
    LogInfo("Input compiled translation: %s\n", src.GetCStr());
    LogInfo("Output translation source: %s\n", dst.GetCStr());

    //-----------------------------------------------------------------------//
    // Read TRA
    //-----------------------------------------------------------------------//
    auto in = File::OpenFileRead(src);
    if (!in)
    {
        LogError("failed to open TRA for reading.\n");
        return -1;
    }

    Translation tra;
    HError err = ReadTraData(tra, std::move(in));
    if (!err)
    {
        LogError("failed to read input TRA:\n");
        LogError("%s\n", err->FullMessage().GetCStr());
        return -1;
    }

    //-----------------------------------------------------------------------//
    // Write TRS
    //-----------------------------------------------------------------------//
    auto out = File::CreateFile(dst);
    if (!out)
    {
        LogError("failed to open output TRS for writing.\n");
        return -1;
    }
    err = WriteTRS(tra, std::move(out));
    if (!err)
    {
        LogError("failed to write TRS:\n");
        LogError("%s\n", err->FullMessage().GetCStr());
        return -1;
    }

    LogInfo("Translation source written successfully.\nDone.\n");
    return 0;
}

int main(int argc, char *argv[])
{
    InitToolOutput("trac");
    LogInfo("trac v0.9.6 - AGS translation compiler and decompiler (TRS <-> TRA)\n"\
        "Copyright (c) 2021-2026 AGS Team and contributors\n");

    ParseResult cmdargs = Parse(argc, argv, {"--gamename", "--uniqueid"});
    if (cmdargs.HelpRequested)
    {
        LogInfo("%s\n", HELP_STRING);
        return 0; // display help and bail out
    }

    if (cmdargs.PosArgs.empty())
    {
        LogError("not enough arguments\n");
        LogInfo("%s\n", HELP_STRING);
        return -1;
    }

    String &src = cmdargs.PosArgs[0];

    // dst can be empty, if it is we will use a default value later
    String dst = "";
    if (cmdargs.PosArgs.size() > 1 && !cmdargs.PosArgs[1].IsEmpty())
        dst = cmdargs.PosArgs[1];

    if (cmdargs.Opt.count("-u") > 0)
    {
        if (dst.IsEmpty())
            dst = Path::ReplaceExtension(src, "trs");
        return Command_Decompile(src, dst);
    }

    String game_name;
    int game_uid = 0;
    bool use_game_uid = false;
    bool use_game_name = false;

    for (const auto &opt : cmdargs.OptWithValue)
    {
        if (opt.first == "--gamename")
        {
            game_name = opt.second;
            use_game_name = true;
        }
        else if (opt.first == "--uniqueid")
        {
            game_uid = StrUtil::StringToInt(opt.second);
            use_game_uid = true;
        }
    }

    if (dst.IsEmpty())
        dst = Path::ReplaceExtension(src, "tra");

    return Command_Compile(src, dst, use_game_name ? &game_name : nullptr, use_game_uid ? &game_uid : nullptr);
}
