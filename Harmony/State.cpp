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
		
		int drawOrder = 0;
		for (const auto& sceneId : scenesIds) {
			if (std::shared_ptr<Scene> scene = engine.sceneManagement->create(sceneId, drawOrder++)) {
				addScene(sceneId, scene);
			}
		}
	}

	State::~State() 
	{
		std::unique_lock<std::shared_mutex> lock(scenesMutex_);
		for (const auto& scene : scenes_)
			engine.sceneManagement->remove(scene->sceneId);
		scenes_.clear();
	}

	void State::addScene(const Utilities::UUID sceneId, std::shared_ptr<Scene> scene)
	{
		std::unique_lock<std::shared_mutex> lock(scenesMutex_);
		
		// Remove any existing scene with the same ID
		scenes_.erase(
			std::remove_if(scenes_.begin(), scenes_.end(),
				[sceneId](const std::shared_ptr<Scene>& s) { return s->sceneId == sceneId; }),
			scenes_.end()
		);
		
		// Add the new scene
		scenes_.push_back(scene);
		
		// Sort by draw order to maintain proper rendering order
		std::sort(scenes_.begin(), scenes_.end(),
			[](const std::shared_ptr<Scene>& a, const std::shared_ptr<Scene>& b) {
				return a->drawOrder < b->drawOrder;
			});
		
		HARMONY_INFO("Scene {} added to state with draw order {}", sceneId, scene->drawOrder);
	}

	void State::removeScene(const Utilities::UUID sceneId)
	{
		std::unique_lock<std::shared_mutex> lock(scenesMutex_);
		
		auto it = std::remove_if(scenes_.begin(), scenes_.end(),
			[sceneId](const std::shared_ptr<Scene>& s) { return s->sceneId == sceneId; });
		
		if (it != scenes_.end()) {
			scenes_.erase(it, scenes_.end());
			HARMONY_INFO("Scene {} removed from state", sceneId);
		} else {
			HARMONY_WARN("Attempted to remove non-existent scene {} from state", sceneId);
		}
	}

	void State::removeSceneByOrder(int drawOrder)
	{
		std::unique_lock<std::shared_mutex> lock(scenesMutex_);
		
		auto it = std::find_if(scenes_.begin(), scenes_.end(),
			[drawOrder](const std::shared_ptr<Scene>& s) { return s->drawOrder == drawOrder; });
		
		if (it != scenes_.end()) {
			Utilities::UUID removedId = (*it)->sceneId;
			scenes_.erase(it);
			HARMONY_INFO("Scene {} with draw order {} removed from state", removedId, drawOrder);
		} else {
			HARMONY_WARN("No scene found with draw order {}", drawOrder);
		}
	}

	std::shared_ptr<Scene> State::getScene(const Utilities::UUID sceneId) const
	{
		std::shared_lock<std::shared_mutex> lock(scenesMutex_);
		
		auto it = std::find_if(scenes_.begin(), scenes_.end(),
			[sceneId](const std::shared_ptr<Scene>& s) { return s->sceneId == sceneId; });
		
		if (it != scenes_.end()) {
			return *it;
		}
		return nullptr;
	}

	void State::internalDraw(sf::RenderTarget& renderTarget) const
	{		
		std::shared_lock<std::shared_mutex> lock(scenesMutex_);
		
		// Scenes are already sorted by draw order, so just draw them in sequence
		for (const auto& scene : scenes_)
			scene->internalDraw(renderTarget);
	}

	void State::update(float deltaTime) 
	{
		std::shared_lock<std::shared_mutex> lock(scenesMutex_);
		
		for (const auto& scene : scenes_) 
			scene->update(deltaTime);
	}
}
