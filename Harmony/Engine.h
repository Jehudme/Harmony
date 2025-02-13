#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Object.h"
#include "Configuration.h"
#include "TaskQueue.h"

namespace Harmony {
    class StateStack;
    class Engine : public Object {
    public:
        Engine(std::shared_ptr<Configuration> configuration);

        void run();

    private:
        virtual void initializeWindow(std::shared_ptr<Configuration> configuration);

        virtual void handleEvent();
        virtual void update();
        virtual void render();

        sf::Clock clock;
        TaskQueue taskQueue;
        sf::RenderWindow renderWindow;
        std::shared_ptr<StateStack> stateStack;
        std::shared_ptr<Configuration> configuration;    
    };

}

