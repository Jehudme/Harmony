#include "pch.h"

#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "StateManagement.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h" // for HARMONY_* macros

namespace Harmony
{
	/// Exception type for engine-level errors
	class EngineError : public std::runtime_error {
	public:
		explicit EngineError(const std::string& msg)
			: std::runtime_error("Engine error: " + msg) {}
	};

	Engine::Engine(Utilities::Configuration& configuration)
		: configuration(configuration),
		taskManagement(std::make_unique<Management::TaskManager>(*this)),
		sceneManagement(std::make_unique<Management::SceneManager>(*this)),
		stateManagement(std::make_unique<Management::StateManager>(*this))
	{
		HARMONY_INFO("Engine initializing...");

		// Load window settings from configuration
		std::optional<std::string> title = configuration.get<std::string>({ "window", "title" });
		std::optional<unsigned int> width = configuration.get<unsigned int>({ "window", "width" });
		std::optional<unsigned int> height = configuration.get<unsigned int>({ "window", "height" });
		std::optional<unsigned int> fps = configuration.get<unsigned int>({ "window", "fps" });

		if (!title.has_value()) 
		{
			title = "Harmony Engine";
			HARMONY_WARN("Window title not specified in configuration, using default: '{}'", title.value());
		}
		if (!width.has_value()) 
		{
			width = 800;
			HARMONY_WARN("Window width not specified in configuration, using default: {}", width.value());
		}
		if (!height.has_value()) 
		{
			height = 600;
			HARMONY_WARN("Window height not specified in configuration, using default: {}", height.value());
		}
		if (!fps.has_value()) 
		{
			targetFPS_ = 0;
			HARMONY_WARN("Window FPS not specified in configuration, using default: uncapped");
		}
		else
		{
			targetFPS_ = fps.value();
		}

		if (width == 0 || height == 0) {
			HARMONY_ERROR("Invalid window dimensions: {}x{}", width.value(), height.value());
			throw EngineError("Window dimensions must be greater than zero");
		}

		window_.create(sf::VideoMode(width.value(), height.value()), title.value());
		if (!window_.isOpen()) {
			HARMONY_ERROR("Failed to create SFML window");
			throw EngineError("Window creation failed");
		}

		window_.setFramerateLimit(targetFPS_);
		HARMONY_INFO("Window created: '{}' ({}x{}, targetFPS={})", title.value(), width.value(), height.value(), targetFPS_);
	}

	Engine::~Engine() {
		HARMONY_INFO("Engine shutting down");
	}

	void Engine::start()
	{
		HARMONY_INFO("Engine starting main loop");
		clock_.restart();
		running_ = true;
		paused_ = false;

		while (running_ && window_.isOpen()) {
			deltaTime_ = clock_.restart();

			try {
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
		if (window_.isOpen()) {
			window_.close();
			HARMONY_INFO("Window closed");
		}
	}

	bool Engine::isRunning() const noexcept
	{
		return running_;
	}

	void Engine::setTargetFPS(unsigned int fps)
	{
		targetFPS_ = fps;
		if (targetFPS_ > 0) {
			window_.setFramerateLimit(targetFPS_);
			HARMONY_INFO("Target FPS set to {}", targetFPS_);
		}
		else {
			window_.setFramerateLimit(0);
			HARMONY_WARN("Target FPS uncapped");
		}
	}

	unsigned int Engine::getTargetFPS() const noexcept
	{
		return targetFPS_;
	}

	sf::Time Engine::getDeltaTime() const noexcept
	{
		return deltaTime_;
	}

	void Engine::handleTasks() {
		taskManagement->handleTasks();
	}

	void Engine::handleEvents()
	{
		sf::Event event;
		while (window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				HARMONY_INFO("Window close event received");
				stop();
			}
		}
	}

	void Engine::handleUpdates() {
		stateManagement->update(clock_.restart());
	}

	void Engine::handleRendering()
	{
		window_.clear(sf::Color::Black);
		window_.draw(*stateManagement.get());
		window_.display();
	}

} // namespace Harmony