#include "pch.h"
#include "State.h"
#include "TaskQueue.h"
#include "Scene.h"
#include "Configuration.h"
#include "Script.h"
#include <stdexcept>

namespace Harmony
{
    namespace
    {
        // Configuration keys
        constexpr const char* CONFIG_SCENES = "Scenes";
        constexpr const char* CONFIG_INITIAL_SCENES = "InitialScenes";
        constexpr const char* CONFIG_SCRIPT = "Script";

        // Error messages
        constexpr const char* ERROR_SCENE_NOT_FOUND = "Scene not found: ";
        constexpr const char* ERROR_NULL_SCENE = "Scene is null and cannot be added.";
    }

    State::State(std::shared_ptr<Configuration> configuration)
        : Object(configuration) {

        // Load scenes from configuration
        if (const auto scenesConfigurationsData = configuration->get({ CONFIG_SCENES })) {
            for (const auto& sceneConfigurations : scenesConfigurationsData.value()) {
                addScene(create<Scene>(create<Configuration>(sceneConfigurations)));
            }
        }

        // Load initial scenes from configuration
        if (const auto initialScenesData = configuration->get({ CONFIG_INITIAL_SCENES })) {
            for (const auto& sceneName : initialScenesData.value()) {
                initialBuffer_.push_back(sceneName.get<std::string>());
            }
        }

        // Load script from configuration
        if (const auto scriptName = configuration->get<std::string>({ CONFIG_SCRIPT })) {
            script_ = Harmony::find<Script>(scriptName.value());
        }
    }

    void State::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (script_) {
            script_->onDraw(shared_from_this(), renderTarget, states);
        }

        for (const auto& scene : sceneBuffer_) {
            renderTarget.draw(*scene, states);
        }
    }

    void State::update(const sf::Time& time, TaskQueue& taskQueue) {
        if (script_) {
            script_->onUpdate(shared_from_this(), time, taskQueue);
        }

        for (const auto& scene : sceneBuffer_) {
            scene->update(time, taskQueue);
        }
    }

    void State::onEnter() {
        processInitializeScenes();

        for (const auto& scene : sceneBuffer_) {
            scene->onEnter();
        }

        if (script_) {
            script_->onEnter(shared_from_this());
        }
    }

    void State::onExit() {
        if (script_) {
            script_->onExit(shared_from_this());
        }

        for (const auto& scene : sceneBuffer_) {
            scene->onExit();
        }

        clearSceneBuffer();
    }

    std::shared_ptr<Scene> State::getScene(const std::string& name) const {
        for (const auto& scene : scenes_) {
            if (scene->getName() == name) {
                return scene;
            }
        }
        throw std::runtime_error(ERROR_SCENE_NOT_FOUND + name);
    }

    std::shared_ptr<Scene> State::getScene(uint64_t uniqueId) const {
        for (const auto& scene : scenes_) {
            if (scene->getUniqueId() == uniqueId) {
                return scene;
            }
        }
        throw std::runtime_error(ERROR_SCENE_NOT_FOUND + std::to_string(uniqueId));
    }

    void State::addScene(std::shared_ptr<Scene> scene) {
        if (!scene) {
            throw std::invalid_argument(ERROR_NULL_SCENE);
        }
        scenes_.push_back(scene);
    }

    void State::removeScene(const std::string& name) {
        scenes_.erase(std::remove_if(scenes_.begin(), scenes_.end(),
            [&name](const std::shared_ptr<Scene>& scene) {
                return scene->getName() == name;
            }), scenes_.end());
    }

    void State::removeScene(uint64_t uniqueId) {
        scenes_.erase(std::remove_if(scenes_.begin(), scenes_.end(),
            [uniqueId](const std::shared_ptr<Scene>& scene) {
                return scene->getUniqueId() == uniqueId;
            }), scenes_.end());
    }

    void State::queueScene(const std::string& name) {
        for (const auto& scene : scenes_) {
            if (scene->getName() == name) {
                sceneBuffer_.push_back(scene);
                return;
            }
        }
        throw std::runtime_error(ERROR_SCENE_NOT_FOUND + name);
    }

    void State::queueScene(uint64_t uniqueId) {
        for (const auto& scene : scenes_) {
            if (scene->getUniqueId() == uniqueId) {
                sceneBuffer_.push_back(scene);
                return;
            }
        }
        throw std::runtime_error(ERROR_SCENE_NOT_FOUND + std::to_string(uniqueId));
    }

    void State::removeQueuedScene(const std::string& name) {
        sceneBuffer_.erase(std::remove_if(sceneBuffer_.begin(), sceneBuffer_.end(),
            [&name](const std::shared_ptr<Scene>& scene) {
                return scene->getName() == name;
            }), sceneBuffer_.end());
    }

    void State::removeQueuedScene(uint64_t uniqueId) {
        sceneBuffer_.erase(std::remove_if(sceneBuffer_.begin(), sceneBuffer_.end(),
            [uniqueId](const std::shared_ptr<Scene>& scene) {
                return scene->getUniqueId() == uniqueId;
            }), sceneBuffer_.end());
    }

    void State::clearSceneBuffer() {
        sceneBuffer_.clear();
    }

    void State::processInitializeScenes() {
        for (const auto& item : initialBuffer_) {
            std::visit([this](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, uint64_t>) {
                    queueScene(arg);
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    queueScene(arg);
                }
                }, item);
        }
    }
}