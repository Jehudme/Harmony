#include "pch.h"
#include "Configuration.h"
#include "State.h"
#include "Scene.h"
#include "Engine.h"
#include "TaskManagement.h"
#include "SceneManagement.h"

namespace Harmony::Internals {

	State::State(const Configuration& configuration, Engine& engine) :
		engine_(engine) {
		const auto scenesIds_ = configuration.get<Utilities::UUIDList>({ "scenes" }).value_or(Utilities::UUIDList());
		
		for (const auto& sceneId : scenesIds_) {
			engine.sceneManagement->create(sceneId);

			if (std::optional<Scene> scene = engine.sceneManagement->get(sceneId)) {
				scenes_.insert({ sceneId, std::ref(scene.value()) });
			}
		}
	}


	State::~State() {
		for (const auto& scene : scenes_) {
			engine_.sceneManagement->destroy(scene.first);
		}
	}

	void State::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto& scene : scenes_) {
			scene.second.get().draw(target, states);
		}
	}

	void State::update(const sf::Time deltaTime, TaskManagement& taskManagement) {
		for (auto& scene : scenes_) {
			scene.second.get().update(deltaTime, taskManagement);
		}
	}
}
