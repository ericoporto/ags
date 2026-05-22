//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2026 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
// 
// AGS package file pack/unpack utility.
// 
// TODO:
// * option to fully test result of "create", including binary comparison of
//   all asset data.
// * option to write library in old format versions (to let test old engines).
// * proper unified error codes for the AGS tools?
// * clarify the use of "verbose" option, and make it consistent
//   throughout the operations.
// 
//=============================================================================
#include <regex>
#include "commands.h"
#include "data/include_utils.h"
#include "util/cmdlineopts.h"
#include "util/string_utils.h"

using namespace AGS::Common;
using namespace AGS::DataUtil;

const char *BIN_STRING = "agspak v0.4.0 - AGS game packaging tool\n"
    "Copyright (c) 2025 AGS Team and contributors\n";

const char *HELP_STRING = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak <COMMAND> <PAK-FILE> [<WORK-DIR>|<DEST_FILE>] [<FILES>] [OPTIONS]\n"
    "      executes an operation regarding the chosen package file, a working\n"
    "      directory, and an optional files list. Depending on a command either the\n"
    "      pack or the directory is an input or an output.\n"
    "      File list should be a comma-separated list of file names, which may\n"
    "      contain include/exclude patterns. If no file list is provided, then all\n"
    "      the files found in the respective input location (dir or pack) will be\n"
    "      selected for the operation.\n"
    "      Options may adjust the operation further.\n"
    "\n"
    "Commands:\n"
    "  create                 create a new pack file, gathering the files from the\n"
    "                         input directory.\n"
    "  extract                export (extract) files from the existing pack file\n"
    "                         into the output directory.\n"
    "  list                   print pack file's contents.\n"
    "  attach                 append pack data to the existing file, gathering the\n"
    "                         files from the input directory. Replaces the previously\n"
    "                         attached pack data, if there was any.\n"
    "  detach                 split an attached pack data into the <DEST_FILE>.\n"
    "                         NOTE: if pack data occupies whole file, then operation\n"
    "                         will be cancelled.\n"
    "  remove                 cut an attached pack data from the file.\n"
    "                         NOTE: if pack data occupies whole file, then its size\n"
    "                         will become zero.\n"
    "\n"
    "Command options:\n"
    "  -f, --pattern-file <file>\n"
    "                         when creating a pack file, use pattern file with the\n"
    "                         include/exclude patterns\n"
    "  -p, --partition <MB>   when creating a pack file, split asset files between\n"
    "                         partitions of this size max. Input files are not split,\n"
    "                         so files larger than this amount will occupy a single\n"
    "                         partition\n"
    "  -r, --recursive        when creating a pack file, include all subdirectories\n"
    "                         of a working directory too\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             Show help message for command\n"
    ;

const char *HELP_CREATE = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak create <PAK-FILE> <WORK-DIR> [<FILES>] [OPTIONS]\n"
    "      creates a new pack file, gathering files from the input directory.\n"
    "\n"
    "Options:\n"
    "  -f, --pattern-file <file>\n"
    "                         use pattern file with include/exclude patterns\n"
    "  -p, --partition <MB>   split assets between partitions of this size max\n"
    "  -r, --recursive        include all subdirectories of the working directory\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             show this help message\n"
    ;

const char *HELP_EXTRACT = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak extract <PAK-FILE> <WORK-DIR> [<FILES>]\n"
    "      exports (extracts) files from the existing pack file into the output\n"
    "      directory.\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             Show this help message\n"
    ;

const char *HELP_LIST = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak list <PAK-FILE>\n"
    "      prints the pack file's contents.\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             Show this help message\n"
    ;

const char *HELP_ATTACH = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak attach <PAK-FILE> <WORK-DIR> [<FILES>] [OPTIONS]\n"
    "      appends pack data to the existing file, gathering files from the input\n"
    "      directory. Replaces the previously attached pack data, if there was any.\n"
    "\n"
    "Options:\n"
    "  -f, --pattern-file <file>\n"
    "                         use pattern file with include/exclude patterns\n"
    "  -p, --partition <MB>   split assets between partitions of this size max\n"
    "  -r, --recursive        include all subdirectories of the working directory\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             show this help message\n"
    ;

const char *HELP_DETACH = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak detach <PAK-FILE> <DEST-FILE> [OPTIONS]\n"
    "      splits an attached pack data into <DEST-FILE>.\n"
    "      NOTE: if pack data occupies the whole file, then the operation will be\n"
    "      cancelled.\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             show this help message\n"
    ;

const char *HELP_REMOVE = "Usage:\n"
   //--------------------------------------------------------------------------------|
    "  agspak remove <PAK-FILE> [OPTIONS]\n"
    "      cuts an attached pack data from the file.\n"
    "      NOTE: if pack data occupies the whole file, then its size will become\n"
    "      zero.\n"
    "\n"
    "Other options:\n"
    "  -v, --verbose          print operation details\n"
    "  -h, --help             show this help message\n"
    ;

enum CommandType
{
    kCmdCreate = 0,
    kCmdExtract,
    kCmdList,
    kCmdAttach,
    kCmdDetach,
    kCmdRemove,
    kCmdMAX,
    kCmdNone = kCmdMAX
};

