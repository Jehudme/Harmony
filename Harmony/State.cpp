#include "pch.h"
#include "State.h"
#include "Configuration.h"

namespace harmony::core {

    State::State(const uint64_t& uniqueId)
        : Object(uniqueId) {
        const std::string uniqueIdKey = std::to_string(uniqueId);

        if (!configuration->getData({ "Objects", uniqueIdKey })) {
            throw std::runtime_error("State Require Valid Argument");
        }

        if (const auto sceneIds = configuration->getData({ "Objects", uniqueIdKey, "StatesId" })) {
            for (const auto& stateId : sceneIds.value()) {
                addScene(utilities::onEnter<Scene>(stateId.get<uint64_t>()));
            }
        }
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

    void State::addScene(std::shared_ptr<Scene> scene) {
        if (!scene) return;
        m_scenes[scene->uniqueId] = scene;
    }

    std::shared_ptr<Scene> State::getScene(const uint64_t& uniqueId) const {
        auto it = m_scenes.find(uniqueId);
        if (it != m_scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    void State::pushScene(const uint64_t& uniqueId) {
        auto scene = getScene(uniqueId);
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

    bool State::isSceneActive(const uint64_t& uniqueId) const {
        return m_scenes.find(uniqueId) != m_scenes.end();
    }

}
