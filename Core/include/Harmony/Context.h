#pragma once
#include "Harmony/Interfaces/IEngine.h"
#include "Harmony/Interfaces/IWindow.h"
#include "Harmony/Interfaces/IPluginsRegistry.h"


namespace Harmony {
	class Context {
		IEngine const* engine;
		IWindow const* window;
		IPluginsRegistry const* pluginsRegistry;
	};

} // namespace Harmony