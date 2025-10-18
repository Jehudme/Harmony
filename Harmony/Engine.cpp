#include "pch.h"

#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "StateManagement.h"
#include "SceneManagement.h"
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
        taskManagement(std::make_unique<Management::TaskManagemer>(*this)),
        sceneManagement(std::make_unique<Management::SceneManager>(*this)),
        stateManagement(std::make_unique<Management::StateManager>(*this))
    {
        HARMONY_INFO("Engine initializing...");

        // Load window settings from configuration
        std::string title = configuration.get<std::string>({ "window", "title" }).value_or("Harmony Engine");
        unsigned int width = configuration.get<unsigned int>({ "window", "width" }).value_or(800);
        unsigned int height = configuration.get<unsigned int>({ "window", "height" }).value_or(600);
        targetFPS_ = configuration.get<unsigned int>({ "window", "fps" }).value_or(0);

        if (width == 0 || height == 0) {
            HARMONY_ERROR("Invalid window dimensions: {}x{}", width, height);
            throw EngineError("Window dimensions must be greater than zero");
        }

        window_.create(sf::VideoMode(width, height), title);
        if (!window_.isOpen()) {
            HARMONY_ERROR("Failed to create SFML window");
            throw EngineError("Window creation failed");
        }

        window_.setFramerateLimit(targetFPS_);
        HARMONY_INFO("Window created: '{}' ({}x{}, targetFPS={})", title, width, height, targetFPS_);
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

    void Engine::handleTasks()
    {
        HARMONY_DEBUG("Handling tasks");
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

    void Engine::handleUpdates()
    {
        HARMONY_DEBUG("Updating states");
        stateManagement->update(clock_.restart());
    }

    void Engine::handleRendering()
    {
        HARMONY_DEBUG("Rendering frame");
        window_.clear(sf::Color::Black);
        window_.draw(*stateManagement.get());
        window_.display();
    }

} // namespace Harmony