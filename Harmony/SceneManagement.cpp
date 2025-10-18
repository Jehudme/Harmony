#include "pch.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "Scene.h"

namespace Harmony::Internals {
	SceneManagement::SceneManagement(Engine& engine_) :
		engine_(engine_) {}

	SceneManagement::~SceneManagement() = default;

	std::shared_ptr<Scene> SceneManagement::create(const Utilities::UUID sceneId) 
	{
		const std::string sceneKey = std::to_string(sceneId);
		const std::optional<Configuration> configuration = engine_.configuration.subsection({ "scenes", sceneKey });
		std::lock_guard<std::mutex> lock(mutex_);

		if (configuration.has_value() && scenes_.contains(sceneId)) 
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>(configuration.value(), sceneId, engine_);
			scenes_[sceneId] = scene->weak_from_this();

			return scene;
		}

		return nullptr;
	}

	void SceneManagement::remove(const Utilities::UUID sceneId) 
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (scenes_.contains(sceneId)) 
		{
			scenes_.erase(sceneId);
		}
	}

	std::shared_ptr<Scene> SceneManagement::get(const Utilities::UUID sceneId) const
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (scenes_.contains(sceneId))
			return scenes_.at(sceneId).lock();

		return nullptr;
	}
}
