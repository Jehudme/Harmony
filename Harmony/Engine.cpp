#include "pch.h"
#include "Object.h"
#include "Engine.h"
#include "StateManager.h"
#include "Configuration.h"
#include "Window.h"
#include "Event.h"
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include "State.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace harmony::core
{
    Engine::Engine(const std::shared_ptr<Configuration> config)
        : Object(*config), stateManager(std::make_shared<StateManager>())
    {
        try {
            // Create states if provided
            if (const auto states = config->getData({ "Configurations", "States" })) {
                for (const auto& state : states.value()) {
                    stateManager->addState(utilities::create<State>(Configuration(state)));
                }
			}
			else {
				throw std::runtime_error("No states configuration provided");
			}

			if (const auto initialState = config->getData<std::string>({ "Configurations", "InitialState" })) {
				stateManager->swichState(initialState.value());
			}
			else {
				throw std::runtime_error("No initial state provided");
			}

            // Create window if provided
            if (const auto window = config->getData({ "Configurations", "Window" })) {
                this->window = utilities::create<Window>(Configuration(window.value()));
                setRenderTarget(this->window->instance);
                renderMode = RenderMode::WindowRendering;
            }
            else {
                throw std::runtime_error("No window configuration provided");
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error during initialization: " + std::string(e.what()));
        }
    }

    void Engine::setRenderTarget(std::shared_ptr<sf::RenderTarget> renderTarget)
    {
        this->renderTarget = renderTarget;

        renderMode = std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)
            ? RenderMode::WindowRendering
            : RenderMode::TextureRendering;
    }

    void Engine::run() {
        try {
            if (!renderTarget) {
                renderTarget = std::make_shared<sf::RenderTexture>();
            }

            while (std::dynamic_pointer_cast<sf::RenderWindow>(renderTarget)->isOpen()) {
                handleEvents();
                update();
                render();
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error during run: " + std::string(e.what()));
        }
    }

    void Engine::handleWindowEvents()
    {
        try {
            sf::Event event;
            while (std::static_pointer_cast<sf::RenderWindow>(renderTarget)->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    std::static_pointer_cast<sf::RenderWindow>(renderTarget)->close();
                }
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error handling window events: " + std::string(e.what()));
        }
    }

    void Engine::handleEvents() {
        try {
            eventQueue.handleEvents();
            if (renderMode == RenderMode::WindowRendering) {
                handleWindowEvents();
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error handling events: " + std::string(e.what()));
        }
    }

    void Engine::update() {
        try {;
            stateManager->update(clock.restart(), eventQueue);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error during update: " + std::string(e.what()));
        }
    }

    void Engine::render() {
        try {
            renderTarget->clear();
            renderTarget->draw(*stateManager);

            if (renderMode == RenderMode::WindowRendering) {
                std::static_pointer_cast<sf::RenderWindow>(renderTarget)->display();
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error during render: " + std::string(e.what()));
        }
    }
}
