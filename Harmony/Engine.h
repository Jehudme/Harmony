#pragma once


namespace Harmony {
	class Configuration;

	class Engine {
	public:
		Engine(Configuration& configuration);

		void start();
		void stop();

	private:
		void handleTasks();
		void handleEvents();
		void handleUpdates();
		void handleRendering();

	private:
		struct Internal;
		std::unique_ptr<Internal> internal_;
	};
}

