#include "pch.h"
#include "SceneNode.h"
#include <algorithm>
#include <stdexcept>

namespace Harmony::Core {

    SceneNode::SceneNode(const uint64_t& uniqueId)
        : Object(uniqueId), isDrawEnable(true), isUpdateEnable(true), parentNode(nullptr),
        currentScene(nullptr), rotationVelocity(0), rotationAcceleration(0)
    {
    }

    SceneNode::~SceneNode() {
        for (auto child : children) {
            child->parentNode = nullptr;
        }
        children.clear();
    }

    void SceneNode::attachChild(const std::shared_ptr<SceneNode> child) {
        if (child) {
            LOG_TRACE(Logger::core, "Attaching child node to SceneNode with uniqueId: {}", uniqueId);
            child->parentNode = this;
            child->currentScene = currentScene;
            children.push_back(child);
        }
        else {
            LOG_WARN(Logger::core, "Attempted to attach a null child to SceneNode with uniqueId: {}", uniqueId);
        }
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild(const SceneNode& child) {
        LOG_TRACE(Logger::core, "Detaching child node from SceneNode with uniqueId: {}", uniqueId);

        try {
            auto found = std::find_if(children.begin(), children.end(),
                [&child](const std::shared_ptr<SceneNode>& ptr) { return ptr.get() == &child; });

            if (found != children.end()) {
                auto detachedChild = *found;
                detachedChild->parentNode = nullptr;
                detachedChild->currentScene = nullptr;
                children.erase(found);
                return detachedChild;
            }
            else {
                LOG_WARN(Logger::core, "Child node not found in SceneNode of type '{}' with uniqueId: {}", objectType, uniqueId);
            }
        }
        catch (const std::exception& e) {
            LOG_ERROR(Logger::core, "Exception during child detachment: {}", e.what());
        }

        return nullptr;
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild() {
        if (parentNode) {
            LOG_TRACE(Logger::core, "Detaching from parent node for SceneNode of type '{}' with uniqueId: {}", objectType, uniqueId);
            parentNode->detachChild(*this);
        }
        return std::static_pointer_cast<SceneNode>(shared_from_this());
    }

    void SceneNode::enableDraw(const bool option) {
        LOG_TRACE(Logger::core, "Setting draw enable to {} for SceneNode of type '{}' with uniqueId: {}", option, objectType, uniqueId);
        isDrawEnable = option;
    }

    void SceneNode::enableUpdate(const bool option) {
        LOG_TRACE(Logger::core, "Setting update enable to {} for SceneNode with uniqueId: {}", option, uniqueId);
        isUpdateEnable = option;
    }

    sf::Transform SceneNode::getGlobalTransform() const {
        LOG_TRACE(Logger::core, "Calculating global transform for SceneNode with uniqueId: {}", uniqueId);
        sf::Transform transform = getTransform();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            transform *= currentParent->getTransform();
            currentParent = currentParent->parentNode;
        }
        return transform;
    }

    sf::Vector2f SceneNode::getGlobalPosition() const {
        LOG_TRACE(Logger::core, "Calculating global position for SceneNode with uniqueId: {}", uniqueId);
        sf::Vector2f position = getPosition();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            position += currentParent->getPosition();
            currentParent = currentParent->parentNode;
        }
        return position;
    }

