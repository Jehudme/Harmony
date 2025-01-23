#pragma once
#include "StateManager.h"

namespace harmony::core
{
	class Engine : public Object
	{
	public:
		Engine(const uint64_t& uniqueId = NULL);

		void initializeWindow(const uint64_t& windowId = NULL);

		void run();

	private:
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

