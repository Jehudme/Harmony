#pragma once
#include "Scene.h"
#include <stack>
#include "Configuration.h"
#include "Script.h"

namespace harmony::core {
    class State : public Object, public sf::Drawable {
    public:
        State(const Configuration& configuration);
        ~State();

        void update(const sf::Time& time, EventQueue& eventQueue);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

        void onEnter();
        void onExit();

        void addScene(std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getScene(const std::string& name) const;
        void pushScene(const std::string& name);
        void popScene();
		void addToBuffer(const std::string& name);
        void clearBuffer();
        bool isSceneActive(const std::string& name) const;

    private:
        std::map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::stack<std::shared_ptr<Scene>> m_renderBuffer;

		std::shared_ptr<DrawScript> m_drawScript;
		std::shared_ptr<UpdateScript> m_updateScript;
		std::shared_ptr<CreateScript> m_createScript;
        std::shared_ptr<DestroyScript> m_destroyScript;
        std::shared_ptr<EnterScript> m_enterScript;
        std::shared_ptr<ExitScript> m_exitScript;

		std::vector<std::string> m_initialScenes;
    };
}