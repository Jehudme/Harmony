#include "pch.h"

#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "StateManagement.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Exceptions.h"
#include "RenderManager.h"
#include "Logger.h" // for HARMONY_* macros

namespace Harmony
{
	// PImpl struct to hide SFML types from header
	struct Engine::EngineImpl {
		sf::Clock clock;
		sf::Time deltaTime;
	};

	Engine::Engine(Utilities::Configuration& configuration)
		: configuration(configuration),
		impl_(std::make_unique<EngineImpl>()),
		renderManager(std::make_unique<Management::RenderManager>(*this)),
		resourceManager(std::make_unique<Management::ResourceManager>(*this)),
		taskManagement(std::make_unique<Management::TaskManager>(*this)),
		sceneManagement(std::make_unique<Management::SceneManager>(*this)),
		stateManagement(std::make_unique<Management::StateManager>(*this)),
		componentManagement(std::make_unique<Management::ComponentManager>(*this)),
		inputManager(std::make_unique<InputManager>(*this))
	{
	}

	Engine::~Engine() {
		HARMONY_INFO("Engine shutting down");
	}

	void Engine::start()
	{
		HARMONY_INFO("Engine starting main loop");
		running_ = true;
		paused_ = false;

		while (running_) {
			try {
				impl_->deltaTime = impl_->clock.restart();
				handleTasks();
				handleEvents();
				handleUpdates();
				handleRendering();
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Unhandled exception in main loop: {}", e.what());
				stop();
			}
		}

		HARMONY_INFO("Main loop terminated");
	}

	void Engine::stop()
	{
		HARMONY_WARN("Engine stopping...");
		running_ = false;
		if (renderManager->isOpen()) {
			renderManager->close();
			HARMONY_INFO("Window closed");
		}
	}

	bool Engine::isRunning() const noexcept
	{
		return running_;
	}

	const sf::Time& Engine::getDeltaTime() const noexcept
	{
		return impl_->deltaTime;
	}

	void Engine::handleTasks() {
		taskManagement->handleTasks();
	}

	void Engine::handleEvents()
	{
		sf::Event event;
		while (renderManager->pollEvent(event)) {
			// Forward all events to the InputManager for processing
			//inputManager->handleEvent(event);

			// Handle specific engine-level events using a switch statement for clarity
			switch (event.type)
			{
			case sf::Event::Closed:
				HARMONY_INFO("Window close event received");
				stop();
				break;

			case sf::Event::Resized:
				HARMONY_DEBUG("Window resized to {}x{}", event.size.width, event.size.height);
				// Future: Handle window resize events
				break;

			case sf::Event::LostFocus:
				HARMONY_DEBUG("Window lost focus");
				// Future: Could pause game or clear input states
				break;

			case sf::Event::GainedFocus:
				HARMONY_DEBUG("Window gained focus");
				// Future: Could resume game
				break;

			// Input events are already handled by InputManager::handleEvent
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseMoved:
			case sf::Event::MouseWheelScrolled:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
				// Already processed by InputManager, no additional action needed
				break;

			case sf::Event::TextEntered:
				HARMONY_TRACE("Text entered: {}", event.text.unicode);
				// Future: Text input for UI elements
				break;

			case sf::Event::JoystickButtonPressed:
			case sf::Event::JoystickButtonReleased:
			case sf::Event::JoystickMoved:
			case sf::Event::JoystickConnected:
			case sf::Event::JoystickDisconnected:
				HARMONY_TRACE("Joystick event: {}", static_cast<int>(event.type));
				// Future: Joystick input support
				break;

			case sf::Event::TouchBegan:
			case sf::Event::TouchMoved:
			case sf::Event::TouchEnded:
				HARMONY_TRACE("Touch event: {}", static_cast<int>(event.type));
				// Future: Touch input support
				break;

			case sf::Event::SensorChanged:
				// Future: Sensor input support (accelerometer, etc.)
				break;

			default:
				// Unknown or unhandled event type
				break;
			}
		}

		// Update input states at the end of event processing
		// This transitions "Pressed" to "Held" states
		//inputManager->updateStates();
	}

	void Engine::handleUpdates() {
		try {
			stateManagement->update(impl_->deltaTime.asSeconds());
		}
		catch (const Exceptions::StateStackEmptyError& e) {
			HARMONY_ERROR("Cannot update: {}", e.what());
			throw;
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Update error: {}", e.what());
			throw;
		}
	}

	void Engine::handleRendering()
	{
		try {
			renderManager->clear(sf::Color::Black);
			stateManagement->internalDraw(renderManager->getRenderTarget());
			renderManager->display();
		}
		catch (const Exceptions::StateStackEmptyError& e) {
			HARMONY_ERROR("Cannot render: {}", e.what());
			throw;
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Rendering error: {}", e.what());
			throw;
		}
	}

} // namespace Harmony