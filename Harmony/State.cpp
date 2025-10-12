#include "pch.h"
#include "Configuration.h"
#include "State.h"
#include "Scene.h"
#include "Engine.h"
#include "TaskManagement.h"
#include "SceneManagement.h"

namespace Harmony::Internals{

	State::State(const Configuration& configuration, Engine& engine) :
		engine_(engine) {
		std::vector<std::uint64_t> scenesIds = configuration.get<std::vector<std::uint64_t>>({ "scenes" }).value_or({});
		for (const auto& sceneId : scenesIds) {
			auto scene = engine.sceneManagement->load(sceneId);
			if (scene) { scenes_.emplace_back(scene); }
		}
	}

	void State::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (const auto& scene : scenes_) {
			scene->draw(target, states);
		}
	}

	void State::update(const sf::Time deltaTime, TaskManagement& taskManagement) {
		for (const auto& scene : scenes_) {
			scene->update(deltaTime, taskManagement);
		}
	}
}
