#pragma once

namespace Harmony {
	class IWindow;
	class IEngine;
	class IPluginsRegistry;

	struct Context {
		IEngine* engine;
		IWindow* window;
		IPluginsRegistry* pluginsRegistry;
	};

} // namespace Harmony