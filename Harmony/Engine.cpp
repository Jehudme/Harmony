#include "pch.h"
#include "Engine.h"
#include <SFML/Window/Event.hpp>
#include "StateStack.h"

namespace Harmony {

    // Constants for default values
    constexpr const char* DEFAULT_WINDOW_TITLE = "Harmony Engine";
    constexpr unsigned int DEFAULT_WINDOW_WIDTH = 600;
    constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 600;
    constexpr bool DEFAULT_FULLSCREEN = false;
    constexpr bool DEFAULT_VERTICAL_SYNC = true;
    constexpr unsigned int DEFAULT_FPS = 60;

    Engine::Engine(std::shared_ptr<Configuration> configuration)
        : Object(configuration), configuration_(configuration), stateStack_(create<StateStack>(configuration)) {

        if (const auto windowData = configuration->get({ CONFIG_WINDOW }))
            initializeWindow(create<Configuration>(windowData.value()));

        else
            initializeWindow(create<Configuration>());
    }

    void Engine::run() {
        while (renderWindow_.isOpen()) {
            handleEvent();
            update();
            render();
        }
    }

    void Engine::initializeWindow(std::shared_ptr<Configuration> configuration) {
        // Extract title and size from configuration
        const std::string title = configuration->get<std::string>({ CONFIG_TITLE }).value_or(DEFAULT_WINDOW_TITLE);
        const sf::Vector2u size = {
            configuration->get<unsigned int>({ CONFIG_SIZE, CONFIG_WIDTH }).value_or(DEFAULT_WINDOW_WIDTH),
            configuration->get<unsigned int>({ CONFIG_SIZE, CONFIG_HEIGHT }).value_or(DEFAULT_WINDOW_HEIGHT)
        };

        // Extract fullscreen mode setting
        const bool fullscreen = configuration->get<bool>({ CONFIG_FULLSCREEN }).value_or(DEFAULT_FULLSCREEN);

        // Set up window style based on fullscreen mode
        sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

        // Create the render window
        renderWindow_.create(sf::VideoMode(size.x, size.y), title, style);

        // Extract and apply vertical sync setting
        const bool verticalSync = configuration->get<bool>({ CONFIG_VERTICAL_SYNC }).value_or(DEFAULT_VERTICAL_SYNC);
        renderWindow_.setVerticalSyncEnabled(verticalSync);

        // Extract and apply FPS limit setting
        const unsigned int fps = configuration->get<unsigned int>({ CONFIG_FPS }).value_or(DEFAULT_FPS);
        renderWindow_.setFramerateLimit(fps);
    }

    void Engine::handleEvent() {
        sf::Event event;

        taskQueue_.execute();
        while (renderWindow_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderWindow_.close();
            }
        }
    }

    void Engine::update() {
        stateStack_->update(clock_.restart(), taskQueue_);
    }

    void Engine::render() {
        renderWindow_.clear();
        renderWindow_.draw(*stateStack_);
        renderWindow_.display();
    }

    // Getters
    const sf::RenderWindow& Engine::getRenderWindow() const {
        return renderWindow_;
    }

    std::shared_ptr<StateStack> Engine::getStateStack() const {
        return stateStack_;
    }

    std::shared_ptr<Configuration> Engine::getConfiguration() const {
        return configuration_;
    }

    // Setters
    void Engine::setConfiguration(std::shared_ptr<Configuration> configuration) {
        if (!configuration) {
            throw std::invalid_argument("Configuration cannot be null.");
        }
        configuration_ = configuration;
        initializeWindow(configuration); // Reinitialize the window with the new configuration
    }
}