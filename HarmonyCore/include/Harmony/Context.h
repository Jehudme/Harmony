#pragma once

namespace Harmony {
	class IWindow;
	class IEngine;

	struct Context {
		IEngine* engine;
		IWindow* window;
	};

} // namespace Harmony