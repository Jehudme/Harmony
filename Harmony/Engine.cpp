#include "pch.h"
#include "Engine.h"
#include "Constants.h"
#include <SFML/Window/Event.hpp>
#include "StateStack.h"

namespace Harmony {

    Engine::Engine(std::shared_ptr<Configuration> configuration)
        : Object(configuration), configuration_(configuration), stateStack_(create<StateStack>(configuration)) {
        initializeWindow(configuration);
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
        const std::string title = configuration->get<std::string>({ Config::WINDOW_TITLE }).value_or(Default::WINDOW_TITLE);
        const sf::Vector2u size = {
            configuration->get<unsigned int>({ Config::WINDOW_SIZE_WIDTH }).value_or(Default::WINDOW_WIDTH),
            configuration->get<unsigned int>({ Config::WINDOW_SIZE_HEIGHT }).value_or(Default::WINDOW_HEIGHT)
        };

        // Extract fullscreen mode setting
        const bool fullscreen = configuration->get<bool>({ Config::WINDOW_FULLSCREEN }).value_or(Default::FULLSCREEN);

        // Set up window style based on fullscreen mode
        sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

        // Create the render window
        renderWindow_.create(sf::VideoMode(size.x, size.y), title, style);

        // Extract and apply vertical sync setting
        const bool verticalSync = configuration->get<bool>({ Config::WINDOW_VERTICAL_SYNC }).value_or(Default::VERTICAL_SYNC);
        renderWindow_.setVerticalSyncEnabled(verticalSync);

        // Extract and apply FPS limit setting
        const unsigned int fps = configuration->get<unsigned int>({ Config::WINDOW_FPS }).value_or(Default::FPS);
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

    const sf::RenderWindow& Engine::getRenderWindow() const {
        return renderWindow_;
    }

    std::shared_ptr<StateStack> Engine::getStateStack() const {
        return stateStack_;
    }

    std::shared_ptr<Configuration> Engine::getConfiguration() const {
        return configuration_;
    }

    void Engine::setConfiguration(std::shared_ptr<Configuration> configuration) {
        if (!configuration) {
            throw std::invalid_argument(Error::NULL_CONFIGURATION);
        }
        configuration_ = configuration;
        initializeWindow(configuration); // Reinitialize the window with the new configuration
    }
}