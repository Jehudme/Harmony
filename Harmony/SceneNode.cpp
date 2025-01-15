#include "pch.h"
#include "SceneNode.h"
#include <algorithm>

namespace Harmony::Core {

    // Constructor
    SceneNode::SceneNode(const uint64_t& uniqueId)
        : Object(uniqueId), m_parent(nullptr), m_isDrawEnabled(true), m_isUpdateEnabled(true) {
    }

    // Attach a child node
    void SceneNode::attachChild(std::shared_ptr<SceneNode> sceneNode) {
        if (sceneNode) {
            sceneNode->m_parent = this;
            m_children.push_back(std::move(sceneNode));
        }
    }

    // Detach a child node
    void SceneNode::detachChild(std::shared_ptr<SceneNode> sceneNode) {
        if (!sceneNode) return;

        auto it = std::find(m_children.begin(), m_children.end(), sceneNode);
        if (it != m_children.end()) {
            (*it)->m_parent = nullptr; // Reset the parent pointer
            m_children.erase(it);      // Remove the child from the vector
        }
    }

    // Get global position
    sf::Vector2f SceneNode::getGlobalPosition() {
        sf::Transform transform = getTransform();
        if (m_parent) {
            transform = m_parent->getTransform() * transform;
        }
        return transform.transformPoint(0.f, 0.f);
    }

    // Execute a function on the node and its arguments
    template<typename... Args>
    void SceneNode::execute(const std::function<void(SceneNode&, Args...)>& func, Args&&... args) {
        if (func) {
            func(*this, std::forward<Args>(args)...);
        }
    }

    // Draw the scene node
    void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        if (m_isDrawEnabled) {
            drawCurrent(target, states);
            for (const auto& child : m_children) {
                child->draw(target, states);
            }
        }
    }

    // Draw the current node
    void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    }

    // Update the scene node
    void SceneNode::update(const sf::Time& time) {
        if (m_isUpdateEnabled) {
            updateCurrent(time);
            for (auto& child : m_children) {
                child->update(time);
            }
        }
    }

    void SceneNode::updateCurrent(const sf::Time& time) {
    }

} // namespace Harmony::Core
