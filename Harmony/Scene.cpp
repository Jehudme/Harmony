#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"

namespace Harmony::Internals {
	Scene::Scene(const Configuration& configuration, const Utilities::UUID sceneId, Engine& engine_) :
		configuration_(configuration),
		sceneId(sceneId),
		engine_(engine_) 
	{
	}

	Scene::~Scene()
	{
		engine_.sceneManagement->remove(sceneId);
	}

	void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
	}

	void Scene::update(const sf::Time deltaTime, TaskManagement& taskManagement) 
	{
	}
}
