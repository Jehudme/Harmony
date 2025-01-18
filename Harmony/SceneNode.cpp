#include "pch.h"
#include "SceneNode.h"
#include <algorithm>

namespace Harmony::Core {

    SceneNode::SceneNode(const uint64_t& uniqueId)
        : Object(uniqueId), isDrawEnable(true), isUpdateEnable(true), parentNode(nullptr), 
        currentScene(nullptr), rotationVelocity(NULL), rotationAcceleration(NULL) {}

    SceneNode::~SceneNode() {
        for (auto& child : m_children) {
            child->parentNode = nullptr;
        }
        m_children.clear();
    }

    void SceneNode::attachChild(const std::shared_ptr<SceneNode> child) {
        if (child) {
            child->parentNode = this;
            child->currentScene = currentScene;
            m_children.push_back(child);
        }
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild(const SceneNode& child) {
        auto found = std::find_if(m_children.begin(), m_children.end(),
            [&child](const std::shared_ptr<SceneNode>& ptr) { return ptr.get() == &child; });

        if (found != m_children.end()) {
            auto detachedChild = *found;
            detachedChild->parentNode = nullptr;
            detachedChild->currentScene = nullptr;
            m_children.erase(found);
            return detachedChild;
        }
        return nullptr;
    }

    void SceneNode::enableDraw(const bool option) {
        isDrawEnable = option;
    }

    void SceneNode::enableUpdate(const bool option) {
        isUpdateEnable = option;
    }

    sf::Vector2f SceneNode::getGlobalPosition()
    {
        sf::Vector2f position = getPosition();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            position = currentParent->getPosition();
            currentParent = currentParent->parentNode;
        }
        return position;
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
            for (const auto& child : m_children) {
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

    void SceneNode::update(const sf::Time& time) {
        auto test = std::vector<std::shared_ptr<Object>>();
        if (isUpdateEnable) {
            updateCurrent(time);
            updateTransform(time);
            for (const std::shared_ptr<SceneNode> child : m_children) {
                test.push_back(child);
                child->update(time);
            }
        }
    }

    void SceneNode::updateCurrent(const sf::Time& time) {}

    void SceneNode::onEnter(Scene& scene) {
        onEnterCurrent(scene);
        for (auto child : m_children)
            child->onEnter(scene);
    }

    void SceneNode::onExit(Scene& scene) {
        onExitCurrent(scene);
        for (auto child : m_children)
            onExit(scene);
    }

    void SceneNode::onEnterCurrent(Scene& scene)
    {
    }

    void SceneNode::onExitCurrent(Scene& scene)
    {
    }
}

sf::FloatRect Harmony::Utilities::getRect(const std::shared_ptr<Core::SceneNode> node)
{
    if (!node)
        throw std::runtime_error("node is empty");

    if (auto rectangle = std::dynamic_pointer_cast<sf::RectangleShape>(node->drawable))
        return rectangle->getGlobalBounds();

    else if (auto circle = std::dynamic_pointer_cast<sf::CircleShape>(node->drawable))
        return circle->getGlobalBounds();

    else if (auto sprite = std::dynamic_pointer_cast<sf::Sprite>(node->drawable))
        return sprite->getGlobalBounds();

    else if (auto text = std::dynamic_pointer_cast<sf::Text>(node->drawable))
        return text->getGlobalBounds();

    else if (auto convex = std::dynamic_pointer_cast<sf::ConvexShape>(node->drawable))
        return convex->getGlobalBounds();

    else if (auto vertexArray = std::dynamic_pointer_cast<sf::VertexArray>(node->drawable)) {
        if (vertexArray->getVertexCount() > 0) {
            sf::FloatRect bounds;
            for (size_t i = 0; i < vertexArray->getVertexCount(); ++i) {
                bounds          = bounds == sf::FloatRect() ? sf::FloatRect(vertexArray->operator[](i).position, { 0.f, 0.f }) : bounds;
                bounds.left     = std::min(bounds.left, vertexArray->operator[](i).position.x);
                bounds.top      = std::min(bounds.top, vertexArray->operator[](i).position.y);
                bounds.width    = std::max(bounds.width, vertexArray->operator[](i).position.x - bounds.left);
                bounds.height   = std::max(bounds.height, vertexArray->operator[](i).position.y - bounds.top);
            }
            return bounds;
        }
    }

    throw std::runtime_error("no matching drawable type is found");
}

bool Harmony::Utilities::intersect(const std::shared_ptr<Core::SceneNode> node1, const std::shared_ptr<Core::SceneNode> node2)
{
    sf::Vector2f positionNode1 = node1->getGlobalPosition();
    sf::FloatRect rect1 = getRect(node1);
    
    rect1.left += positionNode1.x;
    rect1.top += positionNode1.y;

    sf::Vector2f positionNode2 = node2->getGlobalPosition();
    sf::FloatRect rect2 = getRect(node2);

    rect2.left += positionNode1.x;
    rect2.top += positionNode1.y;

    return rect1.intersects(rect2);
}
