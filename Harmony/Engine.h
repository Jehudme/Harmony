#pragma once

#include "StateManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

namespace Harmony::Core {

    class Engine {
    public:
        Engine(const uint64_t& uniqueId = Utilities::Random::generateId());

        // Run the main game loop
        void run();

    private:
        // Event handling method
        void event();

        // Update method for updating the current active state
        void update();

        // Draw method for drawing the current active state
        void draw();

    public:
        sf::RenderWindow renderWindow; // The window to render to
        sf::Clock clock;               // Clock to track time between frames
        StateManager stateManager;     // Manages the states of the game (e.g., GameState, PauseState)
    };

}
