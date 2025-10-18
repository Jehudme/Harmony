#include "pch.h"
#include "State.h"
#include "Scene.h"
#include "Engine.h"
#include "Configuration.h"
#include "TaskManagement.h"
#include "SceneManagement.h"

namespace Harmony::Internals 
{
	State::State(const Configuration& configuration, Engine& engine_) :
		engine_(engine_) 
	{
		const Utilities::UUIDList scenesIds = configuration.get<Utilities::UUIDList>({ "scenes" }).value_or(Utilities::UUIDList());
		
		for (const auto& sceneId : scenesIds) 
			if (std::shared_ptr<Scene> scene = engine_.sceneManagement->create(sceneId))
				scenes_.insert({ sceneId, scene });
	}

	State::~State() 
	{
		for (const auto& scene : scenes_)
			engine_.sceneManagement->remove(scene.first);
	}

	void State::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const auto& scene : scenes_)
			scene.second->draw(target, states);
	}

	void State::update(const sf::Time deltaTime, TaskManagement& taskManagement) 
	{
		for (const auto& scene : scenes_) 
			scene.second->update(deltaTime, taskManagement);
	}
}
