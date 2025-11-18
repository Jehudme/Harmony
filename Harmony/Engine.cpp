#include "pch.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "TasksHandler.h"


namespace Harmony::Internals
{
	Engine::Engine(const Configuration& configuration) : 
		windowHandler(std::make_unique<WindowHandler>(configuration.subsection({"window"}).value_or(Configuration()))),
		tasksHandler(std::make_unique<TasksHandler>(*this))
	{
	}

	Engine::~Engine() = default;

	void Engine::start()
	{
		while (WindowShouldClose())
		{
			handleEvents();
			handleUpdates();
			handleRendering();
		}
	}

	void Engine::stop()
	{
		windowHandler->close();
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