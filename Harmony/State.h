#pragma once
#include "Scene.h"
#include <stack>
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

        namespace Harmony::Core {

            class State : public Object, public sf::Drawable {
            public:
                // Constructor to initialize with a unique ID
                State(const uint64_t& uniqueId = Utilities::Random::generateId());

                // Called every frame to update the current scene(s)
                virtual void update(const sf::Time& time);

                // Draws the current scene(s) from the render stack
                virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

                // Called when entering the state (e.g., setting up scenes)
                virtual void onEnter();

                // Called when exiting the state (e.g., cleaning up resources)
                virtual void onExit();

                // Add a scene by name
                void addScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

                // Get a scene by name
                std::shared_ptr<Scene> getScene(const std::string& sceneName);

                // Switch to a new scene (push it to the render stack)
                void switchToScene(const std::string& sceneName);

                // Pop the current scene (return to the previous one)
                void popScene();

                // Check if a scene is currently active
                bool isSceneActive(const std::string& sceneName) const;

            protected:
                // Map to store scenes by name
                std::map<std::string, std::shared_ptr<Scene>> scenes;

                // Stack to buffer scenes for rendering
                std::stack<std::shared_ptr<Scene>> m_renderBuffer;
            };

        }
