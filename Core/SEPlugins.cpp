#include "SEPlugins.h"
#include "CoreTiming.h"
#include "Config.h"
#include "Core/HLE/sceKernelModule.h"

const static std::string PLUGINS_DIR = "seplugins";
static int SEPluginEvent = -1;
static bool PluginsLoaded = false;

void hleLoadPlugins(u64 userdata, int cyclesLate);

void __SEPluginsInit() {
	File::CreateDir(g_Config.memCardDirectory + PLUGINS_DIR);

	if(g_Config.bEnablePlugins)
	{
		SEPluginEvent = CoreTiming::RegisterEvent("SEPluginEvent", &hleLoadPlugins);
		CoreTiming::ScheduleEvent(msToCycles(500), SEPluginEvent, 0);
	}
}

void hleLoadPlugins(u64 userdata, int cyclesLate) 
{
	if(!PluginsLoaded)
	{
		std::string prxName = "mymodule.prx";
		if(File::Exists(g_Config.memCardDirectory + PLUGINS_DIR + "/" + prxName)) 
		{
			PluginsLoaded = true;

			ERROR_LOG(HLE, "Found elf/prx file!");
			extern u32 sceKernelLoadModule(const char *name, u32 flags, u32 optionAddr);
			std::string path = std::string("ms0:/" + PLUGINS_DIR + "/" + prxName);
			u32 ret = sceKernelLoadModule(path.c_str(), 0, 0);
			ERROR_LOG(HLE, "%08d returned trying to load %s", ret, prxName);
		}
		else ERROR_LOG(HLE, "PRX Not found D:");
	}
}