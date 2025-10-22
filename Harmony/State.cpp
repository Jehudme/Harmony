#include "pch.h"
#include "State.h"
#include "Scene.h"
#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "SceneManagement.h"
#include <SFML/Graphics.hpp>

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
		for (const auto& scene : scenes_)
			scene.second->internalDraw(renderTarget);
	}

	void State::update(float deltaTime) 
	{
		for (const auto& scene : scenes_) 
			scene.second->update(deltaTime);
	}
}
