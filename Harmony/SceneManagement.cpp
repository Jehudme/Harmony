#include "pch.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "Scene.h"

namespace Harmony::Internals {
	SceneManagement::SceneManagement(Engine& engine) :
		engine(engine) {}

	SceneManagement::~SceneManagement() = default;

	std::shared_ptr<Scene> SceneManagement::create(const Utilities::UUID sceneId) 
	{
		const std::string sceneKey = std::to_string(sceneId);
		const std::optional<Configuration> configuration = engine.configuration.subsection({ "scenes", sceneKey });

		if (configuration.has_value() && scenes_.contains(sceneId)) 
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>(configuration.value(), sceneId, engine);
			scenes_[sceneId] = scene->weak_from_this();

			return scene;
		}

		return nullptr;
	}

	void SceneManagement::remove(const Utilities::UUID sceneId) 
	{
		if (scenes_.contains(sceneId)) 
			scenes_.erase(sceneId);
	}

	std::shared_ptr<Scene> SceneManagement::get(const Utilities::UUID sceneId) const 
	{
		if (scenes_.contains(sceneId))
			return scenes_.at(sceneId).lock();

		return nullptr;
	}
}
