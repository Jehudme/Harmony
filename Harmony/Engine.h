#pragma once
#include "Clock.h"
#include "Window.h"

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
		Window window;

	private:
		Clock clock_;
	};
}

