#include "pch.h"
#include "Object.h"
#include "Engine.h"
#include "StateManager.h"
#include "Configuration.h"
#include "Window.h"
#include "Logger.h"

namespace harmony::core
{
    Engine::Engine(const std::shared_ptr<Configuration> configuration) 
        : Object(configuration), stateManager(std::make_shared<StateManager>())
    {
        if (const auto states = configuration->getData({ "Configurations", "States" })) {
            for (const auto& state : states.value()) {
                stateManager->addState(utilities::create<State>(utilities::create<Configuration>(state)));
            }
        }
    }

    void Engine::initializeWindow(const uint64_t& windowId) {
        LOG_TRACE(Logger::core, "[Engine] Initializing window with ID: {}", windowId);
        renderTarget = utilities::create<Window>(windowId)->instance;
        displayWindow = true;
        LOG_TRACE(Logger::core, "[Engine] Window initialized successfully with ID: {}", windowId);
    }

    // The main game loop
    void Engine::run() {
        LOG_INFO(Logger::core, "[Engine] Starting main game loop for Engine ID: {}", uniqueId);
        if (!renderTarget) {
            LOG_WARN(Logger::core, "[Engine] No render target for Engine ID: {}, creating RenderTexture", uniqueId);
            renderTarget = std::make_shared<sf::RenderTexture>();
        }

        while (displayWindow && std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->isOpen()) {
            event();
            update();
            draw();
        }
        LOG_INFO(Logger::core, "[Engine] Main game loop ended for Engine ID: {}", uniqueId);
    }

    // Handle events (user input, window events)
    void Engine::event() {
        if (!displayWindow)
            return;

        sf::Event sfEvent;
        while (std::static_pointer_cast<sf::RenderWindow>(renderTarget)->pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed) {
                std::static_pointer_cast<sf::RenderWindow>(renderTarget)->close();
                LOG_INFO(Logger::core, "[Engine] Window closed event detected for Engine ID: {}", uniqueId);
            }
        }
    }

    void Engine::initialize(const std::shared_ptr<Configuration> configuration)
    {
    }

    void Engine::initiStates(const std::shared_ptr<Configuration> configuration)
    {
        if (const auto states = configuration->getData({ "Configurations", "States" })) {
            for (const auto& state : states.value()) {
                stateManager->addState(utilities::create<State>(utilities::create<Configuration>(state)));
            }
        }
    }

    // Update the game logic and current state
    void Engine::update() {
        sf::Time deltaTime = clock.restart();
        LOG_TRACE(Logger::core, "[Engine] Updating game logic for Engine ID: {} with delta time: {}", uniqueId, deltaTime.asSeconds());
        eventPool.handleEvent();
        stateManager->update(deltaTime, eventPool);
    }

    // Render the current state to the screen
    void Engine::draw() {
        LOG_TRACE(Logger::core, "[Engine] Drawing current state to screen for Engine ID: {}", uniqueId);
        renderTarget->clear();
        renderTarget->draw(*stateManager);

        if (displayWindow) {
            static auto renderWindow = std::static_pointer_cast<sf::RenderWindow>(renderTarget);
            renderWindow->display();
        }
        LOG_TRACE(Logger::core, "[Engine] Drawing completed for Engine ID: {}", uniqueId);
    }
}
