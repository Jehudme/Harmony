#pragma once
#include "Scene.h"
#include <stack>

namespace harmony::core {
    class State : public Object, public sf::Drawable {
    public:
        State(const uint64_t& uniqueId = NULL);

        virtual void update(const sf::Time& time, EventPool& eventPool);
        virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

        virtual void create();
        virtual void destroy();

        void addScene(const std::string& sceneName, std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getScene(const std::string& sceneName) const;
        void switchToScene(const std::string& sceneName);
        void popScene();
        void clearBuffer();
        bool isSceneActive(const std::string& sceneName) const;

    private:
        std::map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::stack<std::shared_ptr<Scene>> m_renderBuffer;
    };
}
