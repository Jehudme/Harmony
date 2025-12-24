#pragma once

#include "Properties.h"
#include "EngineContext.h"
#include "IPlugins.h"

namespace Harmony {
	class EngineContext;

	class IEngine {
	public:
		enum class State {
			Uninitialized,
			Initialized,
			Running,
			Paused,
			Stopped
		};

	public:
		virtual ~IEngine() = default;

		virtual void Initialize(Properties properties) = 0;
		virtual void Shutdown() = 0;

		virtual void Run() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		
		virtual State GetState() const = 0;
		virtual EngineContext& GetContext() = 0;

	private:
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Events() = 0;
	};
} // namespace Harmony