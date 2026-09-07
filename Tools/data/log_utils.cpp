#include "log_utils.h"
#include "debug/debugmanager.h"
#include "debug/outputhandler.h"

using namespace AGS::Common;

namespace AGS
{
namespace DataUtil
{

static const char* MTName(MessageType mt)
{
	switch (mt)
	{
	case kDbgMsg_Alert: return "ALERT";
	case kDbgMsg_Fatal: return "FATAL";
	case kDbgMsg_Error: return "ERROR";
	case kDbgMsg_Warn:  return "WARNING";
	case kDbgMsg_Info:  return "INFO";
	default:            return "DEBUG";
	}
}

using AGS::Common::IOutputHandler;
using AGS::Common::DebugMessage;

class ConsoleOutputTarget : public IOutputHandler
{
	bool printToolName = false;
public:
	String ToolName{};

	void OnRegister() override;

	void PrintMessage(const DebugMessage& msg) override;
};

void InitToolOutput(const String& ToolName)
{
	auto output = std::unique_ptr<ConsoleOutputTarget>();
	output.get()->ToolName = ToolName;
	DbgMgr.RegisterOutput("stdout", std::move(output), kDbgMsg_All, nullptr);
}

void ImplToolLog(MessageType mt, const String& msg)
{
	DbgMgr.Print(kDbgGroup_Main, mt, msg);
}

void ImplToolLog(MessageType mt, const char* fmt, va_list argptr)
{
	DbgMgr.Print(kDbgGroup_Main, mt, String::FromFormatV(fmt, argptr));
}

// -- LogInfo ----------------------------------------------------------
void LogInfo(const String& msg)
{
	ImplToolLog(kDbgMsg_Info, msg);
}

void LogInfo(const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	ImplToolLog(kDbgMsg_Info, fmt, argptr);
	va_end(argptr);
}

void LogInfo(const char* fmt, va_list argptr)
{
	ImplToolLog(kDbgMsg_Info, String::FromFormatV(fmt, argptr));
}
// ----------------------------------------------------------------------

// -- LogWarn ----------------------------------------------------------
void LogWarn(const String& msg)
{
	ImplToolLog(kDbgMsg_Warn, msg);
}

void LogWarn(const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	ImplToolLog(kDbgMsg_Warn, fmt, argptr);
	va_end(argptr);
}

void LogWarn(const char* fmt, va_list argptr)
{
	ImplToolLog(kDbgMsg_Warn, String::FromFormatV(fmt, argptr));
}
// ----------------------------------------------------------------------

// -- LogError ----------------------------------------------------------
void LogError(const String& msg)
{
	ImplToolLog(kDbgMsg_Error, msg);
}

void LogError(const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	ImplToolLog(kDbgMsg_Error, fmt, argptr);
	va_end(argptr);
}

void LogError(const char* fmt, va_list argptr)
{
	ImplToolLog(kDbgMsg_Error, String::FromFormatV(fmt, argptr));
}
// ----------------------------------------------------------------------

void ConsoleOutputTarget::OnRegister()
{
	// ensure messages are in the intended order
	setvbuf(stdout, nullptr, _IOLBF, 0);
}

void ConsoleOutputTarget::PrintMessage(const DebugMessage& msg)
{
	FILE* dest = (msg.MT != kDbgMsg_None && msg.MT <= kDbgMsg_Warn) ? stderr : stdout;
	if (printToolName)
	{
		fprintf(dest, "%s: %s: %s\n", ToolName.GetCStr(), MTName(msg.MT), msg.Text.GetCStr());
	}
	else
	{
		fprintf(dest, "%s: %s\n", MTName(msg.MT), msg.Text.GetCStr());
	}
	fflush(dest);
}

} // namespace DataUtil
} // namespace AGS
