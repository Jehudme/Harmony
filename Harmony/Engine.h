#pragma once
#include "StateManager.h"
#include "Configuration.h"

namespace harmony::core
{
	class Engine : public Object
	{
	public:
		Engine(const std::shared_ptr<Configuration> configuration);

		void initializeWindow(const uint64_t& windowId = NULL);

		void run();

	private:
		void initialize(const std::shared_ptr<Configuration> configuration);

		void initiStates(const std::shared_ptr<Configuration> configuration);

		void update();
		void event();
		void draw();

	public:
		std::shared_ptr<sf::RenderTarget> renderTarget;
		std::shared_ptr<StateManager> stateManager;
		sf::Clock clock;
		EventPool eventPool;

		bool displayWindow;
	};
}

