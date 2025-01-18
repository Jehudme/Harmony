#include "pch.h"
#include "State.h"

namespace Harmony::Core {

    State::State(const uint64_t& uniqueId)
        : Object(uniqueId) {
    }

    void State::update(const sf::Time& time, EventPool& eventPool) {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->update(time, eventPool);
        }
    }

    void State::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->draw(renderTarget, states);
        }
    }

    void State::onEnter() {
    }

    void State::onExit() {
    }

    void State::addScene(const std::string& sceneName, std::shared_ptr<Scene> scene) {
        if (!scene) return;
        m_scenes[sceneName] = scene;
    }

    std::shared_ptr<Scene> State::getScene(const std::string& sceneName) const {
        auto it = m_scenes.find(sceneName);
        if (it != m_scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    void State::switchToScene(const std::string& sceneName) {
        auto scene = getScene(sceneName);
        if (scene) {
            m_renderBuffer.push(scene);
            scene->onEnter();
        }
    }

    void State::popScene() {
        if (!m_renderBuffer.empty()) {
            auto currentScene = m_renderBuffer.top();
            currentScene->onExit();
            m_renderBuffer.pop();
        }
    }
    
    void State::clearBuffer() {
        while (!m_renderBuffer.empty())
            m_renderBuffer.pop();
    }

    bool State::isSceneActive(const std::string& sceneName) const {
        return m_scenes.find(sceneName) != m_scenes.end();
    }

}
