#include "pch.h"
#include "SceneNode.h"
#include <algorithm>

namespace Harmony::Core {

    SceneNode::SceneNode(const uint64_t& uniqueId)
        : Object(uniqueId), isDrawEnable(true), isUpdateEnable(true), m_parent(nullptr), m_scene(nullptr) {}

    SceneNode::~SceneNode() {
        for (auto& child : m_children) {
            child->m_parent = nullptr;
        }
        m_children.clear();
    }

    void SceneNode::attachChild(const std::shared_ptr<SceneNode> child) {
        if (child) {
            child->m_parent = this;
            child->m_scene = m_scene;
            m_children.push_back(child);
        }
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild(const SceneNode& child) {
        auto found = std::find_if(m_children.begin(), m_children.end(),
            [&child](const std::shared_ptr<SceneNode>& ptr) { return ptr.get() == &child; });

        if (found != m_children.end()) {
            auto detachedChild = *found;
            detachedChild->m_parent = nullptr;
            detachedChild->m_scene = nullptr;
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

    sf::Vector2f SceneNode::getGlobalPosition() {
        sf::Transform transform = getTransform();
        const SceneNode* currentParent = m_parent;

        while (currentParent) {
            transform = currentParent->getTransform() * transform;
            currentParent = currentParent->m_parent;
        }
        return transform.transformPoint(0.f, 0.f);
    }

    std::shared_ptr<SceneNode> SceneNode::getRootNode() {
        SceneNode* current = this;

        while (current->m_parent) {
            current = current->m_parent;
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

    void SceneNode::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates state) const {}

    void SceneNode::update(const sf::Time& time) {
        if (isUpdateEnable) {
            updateCurrent(time);
            for (const auto& child : m_children) {
                child->update(time);
            }
        }
    }

    void SceneNode::updateCurrent(const sf::Time& time) {}

}
