#pragma once
#include "Object.h"

namespace Harmony::Core {
    class Scene;

    class SceneNode : public Object, public sf::Transformable, public sf::Drawable {
    public:
        friend Scene;

        SceneNode(const uint64_t& uniqueId = Utilities::Random::generateId());

        void attachChild(std::shared_ptr<SceneNode> sceneNode);
        void detachChild(std::shared_ptr<SceneNode> sceneNode);

        sf::Vector2f getGlobalPosition();

        template<typename... Args>
        void execute(const std::function<void(SceneNode&, Args...)>& func, Args&&... args);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual void update(const sf::Time& time);
        virtual void updateCurrent(const sf::Time& time);

    protected:
        std::vector<std::shared_ptr<SceneNode>> m_children;
        SceneNode* m_parent = nullptr;
        bool m_isDrawEnabled;
        bool m_isUpdateEnabled;
    };

} // namespace Harmony
