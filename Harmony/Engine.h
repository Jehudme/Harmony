#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "Object.h"
#include "Configuration.h"
#include "TaskQueue.h"

namespace Harmony {

    // Constants for configuration keys
    constexpr const char* CONFIG_WINDOW         = "Window";
    constexpr const char* CONFIG_TITLE          = "Title";
    constexpr const char* CONFIG_SIZE           = "Size";
    constexpr const char* CONFIG_WIDTH          = "Width";
    constexpr const char* CONFIG_HEIGHT         = "Height";
    constexpr const char* CONFIG_FULLSCREEN     = "Fullscreen";
    constexpr const char* CONFIG_VERTICAL_SYNC  = "VerticalSync";
    constexpr const char* CONFIG_FPS            = "FPS";

    class StateStack;
    class Engine : public Object {
    public:
        explicit Engine(std::shared_ptr<Configuration> configuration);

        void run();

        // Getters
        const sf::RenderWindow& getRenderWindow() const;
        std::shared_ptr<StateStack> getStateStack() const;
        std::shared_ptr<Configuration> getConfiguration() const;

        // Setters
        void setConfiguration(std::shared_ptr<Configuration> configuration);

    private:
        void initializeWindow(std::shared_ptr<Configuration> configuration);

        void handleEvent();
        void update();
        void render();

        sf::Clock clock_;
        TaskQueue taskQueue_;
        sf::RenderWindow renderWindow_;
        std::shared_ptr<StateStack> stateStack_;
        std::shared_ptr<Configuration> configuration_;
    };
}