struct Command
{
    const char *Opt;
    const CommandType Cmd;
    const size_t NumArgs;
    const char *Help;
} Command[] = {
        {"create",  kCmdCreate,  2, HELP_CREATE},
        {"extract", kCmdExtract, 2, HELP_EXTRACT},
        {"list",    kCmdList,    1, HELP_LIST},
        {"attach",  kCmdAttach,  2, HELP_ATTACH},
        {"detach",  kCmdDetach,  2, HELP_DETACH},
        {"remove",  kCmdRemove,  1, HELP_REMOVE},
        {nullptr,   kCmdNone,    0, nullptr}
};

int DoCommand(CommandType command, const CmdLineOpts::ParseResult &cmdargs)
{
    // Fixed pos options
    const String pak_file = cmdargs.PosArgs.size() > 1 ? cmdargs.PosArgs[1] : String();
    const String work_dir = cmdargs.PosArgs.size() > 2 ? cmdargs.PosArgs[2] : String();
    const String dest_pak_file = cmdargs.PosArgs.size() > 2 ? cmdargs.PosArgs[2] : String();
    const String file_list_str = cmdargs.PosArgs.size() > 3 ? cmdargs.PosArgs[3] : String();
    // Common options
    // a include pattern file that should be inside the input-dir
    // TO-DO: support nested include pattern files in input-dir
    String pattern_file;

    // TODO: easier way to:
    //  - get either short or long named option;
    //  - get option's value without the search loop in the code
    size_t part_size_mb = 0;
    for (const auto &opt_with_value : cmdargs.OptWithValue)
    {
        if (opt_with_value.first == "-f" || opt_with_value.first == "--pattern-file")
        {
            pattern_file = opt_with_value.second;
        }
        else if (opt_with_value.first == "-p" || opt_with_value.first == "--partition")
        {
            part_size_mb = StrUtil::StringToInt(opt_with_value.second);
        }
    }
    const bool do_subdirs = cmdargs.Opt.count("-r") || cmdargs.Opt.count("--recursive");
    const bool verbose = cmdargs.Opt.count("-v") || cmdargs.Opt.count("--verbose");

    std::vector<String> pattern_list;
    if (!file_list_str.IsEmpty())
        pattern_list = file_list_str.Split(',');

    // Run supported commands
    switch (command)
    {
    case kCmdAttach:
    case kCmdCreate:
        {
            if (command == kCmdAttach)
                printf("Operation: append asset package to the existing file\n");
            else
                printf("Operation: create asset package\n");
            return AGSPak::Command_Create(work_dir, pak_file, command == kCmdAttach,
                pattern_list, pattern_file, do_subdirs, part_size_mb, verbose);
        }
    case kCmdExtract:
        {
            printf("Operation: export assets\n");
            return AGSPak::Command_Export(pak_file, work_dir, pattern_list);
        }
    case kCmdList:
        {
            return AGSPak::Command_List(pak_file);
        }
    case kCmdDetach:
        {
            printf("Operation: split asset package away from the existing file\n");
            return AGSPak::Command_Split(pak_file, dest_pak_file, verbose);
        }
    case kCmdRemove:
        {
            printf("Operation: cut asset package from the existing file\n");
            return AGSPak::Command_Cut(pak_file, verbose);
        }
    default:
        // we should never print this
        printf("Error: no valid command is specified\n");
        printf("%s\n", HELP_STRING);
        return -1;
    }

    // this should hopefully be unreachable code
    return -1;
}

int main(int argc, char *argv[])
{
    printf("%s\n", BIN_STRING);

    CmdLineOpts::ParseResult result = CmdLineOpts::Parse(argc, argv, {"-p", "-f", "--pattern-file", "--partition"});

    if (result.PosArgs.empty())
    {
        if (!result.HelpRequested)
        {
            printf("Error: not enough arguments\n");
        }
        printf("%s\n", HELP_STRING);
        return result.HelpRequested ? 0 : -1;
    }

    //-----------------------------------------------------------------------//
    // Parse command specific arguments
    //-----------------------------------------------------------------------//

    const String &asked_command = result.PosArgs[0];
    const size_t asked_command_argc = result.PosArgs.size() - 1;
    CommandType command = kCmdNone;

    for (int cmd = 0; cmd < kCmdMAX; cmd++)
    {
        if (asked_command.Equals(Command[cmd].Opt))
        {
            command = static_cast<CommandType>(cmd);
            const size_t required_cmd_argc = Command[cmd].NumArgs - 1;
            const char *cmd_help = Command[cmd].Help;
            if (result.HelpRequested)
            {
                printf("%s\n", cmd_help);
                return 0;
            }
            // create, extract, and attach can take an optional file list
            // so we will be less strict here
            // A proper way to handle this would be to only do this for these specific commands.
            const size_t max_argc = required_cmd_argc + 1;
            if (!(required_cmd_argc <= asked_command_argc && asked_command_argc <= max_argc))
            {
                printf("Error: required positional arguments don't match\n");
                printf("Requires %zu argument(s), passed %zu\n", required_cmd_argc, asked_command_argc);
                printf("%s\n", cmd_help);
                return -1;
            }
            break;
        }
    }

    if (command == kCmdNone)
    {
        printf("Error: unknown command '%s'\n", asked_command.GetCStr());
        printf("%s\n", HELP_STRING);
        return -1;
    }

    return DoCommand(command, result);
}
