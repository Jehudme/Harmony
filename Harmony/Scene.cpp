#include "pch.h"
#include "Scene.h"
#include "Object.h"

Harmony::Core::Scene::Scene(const uint64_t& uniqueId) : 
	Object(uniqueId), sceneGraph(Object::create<SceneNode>()) {
	sceneGraph->currentScene = this;
}

void Harmony::Core::Scene::update(const sf::Time& time) {
	sceneGraph->update(time);
}

void Harmony::Core::Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
	renderTarget.setView(view);
	renderTarget.draw(*sceneGraph, states);
}

void Harmony::Core::Scene::onEnter()
{
	sceneGraph->onEnter(*this);
	onEnterCurrent();
}

void Harmony::Core::Scene::onExit()
{
	sceneGraph->onExit(*this);
	onExitCurrent();
}

void Harmony::Core::Scene::onEnterCurrent() {}

void Harmony::Core::Scene::onExitCurrent() {}
