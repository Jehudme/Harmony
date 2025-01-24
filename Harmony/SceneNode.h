#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Event.h"

namespace harmony::core
{
    class Scene;

    class SceneNode : public sf::Transformable, public sf::Drawable, public Object
    {
    public:
        SceneNode(const uint64_t& uniqueId = NULL);
        ~SceneNode();

        void attachChild(const std::shared_ptr<SceneNode> child);
        std::shared_ptr<SceneNode> detachChild(const SceneNode& child);
        std::shared_ptr<SceneNode> detachChild();

        void enableDraw(const bool option);
        void enableUpdate(const bool option);

        sf::Transform getGlobalTransform() const;
        sf::Vector2f getGlobalPosition() const;
        sf::FloatRect getGlobalBounds() const;

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates state) const override;
        void update(const sf::Time& time, EventPool& eventPool);

        void onEnter(Scene& scene);
        void onExit(Scene& scene);

        bool intersect(const std::shared_ptr<core::SceneNode> target);
        static bool intersect(const std::shared_ptr<core::SceneNode> node1, const std::shared_ptr<core::SceneNode> node2);

        friend Scene;

    private:
        virtual void onDraw(sf::RenderTarget& renderTarget, sf::RenderStates state) const;
        virtual void onUpdate(const sf::Time& time, EventPool& eventPool);

        virtual void onCreate(Scene& scene);
        virtual void onDestroy(Scene& scene);

        void updateTransform(const sf::Time& time);

    public:
        bool isDrawEnable;
        bool isUpdateEnable;

        SceneNode* parentNode;
        Scene* currentScene;

        float rotationVelocity;
        float rotationAcceleration;

        sf::Vector2f positionVelocity;
        sf::Vector2f positionAcceleration;

        std::shared_ptr<sf::Drawable> drawable;
        std::vector<std::shared_ptr<SceneNode>> children;
    };
}
