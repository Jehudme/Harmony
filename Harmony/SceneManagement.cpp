#include "pch.h"
#include "SceneManagement.h"
#include "Configuration.h"
#include "Engine.h"
#include "Scene.h"

namespace Harmony::Internals {
	SceneManagement::SceneManagement(Engine& engine) :
		engine_(engine) {
	}

	std::shared_ptr<Scene> SceneManagement::create(const Utilities::UUID sceneId) {
		const std::string sceneKey = std::to_string(sceneId);
		
		if (const std::optional<Configuration> configuration = engine_.configuration.subsection({ "scenes", sceneKey })) {
			std::shared_ptr<Scene> scene = std::make_shared<Scene>(configuration.value(), engine_);
			scenes_.emplace(sceneId, scene);
			return scene;
		}

		return std::shared_ptr<Scene>(nullptr);
	}

	void SceneManagement::destroy(const Utilities::UUID sceneId) {
		scenes_.erase(sceneId);
	}

	std::shared_ptr<Scene> SceneManagement::get(const Utilities::UUID sceneId) const {
		return scenes_.contains(sceneId) ? scenes_.at(sceneId).lock() : std::shared_ptr<Scene>();
	}
}
