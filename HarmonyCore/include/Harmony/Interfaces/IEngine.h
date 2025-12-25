#pragma once

#include "Harmony/Context.h"
#include "Harmony/Properties.h"

namespace Harmony {
	struct Context;

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

		virtual void Initialize(const Properties& properties) = 0;
		virtual void Shutdown() = 0;

		virtual void Run() = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;
		
		virtual State GetState() const = 0;
		virtual Context& GetContext() = 0;
	};
} // namespace Harmony