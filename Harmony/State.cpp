#include "pch.h"
#include "State.h"
#include "TaskQueue.h"
#include "Scene.h"
#include "Configuration.h"
#include "Script.h"

namespace Harmony {

    State::State(std::shared_ptr<Configuration> configuration)
        : Object(configuration) {

        if (const auto scenesConfigurationsData = configuration->get({ "Scenes" })) {
            for (const auto sceneConfigurations : scenesConfigurationsData.value()) {
                addScene(create<Scene>(create<Configuration>(sceneConfigurations)));                
            }
        }

        if (const auto initialScenesData = configuration->get({ "InitialScenes" })) {
            for (const auto SceneName : initialScenesData.value()) {
                m_initialBuffer.push_back(SceneName.get<std::string>());
            }
        }

        if (const auto scriptName = configuration->get<std::string>({ "Script" }))
        {
            m_script = Harmony::find<Harmony::Script>(scriptName.value());
        }
    }

    void State::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (m_script)
        {
            m_script->onDraw(shared_from_this(), renderTarget, states);
        }
        
        for (const auto& scene : m_scenes) {
            renderTarget.draw(*scene, states);
        }
    }

    void State::update(const sf::Time& time, TaskQueue& taskQueue) {
        if (m_script)
        {
            m_script->onUpdate(shared_from_this(), time, taskQueue);
        }

        for (const auto& scene : m_scenes) {
            scene->update(time, taskQueue);
        }
    }

    void State::onEnter() {
        processInitializeScenes();
        for (auto scene : m_scenes) {
            scene->onEnter();
        }

        if (m_script)
        {
            m_script->onUpdate(shared_from_this());
        }
    }

    void State::onExit() {
        clearSceneBuffer();
        for (auto scene : m_scenes) {
            scene->onExit();
        }

        if (m_script)
        {
            m_script->onUpdate(shared_from_this());
        }
    }

    std::shared_ptr<Scene> State::getSceneByName(const std::string& name) {
        for (const auto& scene : m_scenes) {
            if (scene->getName() == name) {
                return scene;
            }
        }
        throw std::runtime_error("Scene not found by name: " + name);
    }

    std::shared_ptr<Scene> State::getSceneById(const uint64_t& uniqueId) {
        for (const auto& scene : m_scenes) {
            if (scene->getUniqueId() == uniqueId) {
                return scene;
            }
        }
        throw std::runtime_error("Scene not found by uniqueId: " + std::to_string(uniqueId));
    }

    void State::addScene(std::shared_ptr<Scene> scene) {
        m_scenes.push_back(scene);
    }

    void State::removeSceneByName(const std::string& name) {
        auto it = std::remove_if(m_scenes.begin(), m_scenes.end(),
            [&name](const std::shared_ptr<Scene>& scene) {
                return scene->getName() == name;
            });
        m_scenes.erase(it, m_scenes.end());
    }

    void State::removeSceneById(const uint64_t& uniqueId) {
        auto it = std::remove_if(m_scenes.begin(), m_scenes.end(),
            [&uniqueId](const std::shared_ptr<Scene>& scene) {
                return scene->getUniqueId() == uniqueId;
            });
        m_scenes.erase(it, m_scenes.end());
    }

    void State::queueSceneByName(const std::string& name) {
        for (const auto& scene : m_scenes) {
            if (scene->getName() == name) {
                m_sceneBuffer.push_back(scene);
                return;
            }
        }
        throw std::runtime_error("Scene not found by name: " + name);
    }

    void State::queueSceneById(const uint64_t& uniqueId) {
        for (const auto& scene : m_scenes) {
            if (scene->getUniqueId() == uniqueId) {
                m_sceneBuffer.push_back(scene);
                return;
            }
        }
        throw std::runtime_error("Scene not found by uniqueId: " + std::to_string(uniqueId));
    }

    void State::removeQueuedSceneByName(const std::string& name) {
        auto it = std::remove_if(m_sceneBuffer.begin(), m_sceneBuffer.end(),
            [&name](const std::shared_ptr<Scene>& scene) {
                return scene->getName() == name;
            });
        m_sceneBuffer.erase(it, m_sceneBuffer.end());
    }

    void State::removeQueuedSceneById(const uint64_t& uniqueId) {
        auto it = std::remove_if(m_sceneBuffer.begin(), m_sceneBuffer.end(),
            [&uniqueId](const std::shared_ptr<Scene>& scene) {
                return scene->getUniqueId() == uniqueId;
            });
        m_sceneBuffer.erase(it, m_sceneBuffer.end());
    }

    void State::clearSceneBuffer() {
        m_sceneBuffer.clear();
    }

    void State::processInitializeScenes()
    {
        for (const auto& item : m_initialBuffer) {
            std::visit([this](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, uint64_t>) {
                    queueSceneById(arg);
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    queueSceneByName(arg);
                }
            }, item);
        }
    }

}

