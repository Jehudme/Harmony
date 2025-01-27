#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Configuration.h"

namespace harmony::core {

    Scene::Scene(const Configuration& configuration)
        : Object(configuration) {
        reset();

		if (const auto view = configuration.getData({ "View" })) {
			this->view = sf::View(sf::FloatRect(view.value()[0], view.value()[1], view.value()[2], view.value()[3]));
		}
        
		if (const auto sceneGraph = configuration.getData({ "SceneGraph" })) {
			for (const auto& node : sceneGraph.value()) {
				this->sceneGraph->attachChild(SceneNode::create(Configuration(node)));
			}
		}

		if (const auto drawScript = configuration.getData<std::string>({ "DrawScript" })) {
			this->m_drawScript = DrawScript::getScript(drawScript.value());
		}
		if (const auto updateScript = configuration.getData<std::string>({ "UpdateScript" })) {
			this->m_updateScript = UpdateScript::getScript(updateScript.value());
		}

        if (const auto createScript = configuration.getData<std::string>({ "CreateScript" })) {
            this->m_createScript = CreateScript::getScript(createScript.value());
        }
        if (const auto destroyScript = configuration.getData<std::string>({ "DestroyScript" })) {
            this->m_destroyScript = DestroyScript::getScript(destroyScript.value());
        }
        if (const auto enterScript = configuration.getData<std::string>({ "EnterScript" })) {
            this->m_enterScript = EnterScript::getScript(enterScript.value());
        }
        if (const auto exitScript = configuration.getData<std::string>({ "ExitScript" })) {
            this->m_exitScript = ExitScript::getScript(exitScript.value());
        }

        if (m_createScript) {
            m_createScript->execute(this);
        }
    }

    Scene::~Scene()
    {
        if (m_destroyScript) {
            m_destroyScript->execute(shared_from_this());
        }
    }

    void Scene::update(const sf::Time& time, EventQueue& eventQueue) {
		if (m_updateScript) {
			m_updateScript->execute(shared_from_this(), time, eventQueue);
		}

        sceneGraph->update(time, eventQueue);
    }

    void Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (m_drawScript) {
            m_drawScript->execute(const_cast<Scene*>(this)->shared_from_this(), renderTarget, states);
        }
        renderTarget.setView(view);
        renderTarget.draw(*sceneGraph, states);
    }

    void Scene::reset() {
        sceneGraph = utilities::create<SceneNode>(Configuration());
		sceneGraph->currentScene = this;
    }

    void Scene::onEnter() {
		if (m_enterScript) {
			m_enterScript->execute(shared_from_this());
		}
        sceneGraph->onEnter(*this);
    }

    void Scene::onExit() {
		if (m_exitScript) {
			m_exitScript->execute(shared_from_this());
		}
        reset();
    }
}
