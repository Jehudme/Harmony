#include "pch.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Configuration.h"

namespace Harmony
{
    Scene::Scene(std::shared_ptr<Configuration> configuration)
        : Object(configuration), configuration(configuration)
    {
        reset();
    }

    void Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
    {
        if (sceneGraph) {
            renderTarget.setView(view);
            sceneGraph->draw(renderTarget, states);
        }
    }

    void Scene::update(const sf::Time& time, TaskQueue& taskQueue)
    {
        if (sceneGraph) {
            sceneGraph->update(time, taskQueue);
        }
    }

    void Scene::onEnter()
    {
        reset();
        if (sceneGraph) {
            sceneGraph->onEnter();
        }
    }

    void Scene::onExit()
    {
        if (sceneGraph) {
            sceneGraph->onExit();
        }
    }

    void Scene::reset()
    {
        if (auto sceneGraphConfig = configuration->get({ "SceneGraph" })) {
            sceneGraph = create<SceneNode>(create<Configuration>());
        }
        configureViewFromConfig();
    }
    void Scene::configureViewFromConfig()
    {
        view = sf::View();
        if (auto viewConfig = configuration->get({ "View" })) {
            // Set view center
            if (auto center = viewConfig.value().find("Center"); center != viewConfig.value().end()) {
                view.setCenter(
                    center->at("X").get<float>(),
                    center->at("Y").get<float>()
                );
            }

            // Set view size
            if (auto size = viewConfig.value().find("Size"); size != viewConfig.value().end()) {
                view.setSize(
                    size->at("Width").get<float>(),
                    size->at("Height").get<float>()
                );
            }

            // Set view rotation
            if (auto rotation = viewConfig.value().find("Rotation"); rotation != viewConfig.value().end()) {
                view.setRotation(rotation->get<float>());
            }

            // Set view viewport (optional)
            if (auto viewport = viewConfig.value().find("Viewport"); viewport != viewConfig.value().end()) {
                view.setViewport(sf::FloatRect(
                    viewport->at("Left").get<float>(),
                    viewport->at("Top").get<float>(),
                    viewport->at("Width").get<float>(),
                    viewport->at("Height").get<float>()
                ));
            }
        }
    }
}