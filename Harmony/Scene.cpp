#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Configuration.h"

namespace harmony::core {

    Scene::Scene(const Configuration& configuration)
        : Object(configuration), sceneGraph(utilities::create<SceneNode>(Configuration())) {
        sceneGraph->currentScene = this;
    }

    void Scene::update(const sf::Time& time, EventQueue& eventQueue) {
        sceneGraph->update(time, eventQueue);
    }

    void Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        renderTarget.setView(view);
        renderTarget.draw(*sceneGraph, states);
    }

    void Scene::reset() {
        sceneGraph = utilities::create<SceneNode>(Configuration());
    }

    void Scene::onEnter() {
        onCreate();
        sceneGraph->onEnter(*this);
    }

    void Scene::onExit() {
        sceneGraph->onExit(*this);
        onDestroy();
        reset();
    }

    void Scene::onCreate() {}

    void Scene::onDestroy() {}

}
