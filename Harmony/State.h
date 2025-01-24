#pragma once
#include "Scene.h"
#include <stack>
#include "Configuration.h"

namespace harmony::core {
    class State : public Object, public sf::Drawable {
    public:
        State(const Configuration& configuration);

        void update(const sf::Time& time, EventQueue& eventQueue);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

        void onEnter();
        void onExit();

        void addScene(std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getScene(const std::string& name) const;
        void pushScene(const std::string& name);
        void popScene();
        void clearBuffer();
        bool isSceneActive(const std::string& name) const;

    private:
        std::map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::stack<std::shared_ptr<Scene>> m_renderBuffer;
    };
}
