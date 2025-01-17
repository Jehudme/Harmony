#pragma once
#include "StateManager.h"

namespace Harmony::Core
{
	class Engine : public Object
	{
	public:
		Engine(const uint64_t& uniqueId = NULL);

		void initializeWindow(
			const unsigned int width = 600, const unsigned int height = 900, 
			const std::string& title = "Harmony", const unsigned int framerateLimit = 60, const bool verticalSyncEnabled = false
		);

		void run();

	private:
		void update();
		void event();
		void draw();

	public:
		std::shared_ptr<sf::RenderTarget> renderTarget;
		std::shared_ptr<StateManager> stateManager;
		sf::Clock clock;

		bool displayWindow;
	};
}

