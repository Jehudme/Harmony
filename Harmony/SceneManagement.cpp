#include "pch.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "Scene.h"

namespace Harmony::Management
{
	SceneManager::SceneManager(Engine& engine) :
		engine(engine) {}

	SceneManager::~SceneManager() = default;

	std::shared_ptr<Scenes::Scene> SceneManager::create(const Utilities::UUID sceneId) 
	{
		const std::string sceneKey = std::to_string(sceneId);
		const std::optional<Utilities::Configuration> configuration = engine.configuration.subsection({ "scenes", sceneKey });
		std::lock_guard<std::mutex> lock(mutex_);

		if (configuration.has_value() && scenes_.contains(sceneId)) 
		{
			std::shared_ptr<Scenes::Scene> scene = std::make_shared<Scenes::Scene>(configuration.value(), sceneId, engine);
			scenes_[sceneId] = scene->weak_from_this();

			return scene;
		}

		return nullptr;
	}

	void SceneManager::remove(const Utilities::UUID sceneId) 
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (scenes_.contains(sceneId)) 
		{
			scenes_.erase(sceneId);
		}
	}

	std::shared_ptr<Scenes::Scene> SceneManager::get(const Utilities::UUID sceneId) const
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (scenes_.contains(sceneId))
			return scenes_.at(sceneId).lock();

		return nullptr;
	}
}
