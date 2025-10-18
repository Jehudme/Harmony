#include "pch.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "Scene.h"
#include "Logger.h" // for HARMONY_* macros

namespace Harmony::Errors {
    SceneManagerError::SceneManagerError(const std::string& msg)
        : std::runtime_error("SceneManager error: " + msg) {}
}

namespace Harmony::Management
{
    SceneManager::SceneManager(Engine& engine)
        : engine(engine)
    {
        HARMONY_INFO("SceneManager created");
    }

    SceneManager::~SceneManager() {
        HARMONY_INFO("SceneManager destroyed, {} scenes tracked", scenes_.size());
    }

    std::shared_ptr<Scenes::Scene> SceneManager::create(const Utilities::UUID sceneId)
    {
        const std::string sceneKey = std::to_string(sceneId);
        const auto configuration = engine.configuration.subsection({ "scenes", sceneKey });

        std::lock_guard<std::mutex> lock(mutex_);

        if (!configuration.has_value()) {
            HARMONY_ERROR("Failed to create scene [{}]: no configuration found", sceneKey);
            throw Errors::SceneManagerError("Missing configuration for scene " + sceneKey);
        }

        if (scenes_.contains(sceneId)) {
            HARMONY_WARN("Scene [{}] already exists, overriding", sceneKey);
        }

        auto scene = std::make_shared<Scenes::Scene>(configuration.value(), sceneId, engine);
        scenes_[sceneId] = scene->weak_from_this();

        HARMONY_INFO("Scene [{}] created successfully", sceneKey);
        return scene;
    }

    void SceneManager::remove(const Utilities::UUID sceneId)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (scenes_.contains(sceneId)) {
            scenes_.erase(sceneId);
            HARMONY_INFO("Scene [{}] removed", sceneId);
        }
        else {
            HARMONY_WARN("Attempted to remove non-existent scene [{}]", sceneId);
        }
    }

    std::shared_ptr<Scenes::Scene> SceneManager::get(const Utilities::UUID sceneId) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (scenes_.contains(sceneId)) {
            auto scene = scenes_.at(sceneId).lock();
            if (scene) {
                HARMONY_DEBUG("Retrieved scene [{}]", sceneId);
                return scene;
            }
            else {
                HARMONY_ERROR("Scene [{}] exists but expired (dangling weak_ptr)", sceneId);
                throw Errors::SceneManagerError("Scene expired: " + std::to_string(sceneId));
            }
        }

        HARMONY_WARN("Scene [{}] not found", sceneId);
        return nullptr;
    }
}