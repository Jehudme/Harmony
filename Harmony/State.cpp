#include "pch.h"
#include "State.h"
#include "Configuration.h"
#include <memory>
#include <string>
#include "Event.h"
#include "Object.h"
#include "Scene.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

namespace harmony::core {

    State::State(const Configuration& configuration)
        : Object(configuration) {

        if (const auto scenes = configuration.getData({ "Scenes" })) {
            for (const auto& scene : scenes.value()) {
                addScene(utilities::create<Scene>(Configuration(scene)));
            }
        }

		if (const auto initialScenes = configuration.getData({ "InitialScenes" })) {
			m_initialScenes.reserve(initialScenes.value().size());
			for (const auto& scene : initialScenes.value()) {
				m_initialScenes.push_back(scene.get<std::string>());
			}
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

    State::~State()
    {
        if(m_destroyScript){
            m_destroyScript->execute(shared_from_this());
        }
    }

    void State::update(const sf::Time& time, EventQueue& eventQueue) {
		if (m_updateScript) {
			m_updateScript->execute(shared_from_this(), time, eventQueue);
		}
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->update(time, eventQueue);
        }
    }

    void State::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        if (m_drawScript) {
            auto nonConstThis = const_cast<State*>(this)->shared_from_this();
            m_drawScript->execute(nonConstThis, renderTarget, states);
        }

        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->draw(renderTarget, states);
        }
    }


    void State::onEnter() {
		if (m_enterScript) {
			m_enterScript->execute(shared_from_this());
		}
        for (const auto& scene : m_initialScenes) {
            pushScene(scene);
        }
    }

	void State::onExit() {
		if (m_exitScript) {
			m_exitScript->execute(shared_from_this());
		}

		if (!m_renderBuffer.empty()) {
			m_renderBuffer.top()->onExit();
		}
		clearBuffer();
    }

    void State::addScene(std::shared_ptr<Scene> scene) {
        if (scene) {
            m_scenes[scene->name] = scene;
        }
    }

    std::shared_ptr<Scene> State::getScene(const std::string& name) const {
        auto it = m_scenes.find(name);
        return (it != m_scenes.end()) ? it->second : nullptr;
    }

    void State::pushScene(const std::string& name) {
        if (auto scene = getScene(name)) {
            m_renderBuffer.push(scene);
            scene->onEnter();
        }
    }

    void State::popScene() {
        if (!m_renderBuffer.empty()) {
            auto currentScene = m_renderBuffer.top();
            currentScene->onExit();
            m_renderBuffer.pop();
        }
    }

    void State::addToBuffer(const std::string& name)
	{
		if (auto scene = getScene(name)) {
		    m_renderBuffer.push(scene);
		    scene->onEnter();
        } else {
			throw std::runtime_error("Scene with name '" + name + "' not found.");
        }
    }

    void State::clearBuffer() {
        while (!m_renderBuffer.empty()) {
            m_renderBuffer.pop();
        }
    }

    bool State::isSceneActive(const std::string& name) const {
        return m_scenes.find(name) != m_scenes.end();
    }

}
