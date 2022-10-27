#ifndef PLUGIN_AGS_WAVES_H
#define PLUGIN_AGS_WAVES_H

#include "plugin/agsplugin.h"

namespace agswaves
{
  void AGS_EngineStartup(IAGSEngine *lpEngine);
  void AGS_EngineShutdown(); // empty
  int AGS_EngineOnEvent(int event, int data);
  int AGS_EngineDebugHook(const char *scriptName, int lineNum, int reserved); // empty
  void AGS_EngineInitGfx(const char *driverID, void *data); // empty
}

#endif // PLUGIN_AGS_WAVES_H