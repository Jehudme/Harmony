#include "pch.h"
#include "State.h"
#include "Scene.h"
#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "SceneManagement.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace Harmony::Scenes
{
	State::State(const Utilities::Configuration& configuration, Engine& engine) :
		engine(engine) 
	{
		const Utilities::UUIDList scenesIds = configuration.get<Utilities::UUIDList>({ "scenes" }).value_or(Utilities::UUIDList());
		
		for (const auto& sceneId : scenesIds) 
			if (std::shared_ptr<Scene> scene = engine.sceneManagement->create(sceneId))
				scenes_.insert({ sceneId, scene });
	}

	State::~State() 
	{
		for (const auto& scene : scenes_)
			engine.sceneManagement->remove(scene.first);
	}

	void State::internalDraw(sf::RenderTarget& renderTarget) const
	{		
		// Create a sorted vector of scenes by drawOrder
		std::vector<std::shared_ptr<Scene>> sortedScenes;
		sortedScenes.reserve(scenes_.size());
		
		for (const auto& [id, scene] : scenes_) {
			sortedScenes.push_back(scene);
		}
		
		// Sort by drawOrder (lower values drawn first, i.e., in background)
		std::sort(sortedScenes.begin(), sortedScenes.end(), 
			[](const std::shared_ptr<Scene>& a, const std::shared_ptr<Scene>& b) {
				return a->drawOrder < b->drawOrder;
			});
		
		// Draw scenes in sorted order
		for (const auto& scene : sortedScenes) {
			scene->internalDraw(renderTarget);
		}
	}

	void State::update(float deltaTime) 
	{
		for (const auto& scene : scenes_) 
			scene.second->update(deltaTime);
	}

	void State::addScene(std::shared_ptr<Scene> scene)
	{
		if (scene) {
			scenes_.insert({ scene->sceneId, scene });
			HARMONY_INFO("Scene {} added to state (drawOrder: {})", scene->sceneId, scene->drawOrder);
		}
	}

	void State::removeScene(Utilities::UUID sceneId)
	{
		if (scenes_.erase(sceneId)) {
			HARMONY_INFO("Scene {} removed from state", sceneId);
		}
		else {
			HARMONY_WARN("Attempted to remove non-existent scene {} from state", sceneId);
		}
	}
}
