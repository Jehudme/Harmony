#include "pch.h"
#include "Scene.h"
#include "Group.h"
#include "Configuration.h"

namespace Harmony
{
    Scene::Scene(std::shared_ptr<Configuration> configuration)
        : Object(configuration), m_configuration(configuration) {
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
        initialize(m_configuration);
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

    void Scene::initialize(std::shared_ptr<Configuration> configuration)
    {
        configure_view(configuration);
        if (auto sceneGraphData = configuration->get({ "SceneGraph" })) {
            sceneGraph = create<Group>(create<Configuration>(sceneGraphData.value()));
        }
        else
        {
            sceneGraph = create<Group>(create<Configuration>());
        }
    }

    void Scene::configure_view(std::shared_ptr<Configuration> configuration)
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