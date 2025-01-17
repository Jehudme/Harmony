#include "pch.h"
#include "Scene.h"
#include "Object.h"

Harmony::Core::Scene::Scene(const uint64_t& uniqueId) : 
	Object(uniqueId), m_sceneGraph(Object::create<SceneNode>()) {}

void Harmony::Core::Scene::update(const sf::Time& time) {
	m_sceneGraph->update(time);
}

void Harmony::Core::Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
	renderTarget.setView(view);
	renderTarget.draw(*m_sceneGraph, states);
}

void Harmony::Core::Scene::onEnter() {}

void Harmony::Core::Scene::onExit() {}
