#include "pch.h"
#include "Scene.h"
#include "Group.h"
#include "Configuration.h"
#include "Script.h"

namespace Harmony {

    Scene::Scene(std::shared_ptr<Configuration> configuration)
        : Object(configuration), configuration_(configuration) {
    }

    void Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (sceneGraph_) {
            renderTarget.setView(view_);
            sceneGraph_->draw(renderTarget, states);
        }
    }

    void Scene::update(const sf::Time& time, TaskQueue& taskQueue) {
        if (script_) {
            script_->onUpdate(shared_from_this(), time, taskQueue);
        }

        if (sceneGraph_) {
            sceneGraph_->update(time, taskQueue);
        }
    }

    void Scene::onEnter() {
        initialize(configuration_);
        if (script_) {
            script_->onEnter(shared_from_this());
        }
    }

    void Scene::onExit() {
        sceneGraph_.reset();
        if (script_) {
            script_->onExit(shared_from_this());
        }
    }

    void Scene::initialize(std::shared_ptr<Configuration> configuration) {
        configureView(configuration);

        if (auto sceneGraphData = configuration->get({ CONFIG_SCENE_GRAPH })) {
            sceneGraph_ = create<Group>(create<Configuration>(sceneGraphData.value()));
        }
        else {
            sceneGraph_ = create<Group>(create<Configuration>());
        }

        if (const auto scriptName = configuration->get<std::string>({ CONFIG_SCRIPT })) {
            script_ = Harmony::find<Script>(scriptName.value());
        }
    }

    void Scene::configureView(std::shared_ptr<Configuration> configuration) {
        view_ = sf::View();

        if (auto viewConfig = configuration->get({ CONFIG_VIEW })) {
            // Set view center
            if (auto center = viewConfig.value().find(CONFIG_VIEW_CENTER); center != viewConfig.value().end()) {
                view_.setCenter(
                    center->at(CONFIG_VIEW_X).get<float>(),
                    center->at(CONFIG_VIEW_Y).get<float>()
                );
            }

            // Set view size
            if (auto size = viewConfig.value().find(CONFIG_VIEW_SIZE); size != viewConfig.value().end()) {
                view_.setSize(
                    size->at(CONFIG_VIEW_WIDTH).get<float>(),
                    size->at(CONFIG_VIEW_HEIGHT).get<float>()
                );
            }

            // Set view rotation
            if (auto rotation = viewConfig.value().find(CONFIG_VIEW_ROTATION); rotation != viewConfig.value().end()) {
                view_.setRotation(rotation->get<float>());
            }

            // Set view viewport (optional)
            if (auto viewport = viewConfig.value().find(CONFIG_VIEW_VIEWPORT); viewport != viewConfig.value().end()) {
                view_.setViewport(sf::FloatRect(
                    viewport->at(CONFIG_VIEW_LEFT).get<float>(),
                    viewport->at(CONFIG_VIEW_TOP).get<float>(),
                    viewport->at(CONFIG_VIEW_WIDTH).get<float>(),
                    viewport->at(CONFIG_VIEW_HEIGHT).get<float>()
                ));
            }
        }
    }
}