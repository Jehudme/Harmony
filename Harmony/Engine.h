#pragma once
#include "StateManager.h"
#include "Configuration.h"
#include "Window.h"

namespace harmony::core
{
    class Engine : public Object
    {
    public:
        Engine(const std::shared_ptr<Configuration> config);
		void setRenderTarget(std::shared_ptr<sf::RenderTarget> renderTarget);
        void run();

    private:
        void handleWindowEvents();
        void handleEvents();
        void update();
        void render();

    private:
        enum RenderMode
        {
            TextureRendering,
            WindowRendering
        };

    public:
        std::shared_ptr<sf::RenderTarget> renderTarget;
        std::shared_ptr<StateManager> stateManager;
        std::shared_ptr<Window> window;

        sf::Clock clock;
        EventQueue eventQueue;
        RenderMode renderMode;
    };
}


