#pragma once
#include "Scene.h"
#include <stack>

namespace harmony::core {
    class State : public Object, public sf::Drawable {
    public:
        State(const uint64_t& uniqueId);

        void update(const sf::Time& time, EventPool& eventPool);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

        void onEnter();
        void onExit();

        void addScene(std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getScene(const uint64_t& uniqueId) const;
        void pushScene(const uint64_t& uniqueId);
        void popScene();
        void clearBuffer();
        bool isSceneActive(const uint64_t& uniqueId) const;

    private:
        std::map<uint64_t, std::shared_ptr<Scene>> m_scenes;
        std::stack<std::shared_ptr<Scene>> m_renderBuffer;
    };
}
