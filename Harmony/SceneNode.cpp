#include "pch.h"
#include "SceneNode.h"
#include <algorithm>
#include "iostream"

namespace Harmony::Core {

    SceneNode::SceneNode(const uint64_t& uniqueId)
        : Object(uniqueId), isDrawEnable(true), isUpdateEnable(true), parentNode(nullptr), 
        currentScene(nullptr), rotationVelocity(NULL), rotationAcceleration(NULL) {}

    SceneNode::~SceneNode() {
        for (auto& child : children) {
            child->parentNode = nullptr;
        }
        children.clear();
    }

    void SceneNode::attachChild(const std::shared_ptr<SceneNode> child) {
        if (child) {
            child->parentNode = this;
            child->currentScene = currentScene;
            children.push_back(child);
        }
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild(const SceneNode& child) {
        auto found = std::find_if(children.begin(), children.end(),
            [&child](const std::shared_ptr<SceneNode>& ptr) { return ptr.get() == &child; });

        if (found != children.end()) {
            auto detachedChild = *found;
            detachedChild->parentNode = nullptr;
            detachedChild->currentScene = nullptr;
            children.erase(found);
            return detachedChild;
        }
        return nullptr;
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild()
    {
        if(parentNode)
            parentNode->detachChild(*this);
        return std::static_pointer_cast<SceneNode>(shared_from_this());
    }

    void SceneNode::enableDraw(const bool option) {
        isDrawEnable = option;
    }

    void SceneNode::enableUpdate(const bool option) {
        isUpdateEnable = option;
    }

    sf::Transform SceneNode::getGlobalTransform() const
    {
        sf::Transform transform = getTransform();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            transform *= currentParent->getTransform();
            currentParent = currentParent->parentNode;
        }

        if (drawable) {
            if (auto rectangle = std::dynamic_pointer_cast<sf::RectangleShape>(drawable))
                transform *= rectangle->getTransform();

            else if (auto circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable))
                transform *= circle->getTransform();

            else if (auto sprite = std::dynamic_pointer_cast<sf::Sprite>(drawable))
                transform *= sprite->getTransform();

            else if (auto text = std::dynamic_pointer_cast<sf::Text>(drawable))
                transform *= text->getTransform();

            else if (auto convex = std::dynamic_pointer_cast<sf::ConvexShape>(drawable))
                transform *= convex->getTransform();

            else if (auto vertexArray = std::dynamic_pointer_cast<sf::VertexArray>(drawable)) {
                const auto globalBounds = getGlobalBounds();
                auto transfomable = sf::Transformable();
                transfomable.setPosition(getGlobalBounds().getPosition());
                transform *= transfomable.getTransform();
            }

        }
        return transform;
    }

    sf::Vector2f SceneNode::getGlobalPosition() const
    {
        sf::Vector2f position = getPosition();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            position += currentParent->getPosition();
            currentParent = currentParent->parentNode;
        }

        if (drawable) {
            if (auto rectangle = std::dynamic_pointer_cast<sf::RectangleShape>(drawable))
                position += rectangle->getPosition();

            else if (auto circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable))
                position += circle->getPosition();

            else if (auto sprite = std::dynamic_pointer_cast<sf::Sprite>(drawable))
                position += sprite->getPosition();

            else if (auto text = std::dynamic_pointer_cast<sf::Text>(drawable))
                position += text->getPosition();

            else if (auto convex = std::dynamic_pointer_cast<sf::ConvexShape>(drawable))
                position += convex->getPosition();

            else if (auto vertexArray = std::dynamic_pointer_cast<sf::VertexArray>(drawable)) {
                const auto globalBounds = getGlobalBounds();
                auto transfomable = sf::Transformable();
                transfomable.setPosition(getGlobalBounds().getPosition());
                position += transfomable.getPosition();
            }

        }
        return position;
    }

    static inline sf::FloatRect getGlobalBoundsFromPosition(sf::FloatRect bounds, sf::Vector2f position)
    {
        bounds.left += position.x;
        bounds.top += position.y;

        return bounds;
    }

    sf::FloatRect SceneNode::getGlobalBounds() const
    {
        const sf::Transform globalTransform = getGlobalTransform();

        if (auto rectangle = std::dynamic_pointer_cast<sf::RectangleShape>(drawable))
            return globalTransform.transformRect(rectangle->getGlobalBounds());

        else if (auto circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable))
            return globalTransform.transformRect(circle->getLocalBounds());

        else if (auto sprite = std::dynamic_pointer_cast<sf::Sprite>(drawable))
            return globalTransform.transformRect(sprite->getGlobalBounds());

        else if (auto text = std::dynamic_pointer_cast<sf::Text>(drawable))
            return globalTransform.transformRect(text->getGlobalBounds());

        else if (auto convex = std::dynamic_pointer_cast<sf::ConvexShape>(drawable))
            return globalTransform.transformRect(convex->getGlobalBounds());

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

    std::shared_ptr<SceneNode> SceneNode::getRootNode() {
        SceneNode* current = this;

        while (current->parentNode) {
            current = current->parentNode;
        }

        return std::static_pointer_cast<SceneNode>(shared_from_this());
    }

    void SceneNode::draw(sf::RenderTarget& renderTarget, sf::RenderStates state) const {
        if (isDrawEnable) {
            state.transform *= getTransform();
            drawCurrent(renderTarget, state);
            for (const auto& child : children) {
                child->draw(renderTarget, state);
            }
        }
    }

    void SceneNode::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates state) const {
        if(drawable)
            renderTarget.draw(*drawable, state);
    }

    void SceneNode::updateTransform(const sf::Time& time)
    {
        const float deltaTime = time.asSeconds();

        move(
            positionVelocity.x * deltaTime + 0.5f * positionAcceleration.x * powf(deltaTime, 2),
            positionVelocity.y * deltaTime + 0.5f * positionAcceleration.y * powf(deltaTime, 2)
        );

        rotate(rotationVelocity * deltaTime + 0.5f * rotationAcceleration * powf(deltaTime, 2));

        positionVelocity.x += positionAcceleration.x * deltaTime;
        positionVelocity.y += positionAcceleration.y * deltaTime;
        
        rotationVelocity += rotationAcceleration * deltaTime;
    }

    void SceneNode::update(const sf::Time& time, EventPool& eventPool) {
        if (isUpdateEnable) {
            updateCurrent(time, eventPool);
            updateTransform(time);
            for (const auto child : children) {
                child->update(time, eventPool);
            }
        }
    }

    void SceneNode::updateCurrent(const sf::Time& time, EventPool& eventPool) {}

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

    bool SceneNode::intersect(const std::shared_ptr<Core::SceneNode> node1, const std::shared_ptr<Core::SceneNode> node2)
    {
        sf::Vector2f positionNode1 = node1->getGlobalPosition();
        sf::FloatRect rect1 = node1->getGlobalBounds();

        rect1.left += positionNode1.x;
        rect1.top += positionNode1.y;

        sf::Vector2f positionNode2 = node2->getGlobalPosition();
        sf::FloatRect rect2 = node2->getGlobalBounds();

        rect2.left += positionNode1.x;
        rect2.top += positionNode1.y;

        return rect1.intersects(rect2);
    }

    void SceneNode::onEnterCurrent(Scene& scene)
    {
    }

    void SceneNode::onExitCurrent(Scene& scene)
    {
    }
}