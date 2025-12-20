#pragma once

#include "Properties.h"

#include <memory>

namespace Harmony {
	struct EngineContext;

	class Engine {
	public:
		Engine(Properties properties);
		~Engine();

		void start();
		void stop();

		void pause();
		void resume();

		bool running() const;
		bool paused() const;

		EngineContext context();

	private:
		void event();
		void update();
		void render();

		void waitWhilePaused();

	private:
		struct Internal;

	private:
		std::unique_ptr<Internal> m_internal;
	};
}