#include "pch.h"
#include "State.h"
#include "Configuration.h"
#include <memory>
#include <string>
#include "Event.h"
#include "Object.h"
#include "Scene.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

namespace harmony::core {

    State::State(const Configuration& configuration)
        : Object(configuration) {
        if (const auto scenes = configuration.getData({ "Scenes" })) {
            for (const auto& scene : scenes.value()) {
                addScene(utilities::create<Scene>(Configuration(scene)));
            }
        }
    }

    void State::update(const sf::Time& time, EventQueue& eventQueue) {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->update(time, eventQueue);
        }
    }

    void State::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->draw(renderTarget, states);
        }
    }

    void State::onEnter() {}

    void State::onExit() {}

    void State::addScene(std::shared_ptr<Scene> scene) {
        if (scene) {
            m_scenes[scene->name] = scene;
        }
    }

    std::shared_ptr<Scene> State::getScene(const std::string& name) const {
        auto it = m_scenes.find(name);
        return (it != m_scenes.end()) ? it->second : nullptr;
    }

    void State::pushScene(const std::string& name) {
        if (auto scene = getScene(name)) {
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
        while (!m_renderBuffer.empty()) {
            m_renderBuffer.pop();
        }
    }

    bool State::isSceneActive(const std::string& name) const {
        return m_scenes.find(name) != m_scenes.end();
    }

}
