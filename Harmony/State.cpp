#include "pch.h"
#include "State.h"
#include "Scene.h"
#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "SceneManagement.h"

namespace Harmony::Internals {

	State::State(const Configuration& configuration, Engine& engine) :
		engine(engine) 
	{
		const Utilities::UUIDList scenesIds = configuration.get<Utilities::UUIDList>({ "scenes" }).value_or(Utilities::UUIDList());
		
		for (const auto& sceneId : scenesIds) {
			engine.sceneManagement->create(sceneId);

			if (std::shared_ptr<Scene> scene = engine.sceneManagement->get(sceneId)) {
				scenes_.insert({ sceneId, engine.sceneManagement->get(sceneId) });
			}
		}
	}


	State::~State() {
		for (const auto& scene : scenes_) {
			engine.sceneManagement->destroy(scene.first);
		}
	}

	void State::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto& scene : scenes_) {
			scene.second->draw(target, states);
		}
	}

	void State::update(const sf::Time deltaTime, TaskManagement& taskManagement) {
		for (auto& scene : scenes_) {
			scene.second->update(deltaTime, taskManagement);
		}
	}
}
