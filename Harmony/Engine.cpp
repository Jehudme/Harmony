#include "pch.h"
#include "Engine.h"
#include <SFML/Window/Event.hpp>

namespace Harmony {

    Engine::Engine(std::shared_ptr<Configuration> configuration)
        : Object(configuration) {
    }

    void Engine::run() {
        while (renderWindow.isOpen()) {
            handleEvent();
            update();
            render();
        }
    }

    void Engine::initializeWindow(std::shared_ptr<Configuration> configuration)
    {
        // Extract title and size from configuration
        const std::string title = configuration->get<std::string>({ "Title" }).value_or("Harmony Engine");
        const sf::Vector2u size = {
            configuration->get<unsigned int>({ "Size", "Width" }).value_or(600),
            configuration->get<unsigned int>({ "Size", "Height" }).value_or(600)
        };

        // Extract fullscreen mode setting
        const bool fullscreen = configuration->get<bool>({ "Fullscreen" }).value_or(false);

        // Set up window style based on fullscreen mode
        sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

        // Create the render window
        renderWindow.create(sf::VideoMode(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y)), title, style);

        // Extract and apply vertical sync setting
        const bool verticalSync = configuration->get<bool>({ "VerticalSync" }).value_or(true);
        renderWindow.setVerticalSyncEnabled(verticalSync);

        // Extract and apply FPS limit setting
        const unsigned int fps = configuration->get<unsigned int>({ "FPS" }).value_or(60);
        renderWindow.setFramerateLimit(fps);
    }

    void Engine::handleEvent() {
        sf::Event event;
        while (renderWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderWindow.close();
            }
        }
    }

    void Engine::update() {
        // Implement the update logic here
    }

    void Engine::render() {
        renderWindow.clear();
        // Implement the rendering logic here
        renderWindow.display();
    }

}
