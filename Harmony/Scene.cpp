#include "pch.h"
#include "Scene.h"

namespace Harmony::Internals {
	Scene::Scene(const Configuration& configuration, Engine& engine) : 
		configuration_(configuration),
		engine_(engine) {
	}

	void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (onPreDrawFunction_) { onPreDrawFunction_(*const_cast<Scene*>(this), target, states); }
		if (onPostDrawFunction_) { onPostDrawFunction_(*const_cast<Scene*>(this), target, states); }
	}
	void Scene::update(const sf::Time deltaTime, TaskManagement& taskManagement) {
		if (onPreUpdateFunction_) { onPreUpdateFunction_(*this, deltaTime, taskManagement); }
		if (onPostUpdateFunction_) { onPostUpdateFunction_(*this, deltaTime, taskManagement); }
	}
}
