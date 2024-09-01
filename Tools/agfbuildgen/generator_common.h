//
// Created by erico on 2024-09-08.
//

#ifndef AGS_TOOL_AGFBUILDGEN__GENERATOR_COMMON_H
#define AGS_TOOL_AGFBUILDGEN__GENERATOR_COMMON_H

#include "util/string.h"

using namespace AGS::Common;

struct GeneratorOptions {
    // String ToolsDir; // not sure how to support this yet...

    String AgsDefnsFile {}; // path to agsdefns.sh

    String ToolAgfexport {};
    String ToolAgf2dlgasc {};
    String ToolAgscc {};
    String ToolTrac {};
    String ToolCrm2ash {};
    String ToolAgspak {};

    String GameProjectDir {};
    String GameProjectFile {};
    String OutputFile {};
    String OutputDir {};
    String TempDir {};

    bool HasDialogScripts {};

    std::vector<String> HeaderFileList {};
    std::vector<String> ScriptFileList {};
    std::vector<String> RoomFileList {};
    std::vector<String> TrsFileList {};
};

#endif // AGS_TOOL_AGFBUILDGEN__GENERATOR_COMMON_H
