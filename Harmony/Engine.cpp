#include "pch.h"
#include "Engine.h"

namespace Harmony::Core {

    Engine::Engine(const uint64_t& uniqueId)
        : renderWindow(sf::VideoMode(800, 600), "Harmony Engine"), clock() {
    }

    void Engine::run() {
        // Main game loop
        while (renderWindow.isOpen()) {
            event();
            update();
            draw();
        }
    }

    void Engine::event() {
        sf::Event sfEvent;
        while (renderWindow.pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed) {
                renderWindow.close();
            }
        }
    }

    void Engine::update() {
        sf::Time deltaTime = clock.restart();
        stateManager.update(deltaTime); // Update the current state through the StateManager
    }

    void Engine::draw() {
        renderWindow.clear();
        stateManager.draw(renderWindow, sf::RenderStates::Default); // Draw the current state through the StateManager
        renderWindow.display();
    }
}
