#include "pch.h"
#include "SceneNode.h"
#include "TaskQueue.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Configuration.h"

#include "Rectangle.h"
#include "Group.h"

namespace Harmony
{
    SceneNode::SceneNode(std::shared_ptr<Configuration> configuration, const bool enableOnEnter)
        : Object(configuration), configuration(configuration), parent(nullptr), scene(nullptr)
    {
        onEnter();
    }

    void SceneNode::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        drawCurrent(renderTarget, states);

        for (const auto& child : children)
        {
            child->draw(renderTarget, states);
        }
    }

    void SceneNode::update(const sf::Time& time, TaskQueue& taskQueue)
    {
        updateCurrrent(time, taskQueue);
        updateTransform(time, taskQueue);

        for (const auto& child : children)
        {
            child->update(time, taskQueue);
        }
    }

    void SceneNode::attachChild(std::shared_ptr<SceneNode> sceneNode)
    {
        if (sceneNode)
        {
            children.push_back(sceneNode);

            sceneNode->scene = scene;
            sceneNode->parent = this;
        }
    }

    void SceneNode::detachChild(std::shared_ptr<SceneNode> sceneNode)
    {
        if (sceneNode)
        {
            auto it = std::find(children.begin(), children.end(), sceneNode);
            if (it != children.end())
            {
                children.erase(it);
            }
        }
    }

    sf::Vector2f SceneNode::getGlobalPosition() const
    {
        sf::Vector2f globalPosition = getPosition();

        SceneNode* current = parent;

        while (current)
        {
            globalPosition = current->getTransform().transformPoint(globalPosition);
            current = current->parent;
        }

        return globalPosition;
    }

    std::vector<std::shared_ptr<SceneNode>> SceneNode::findChildrenByName(std::initializer_list<std::string> names)
    {
        std::vector<std::shared_ptr<SceneNode>> sceneNodes;

        for (const auto child : children)
        {
            std::vector<std::shared_ptr<SceneNode>> childSceneNodes;
            sceneNodes.insert(sceneNodes.end(), childSceneNodes.begin(), childSceneNodes.end());
        }

        return sceneNodes;
    }

    void SceneNode::onEnter()
    {
        initialize(configuration);
    }

    void SceneNode::onExit()
    {
        for (const auto child : children)
            child->onExit();
    }

    void SceneNode::initialize(std::shared_ptr<Configuration> configuration)
    {
        if (const auto position = configuration->get({ "Position" }))
        {
            setPosition(
                position.value()["X"].get<float>(),
                position.value()["Y"].get<float>()
            );
        }

        if (const auto scale = configuration->get({ "Scale" }))
        {
            setScale(
                scale.value()["X"].get<float>(),
                scale.value()["Y"].get<float>()
            );
        }

        if (const auto origine = configuration->get({ "Origine" }))
        {
            setPosition(
                origine.value()["X"].get<float>(),
                origine.value()["Y"].get<float>()
            );
        }

        if (const auto rotation = configuration->get({ "Rotation" }))
        {
            setRotation(rotation.value().get<float>());
        }

        if (const auto positionVelocity = configuration->get({ "PositionVelocity" }))
        {
            position_velocity = {
                positionVelocity.value()["X"].get<float>(),
                positionVelocity.value()["Y"].get<float>()
            };
        }

        if (const auto positionAcceleration = configuration->get({ "PositionAcceleration" }))
        {
            position_acceleration = {
                positionAcceleration.value()["X"].get<float>(),
                positionAcceleration.value()["Y"].get<float>()
            };
        }

        if (const auto rotationVelocity = configuration->get({ "RotationVelocity" }))
        {
            rotation_velocity = rotationVelocity.value().get<float>();
        }

        if (const auto rotationAcceleration = configuration->get({ "RotationAcceleration" }))
        {
            rotation_acceleration = rotationAcceleration.value().get<float>();
        }

        if (const auto childrenData = configuration->get({ "Children" })) 
        {
            for (auto childData : childrenData.value()) 
            {
                auto childConfiguration = create<Configuration>(childData);
                attachChild(create<SceneNode>(std::move(childConfiguration)));
            }
        }
    }

    void SceneNode::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const
    {
    }

    void SceneNode::updateCurrrent(const sf::Time& time, TaskQueue& taskQueue)
    {
    }

    void SceneNode::updateTransform(const sf::Time& time, TaskQueue& taskQueue)
    {
        float deltaTime = time.asSeconds();

        position_velocity += position_acceleration * deltaTime;
        move(position_velocity * deltaTime);

        rotation_velocity += rotation_acceleration * deltaTime;
        rotate(rotation_velocity * deltaTime);
    }

    template<>
    std::shared_ptr<SceneNode> create<SceneNode, std::shared_ptr<Configuration>>(std::shared_ptr<Configuration>&& configuration) {
        const std::string type = configuration->get<std::string>({ "Type" }).value_or("NONE");

        if (type == "Rectangle") {
            return create<Rectangle>(configuration);
        }

        else if (type == "Group") {
            return create<Group>(configuration);
        }

        else {
            throw std::runtime_error("Unknow Type");
        }
    };
}
