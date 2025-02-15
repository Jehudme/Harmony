#pragma once

#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace Harmony {

    // Constants for configuration keys
    constexpr const char* CONFIG_POSITION = "Position";
    constexpr const char* CONFIG_SCALE = "Scale";
    constexpr const char* CONFIG_ORIGIN = "Origin";
    constexpr const char* CONFIG_ROTATION = "Rotation";
    constexpr const char* CONFIG_POSITION_VELOCITY = "PositionVelocity";
    constexpr const char* CONFIG_POSITION_ACCELERATION = "PositionAcceleration";
    constexpr const char* CONFIG_ROTATION_VELOCITY = "RotationVelocity";
    constexpr const char* CONFIG_ROTATION_ACCELERATION = "RotationAcceleration";
    constexpr const char* CONFIG_CHILDREN = "Children";
    constexpr const char* CONFIG_TYPE = "Type";
    constexpr const char* CONFIG_VECTOR_X = "X";
    constexpr const char* CONFIG_VECTOR_Y = "Y";

    constexpr const char* ERROR_UNKNOWN_TYPE = "Unknown type: ";

    class TaskQueue;
    class Scene;
    struct Script;

    class SceneNode : public Object, public sf::Transformable, public sf::Drawable {
    public:
        explicit SceneNode(std::shared_ptr<Configuration> configuration, bool enableOnEnter = false);
        ~SceneNode();

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void attachChild(std::shared_ptr<SceneNode> child);
        void detachChild(std::shared_ptr<SceneNode> child);

        sf::Vector2f getGlobalPosition() const;

        std::vector<std::shared_ptr<SceneNode>> findChildrenByName(std::initializer_list<std::string> names);
        std::vector<std::shared_ptr<SceneNode>> findChildrenByName(std::initializer_list<uint64_t> uniqueIds);

        virtual void initialize(std::shared_ptr<Configuration> configuration);

    private:
        virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const;
        virtual void updateCurrent(const sf::Time& time, TaskQueue& taskQueue);

        void updateTransform(const sf::Time& time, TaskQueue& taskQueue);

        std::vector<std::shared_ptr<SceneNode>> children_;
        sf::Vector2f positionVelocity_;
        sf::Vector2f positionAcceleration_;
        float rotationVelocity_ = 0;
        float rotationAcceleration_ = 0;

        SceneNode* parent_ = nullptr;
        Scene* scene_ = nullptr;
        std::shared_ptr<Script> script_;
        std::shared_ptr<Configuration> configuration_;
    };

    template<>
    std::shared_ptr<SceneNode> create<SceneNode, std::shared_ptr<Configuration>>(std::shared_ptr<Configuration>&& configuration);
}

class SceneNode
{
};
