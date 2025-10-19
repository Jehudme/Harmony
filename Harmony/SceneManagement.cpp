#include "pch.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "Scene.h"
#include "Logger.h"

namespace Harmony::Management
{
    SceneManager::SceneManager(Engine& engine): engine(engine)  { 
        HARMONY_TRACE("SceneManager created"); 
    }

    SceneManager::~SceneManager() {
        HARMONY_INFO("SceneManager destroyed, {} scenes tracked", scenes_.size());
    }

    std::shared_ptr<Scenes::Scene> SceneManager::create(const Utilities::UUID sceneId)
    {
        const std::string sceneKey = std::to_string(sceneId);
        const auto configuration = engine.configuration.subsection({ "scenes", sceneKey });

        std::lock_guard<std::shared_mutex> lock(mutex_);

        if (!configuration.has_value()) {
            throw Exceptions::SceneConfigurationNotFoundError(sceneKey);
		}
        if (scenes_.contains(sceneId)) {
            HARMONY_WARN("Scene [{}] already exists, overriding", sceneKey);
        }

        std::shared_ptr<Scenes::Scene> scene = std::make_shared<Scenes::Scene>(configuration.value(), sceneId, engine);
        scenes_[sceneId] = scene->weak_from_this();

        HARMONY_INFO("Scene [{}] created successfully", sceneKey);
        return scene;
    }

    void SceneManager::remove(const Utilities::UUID sceneId)
    {
        std::lock_guard<std::shared_mutex> lock(mutex_);

        if (scenes_.contains(sceneId))  scenes_.erase(sceneId);
        else HARMONY_WARN("Attempted to remove non-existent scene [{}]", sceneId);
    }

    std::shared_ptr<Scenes::Scene> SceneManager::find(const Utilities::UUID sceneId) const {
        std::shared_lock lock(mutex_);

        auto it = scenes_.find(sceneId);
        if (it != scenes_.end()) {
            auto scene = it->second.lock();
            if (scene) return scene;
            throw Exceptions::ExpiredSceneError(sceneId);
        }

        HARMONY_WARN("Scene [{}] not found", sceneId);
        return nullptr;
    }
}

namespace Harmony::Exceptions
{
    SceneConfigurationNotFoundError::SceneConfigurationNotFoundError(const std::string& sceneKey)
        : std::runtime_error("Missing scene configuration: " + sceneKey) { HARMONY_ERROR(what()); }
    
    ExpiredSceneError::ExpiredSceneError(const Utilities::UUID sceneId)
		: std::runtime_error("Scene expired: " + std::to_string(sceneId)) { HARMONY_ERROR(what()); }
}