#include "pch.h"
#include "Engine.h"
#include "Window.h"

namespace Harmony::Internals
{
	Engine::Engine(const Configuration& configuration)
		: window(configuration) {}

	Engine::~Engine() = default;

	void Engine::start()
	{
		while (!window.shouldClose())
		{
			handleEvents();
			handleUpdates();
			handleRendering();
		}
	}

	void Engine::handleEvents()
	{
	}

	void Engine::handleUpdates()
	{
	}

	void Engine::handleRendering()
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		EndDrawing();
	}

}