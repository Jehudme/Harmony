#include "pch.h"
#include "SceneNode.h"
#include "TaskQueue.h"
#include "Configuration.h"
#include "Rectangle.h"
#include "Group.h"
#include "Circle.h"
#include "Script.h"
#include "Text.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

namespace Harmony {

    constexpr const char* CONFIG_SCRIPT = "Script";

    SceneNode::SceneNode(std::shared_ptr<Configuration> configuration, bool enableOnEnter)
        : Object(configuration), configuration_(configuration) {
        initialize(configuration_);
        if (script_) {
            script_->onEnter(this);
        }
    }

    SceneNode::~SceneNode() {
        if (script_) {
            script_->onExit(this);
        }
    }

    void SceneNode::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        states.transform *= getTransform();

        if (script_) {
            script_->onDraw(shared_from_this(), renderTarget, states);
        }

        drawCurrent(renderTarget, states);

        for (const auto& child : children) {
            child->draw(renderTarget, states);
        }
    }

    void SceneNode::update(const sf::Time& time, TaskQueue& taskQueue) {
        updateCurrent(time, taskQueue);
        updateTransform(time, taskQueue);

        if (script_) {
            script_->onUpdate(std::static_pointer_cast<SceneNode>(shared_from_this()), time, taskQueue);
        }

        for (const auto& child : children) {
            child->update(time, taskQueue);
        }
    }

    void SceneNode::attachChild(std::shared_ptr<SceneNode> child) {
        if (child) {
            children.push_back(child);
            child->scene_ = scene_;
            child->parent_ = this;
        }
    }

    void SceneNode::detachChild(std::shared_ptr<SceneNode> child) {
        if (child) {
            auto it = std::find(children.begin(), children.end(), child);
            if (it != children.end()) {
                children.erase(it);
            }
        }
    }

    sf::FloatRect SceneNode::getGlobalBound() const
    {
        throw std::runtime_error("Do not have bounds");
    }

    sf::Vector2f SceneNode::getGlobalPosition() const {
        sf::Vector2f globalPosition = getPosition();

        SceneNode* current = parent_;
        while (current) {
            globalPosition += current->getPosition();
            current = current->parent_;
        }

        return globalPosition;
    }

    sf::Transform SceneNode::getGlobalTransform() const
    {
        sf::Transform globalTransfom = getTransform();

        SceneNode* current = parent_;
        while (current) {
            globalTransfom *= current->getTransform();
            current = current->parent_;
        }

        return globalTransfom;
    }

    std::vector<std::shared_ptr<SceneNode>> SceneNode::findChildrenByName(std::initializer_list<std::string> names) {
        std::vector<std::shared_ptr<SceneNode>> result;
        for (const auto& child : children) {
            // Add logic to filter children by name if needed
            result.push_back(child);
        }
        return result;
    }

    void SceneNode::initialize(std::shared_ptr<Configuration> configuration) {
        if (const auto position = configuration->get({ CONFIG_POSITION })) {
            setPosition(
                position.value()[CONFIG_VECTOR_X].get<float>(),
                position.value()[CONFIG_VECTOR_Y].get<float>()
            );
        }

        if (const auto scale = configuration->get({ CONFIG_SCALE })) {
            setScale(
                scale.value()[CONFIG_VECTOR_X].get<float>(),
                scale.value()[CONFIG_VECTOR_Y].get<float>()
            );
        }

        if (const auto origin = configuration->get({ CONFIG_ORIGIN })) {
            setOrigin(
                origin.value()[CONFIG_VECTOR_X].get<float>(),
                origin.value()[CONFIG_VECTOR_Y].get<float>()
            );
        }

        if (const auto rotation = configuration->get({ CONFIG_ROTATION })) {
            setRotation(rotation.value().get<float>());
        }

        if (const auto positionVelocity = configuration->get({ CONFIG_POSITION_VELOCITY })) {
            this->positionVelocity = {
                positionVelocity.value()[CONFIG_VECTOR_X].get<float>(),
                positionVelocity.value()[CONFIG_VECTOR_Y].get<float>()
            };
        }

        if (const auto positionAcceleration = configuration->get({ CONFIG_POSITION_ACCELERATION })) {
            this->positionAcceleration = {
                positionAcceleration.value()[CONFIG_VECTOR_X].get<float>(),
                positionAcceleration.value()[CONFIG_VECTOR_Y].get<float>()
            };
        }

        if (const auto rotationVelocity = configuration->get({ CONFIG_ROTATION_VELOCITY })) {
            this->rotationVelocity = rotationVelocity.value().get<float>();
        }

        if (const auto rotationAcceleration = configuration->get({ CONFIG_ROTATION_ACCELERATION })) {
            this->rotationAcceleration = rotationAcceleration.value().get<float>();
        }

        if (const auto childrenData = configuration->get({ CONFIG_CHILDREN })) {
            for (const auto& childData : childrenData.value()) {
                auto childConfiguration = create<Configuration>(childData);
                attachChild(create<SceneNode>(std::move(childConfiguration)));
            }
        }

        if (const auto scriptName = configuration->get<std::string>({ CONFIG_SCRIPT })) {
            script_ = Harmony::find<Script>(scriptName.value());
        }
    }

    bool SceneNode::intersect(std::shared_ptr<SceneNode> node1, std::shared_ptr<SceneNode> node2)
    {
        return node1->getGlobalBound().intersects(node2->getGlobalBound());
    }

    void SceneNode::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        // Default implementation does nothing
    }

    void SceneNode::updateCurrent(const sf::Time& time, TaskQueue& taskQueue) {
        // Default implementation does nothing
    }

    void SceneNode::updateTransform(const sf::Time& time, TaskQueue& taskQueue) {
        float deltaTime = time.asSeconds();

        positionVelocity += positionAcceleration * deltaTime;
        move(positionVelocity * deltaTime);

        rotationVelocity += rotationAcceleration * deltaTime;
        rotate(rotationVelocity * deltaTime);
    }

    template<>
    std::shared_ptr<SceneNode> create<SceneNode, std::shared_ptr<Configuration>>(std::shared_ptr<Configuration>&& configuration) {
        const std::string type = configuration->get<std::string>({ CONFIG_TYPE }).value_or("NONE");

        if (type == "Rectangle") {
            return create<Rectangle>(configuration);
        }

        if (type == "Circle") {
            return create<Circle>(configuration);
        }

        if (type == "Text") {
            return create<Text>(configuration);
        }

        else if (type == "Group") {
            return create<Group>(configuration);
        }
        else {
            throw std::runtime_error(ERROR_UNKNOWN_TYPE + type);
        }
    }
}