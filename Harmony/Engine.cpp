#include "pch.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "TasksHandler.h"
#include "ResourceHandler.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"
#include "Configuration.h"

#include "Scene.h"

namespace Harmony::Internals
{
	static std::unique_ptr<Scene> sceneInstance = nullptr;

	Engine::Engine(const Configuration configuration) : 
		windowHandler(nullptr),
		tasksHandler(nullptr)
	{
		HARMONY_INFO("Initializing Harmony Engine");
		
		try {
			// Initialize configuration
			HARMONY_DEBUG("Initializing Configuration subsystem");
			this->configuration = std::make_unique<Configuration>(configuration);

			// Initialize tasks handler
			HARMONY_DEBUG("Initializing TasksHandler subsystem");
			tasksHandler = std::make_unique<TasksHandler>(*this);

			HARMONY_INFO("TasksHandler initialized successfully");

			// Initialize resources handler
			HARMONY_DEBUG("Initializing ResourcesHandler subsystem");
			resourcesHandler = std::make_unique<ResourcesHandler>(*this);

			// Initialize window handler
			HARMONY_DEBUG("Initializing WindowHandler subsystem");
			windowHandler = std::make_unique<WindowHandler>(
				configuration.subsection({"window"}).value_or(Configuration()));
			
			HARMONY_INFO("WindowHandler initialized successfully");

			// Start tasks handler
			tasksHandler->start();

			sceneInstance = std::make_unique<Scene>(*this, 101);

			HARMONY_INFO("Harmony Engine initialized successfully");
		}
		catch (const Exceptions::HarmonyException& e) {
			HARMONY_CRITICAL("Engine initialization failed with HarmonyException: {}", e.what());
			throw Exceptions::EngineInitializationException(e.what());
		}
		catch (const std::exception& e) {
			HARMONY_CRITICAL("Engine initialization failed with exception: {}", e.what());
			throw Exceptions::EngineInitializationException(e.what());
		}
	}

	Engine::~Engine() {
		HARMONY_INFO("Destroying Harmony Engine");
	}

	void Engine::start()
	{
		HARMONY_INFO("Starting Harmony Engine main loop");
		HARMONY_ASSERT_NOT_NULL(windowHandler.get(), "WindowHandler is null");
		
		if (!windowHandler) {
			HARMONY_CRITICAL("Cannot start engine: WindowHandler is null");
			throw Exceptions::InvalidEngineStateException("start", "WindowHandler is null");
		}

		while (!WindowShouldClose())
		{
			handleEvents();
			handleUpdates();
			handleRendering();
		}
		
		HARMONY_INFO("Harmony Engine main loop exited");
	}

	void Engine::stop()
	{
		HARMONY_INFO("Stopping Harmony Engine");
		HARMONY_ASSERT_NOT_NULL(windowHandler.get(), "WindowHandler is null");
		
		if (windowHandler) {
			windowHandler->close();
			HARMONY_DEBUG("Window closed");
		} else {
			HARMONY_WARN("Cannot stop engine: WindowHandler is null");
		}
	}

	void Engine::handleEvents()
	{
		HARMONY_TRACE("Handling events");

	}

	void Engine::handleUpdates()
	{
		HARMONY_TRACE("Handling updates");
		sceneInstance->update();
		resourcesHandler->handleResources();
		tasksHandler->handleTasks();
	}

	void Engine::handleRendering()
	{
		HARMONY_TRACE("Handling rendering");
		
		BeginDrawing();
		ClearBackground(RAYWHITE);
		sceneInstance->render();
		EndDrawing();
	}

}