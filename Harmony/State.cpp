#include "pch.h"
#include "State.h"

namespace Harmony::Core {

    State::State(const uint64_t& uniqueId)
        : Object(uniqueId) {
    }

    void State::update(const sf::Time& time) {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->update(time); // Update the top scene on the stack
        }
    }

    void State::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->draw(target, states); // Render the top scene on the stack
        }
    }

    void State::onEnter() {
        // Implement logic to set up the state (e.g., loading scenes)
    }

    void State::onExit() {
        // Implement logic to clean up resources when exiting the state
    }

    void State::addScene(const std::string& sceneName, std::shared_ptr<Scene> scene) {
        scenes[sceneName] = scene; // Add a scene to the map
    }

    std::shared_ptr<Scene> State::getScene(const std::string& sceneName) {
        if (scenes.find(sceneName) != scenes.end()) {
            return scenes[sceneName]; // Return the scene if found
        }
        return nullptr; // Return nullptr if scene not found
    }

    void State::switchToScene(const std::string& sceneName) {
        auto scene = getScene(sceneName);
        if (scene) {
            m_renderBuffer.push(scene); // Push the scene onto the render stack
        }
    }

    void State::popScene() {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.pop(); // Pop the top scene off the render stack
        }
    }

    bool State::isSceneActive(const std::string& sceneName) const {
        return scenes.find(sceneName) != scenes.end(); // Check if the scene exists in the map
    }

}
