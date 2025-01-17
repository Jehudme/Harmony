#pragma once
#include "Scene.h"
#include <stack>

namespace Harmony::Core {
    class State : public Object, public sf::Drawable {
    public:
        State(const uint64_t& uniqueId = NULL);

        virtual void update(const sf::Time& time);
        virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

        virtual void onEnter();
        virtual void onExit();

        void addScene(const std::string& sceneName, std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getScene(const std::string& sceneName) const;
        void switchToScene(const std::string& sceneName);
        void popScene();
        bool isSceneActive(const std::string& sceneName) const;

    private:
        std::map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::stack<std::shared_ptr<Scene>> m_renderBuffer;
    };
}
