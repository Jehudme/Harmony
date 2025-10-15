#include "pch.h"

#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "StateManagement.h"
#include "SceneManagement.h"

namespace Harmony::Internals 
{
    Engine::Engine(Configuration& configuration): 
        configuration(configuration),
        taskManagement(std::make_unique<TaskManagement>(*this)),
		sceneManagement(std::make_unique<SceneManagement>(*this)),
        stateManagement(std::make_unique<StateManagement>(*this)) 
    {
        // Load window settings from configuration
        std::string title   = configuration.get<std::string>   ({ "window", "title" }) .value_or("Harmony Engine");
        unsigned int width  = configuration.get<unsigned int>  ({ "window", "width" }) .value_or(800);
        unsigned int height = configuration.get<unsigned int>  ({ "window", "height" }).value_or(600);
        targetFPS_          = configuration.get<unsigned int>  ({ "window", "fps" })   .value_or(0);

        window_.create(sf::VideoMode(width, height), title);
        window_.setFramerateLimit(targetFPS_);
    }

    Internals::Engine::~Engine() = default;

    void Engine::start() 
    {
        clock_.restart();
        running_ = true;
        paused_ = false;

        while (running_ && window_.isOpen()) 
        {
            deltaTime_ = clock_.restart();

            // Main loop stages
            handleTasks();
            handleEvents();
            handleUpdates();
            handleRendering();
        }
    }

    void Engine::stop() 
    {
        running_ = false;
        if (window_.isOpen()) 
            window_.close();
    }

    bool Engine::isRunning() const noexcept 
    {
        return running_;
    }

 
    void Engine::setTargetFPS(unsigned int fps) 
    {
        targetFPS_ = fps;
        if (targetFPS_ > 0) window_.setFramerateLimit(targetFPS_);
        else window_.setFramerateLimit(0); // uncapped
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
        taskManagement->handleTasks();
    }

    void Engine::handleEvents() 
    {
        sf::Event event;
        while (window_.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                stop();
            }
        }
    }

    void Engine::handleUpdates() 
    {
        stateManagement->update(clock_.restart(), *taskManagement.get());
    }

    void Engine::handleRendering() 
    {
        window_.clear(sf::Color::Black);
        window_.draw(*stateManagement.get());
        window_.display();
    }

} // namespace Harmony