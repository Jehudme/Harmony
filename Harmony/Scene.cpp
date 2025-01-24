#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Configuration.h"

harmony::core::Scene::Scene(const std::shared_ptr<Configuration> configuration) :
	Object(uniqueId), sceneGraph(utilities::create<SceneNode>()) {
	sceneGraph->currentScene = this;
}

void harmony::core::Scene::update(const sf::Time& time, EventPool& eventPool) {
	sceneGraph->update(time, eventPool);
}

void harmony::core::Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
	renderTarget.setView(view);
	renderTarget.draw(*sceneGraph, states);
}

void harmony::core::Scene::reset()
{
	sceneGraph = utilities::create<SceneNode>();
}

void harmony::core::Scene::onEnter()
{
	onCreate();
	sceneGraph->onEnter(*this);
}

void harmony::core::Scene::onExit()
{
	sceneGraph->onExit(*this);
	onDestroy();
	reset();
}

void harmony::core::Scene::onCreate() {}

void harmony::core::Scene::onDestroy() {}
