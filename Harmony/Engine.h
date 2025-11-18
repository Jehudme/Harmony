#pragma once
#include "Clock.h"
#include "Configuration.h"

namespace Harmony::Internals {
	class WindowHandler;
	class TasksHandler;
}

namespace Harmony::Internals {
	class Engine
	{
	public:
		Engine(const Configuration& configuration);
		~Engine();

		void start();
		void stop();

	private:
		void handleEvents();
		void handleUpdates();
		void handleRendering();

	public:
		std::unique_ptr<TasksHandler> tasksHandler;
		std::unique_ptr<WindowHandler> windowHandler;
	private:
		Clock clock_;
	};
}

