#include "pch.h"
#include "Engine.h"
#include "StateManager.h"

namespace Harmony::Core
{
    // Constructor
    Engine::Engine(const uint64_t& uniqueId)
        : Object(uniqueId), displayWindow(false), stateManager(std::make_shared<StateManager>()) {
    }

    void Engine::initializeWindow(unsigned int width, unsigned int height, const std::string& title, unsigned int framerateLimit, bool verticalSyncEnabled) {
        renderTarget = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), title);

        // Set window properties based on provided arguments
        std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->setFramerateLimit(framerateLimit);
        std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->setVerticalSyncEnabled(verticalSyncEnabled);

        displayWindow = true;
    }

    // The main game loop
    void Engine::run() {
        if (!renderTarget)
            initializeWindow();

        while (std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->isOpen()) {
            event();
            update();
            draw();
        }
    }

    // Handle events (user input, window events)
    void Engine::event() {
        sf::Event sfEvent;
        while (std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed) {
                std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->close();
            }
        }
    }

    // Update the game logic and current state
    void Engine::update() {
        sf::Time deltaTime = clock.restart();
        stateManager->update(deltaTime);
    }

    // Render the current state to the screen
    void Engine::draw() {
        renderTarget->clear();
        renderTarget->draw(*stateManager);

        if (displayWindow) {
            static auto renderWindow = std::static_pointer_cast<sf::RenderWindow>(renderTarget);
            renderWindow->display();
        }
    }
}