    sf::FloatRect SceneNode::getGlobalBounds() const {
        LOG_TRACE(Logger::core, "Calculating global bounds for SceneNode with uniqueId: {}", uniqueId);
        try {
            const sf::Transform globalTransform = getGlobalTransform();

            if (auto rectangle = std::dynamic_pointer_cast<sf::RectangleShape>(drawable)) {
                return globalTransform.transformRect(rectangle->getGlobalBounds());
            }

            else if (auto circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable)) {
                return globalTransform.transformRect(circle->getLocalBounds());
            }

            else if (auto sprite = std::dynamic_pointer_cast<sf::Sprite>(drawable)) {
                return globalTransform.transformRect(sprite->getGlobalBounds());
            }

            else if (auto text = std::dynamic_pointer_cast<sf::Text>(drawable)) {
                return globalTransform.transformRect(text->getGlobalBounds());
            }

            else if (auto convex = std::dynamic_pointer_cast<sf::ConvexShape>(drawable)) {
                return globalTransform.transformRect(convex->getGlobalBounds());
            }

            else if (auto vertexArray = std::dynamic_pointer_cast<sf::VertexArray>(drawable)) {
                if (vertexArray->getVertexCount() > 0) {
                    sf::FloatRect bounds;
                    for (size_t i = 0; i < vertexArray->getVertexCount(); ++i) {
                        bounds = bounds == sf::FloatRect() ? sf::FloatRect(vertexArray->operator[](i).position, { 0.f, 0.f }) : bounds;
                        bounds.left = std::min(bounds.left, vertexArray->operator[](i).position.x);
                        bounds.top = std::min(bounds.top, vertexArray->operator[](i).position.y);
                        bounds.width = std::max(bounds.width, vertexArray->operator[](i).position.x - bounds.left);
                        bounds.height = std::max(bounds.height, vertexArray->operator[](i).position.y - bounds.top);
                    }
                    return globalTransform.transformRect(bounds);
                }
            }

            throw std::runtime_error("no matching drawable type is found");
        }
        catch (const std::exception& e) {
            LOG_ERROR(Logger::core, "Exception during global bounds calculation: {}", e.what());
            return sf::FloatRect();
        }
    }

    void SceneNode::draw(sf::RenderTarget& renderTarget, sf::RenderStates state) const {
        if (isDrawEnable) {
            LOG_TRACE(Logger::core, "Drawing SceneNode with uniqueId: {}", uniqueId);
            state.transform *= getTransform();
            drawCurrent(renderTarget, state);

            for (const auto& child : children) {
                child->draw(renderTarget, state);
            }
        }
    }

    void SceneNode::update(const sf::Time& time, EventPool& eventPool) {
        if (isUpdateEnable) {
            LOG_TRACE(Logger::core, "Updating SceneNode with uniqueId: {}", uniqueId);
            updateCurrent(time, eventPool);
            updateTransform(time);

            for (const auto& child : children) {
                child->update(time, eventPool);
            }
        }
    }

    void SceneNode::updateTransform(const sf::Time& time) {
        float deltaTime = time.asSeconds();

        move(
            positionVelocity.x * deltaTime + 0.5f * positionAcceleration.x * deltaTime * deltaTime,
            positionVelocity.y * deltaTime + 0.5f * positionAcceleration.y * deltaTime * deltaTime
        );

        rotate(rotationVelocity * deltaTime + 0.5f * rotationAcceleration * deltaTime * deltaTime);

        positionVelocity += (positionAcceleration * deltaTime);
        rotationVelocity += (rotationAcceleration * deltaTime);
    }

    void SceneNode::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates state) const {
        if (drawable) {
            renderTarget.draw(*drawable, state);
        }
    }

    void SceneNode::updateCurrent(const sf::Time& time, EventPool& eventPool) {
    }

    void SceneNode::onEnterCurrent(Scene& scene)
    {
    }

    void SceneNode::onExitCurrent(Scene& scene)
    {
    }

    void SceneNode::onEnter(Scene& scene) {
        onEnterCurrent(scene);
        for (auto child : children)
            child->onEnter(scene);
    }

    void SceneNode::onExit(Scene& scene) {
        onExitCurrent(scene);
        for (auto child : children)
            onExit(scene);
    }

    bool SceneNode::intersect(const std::shared_ptr<Core::SceneNode> target) {
        return intersect(std::static_pointer_cast<SceneNode>(shared_from_this()), target);
    }

    bool SceneNode::intersect(const std::shared_ptr<Core::SceneNode> node1, const std::shared_ptr<Core::SceneNode> node2) {
        return node1->getGlobalBounds().intersects(node2->getGlobalBounds());
    }

}
