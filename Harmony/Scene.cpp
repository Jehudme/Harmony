#include "pch.h"
#include "Scene.h"

#include "Scene.h"

namespace Harmony::Core {

    // Constructor
    Scene::Scene(const uint64_t& uniqueId)
        : Object(uniqueId),
        m_rootNode(std::make_shared<SceneNode>()), // Initialize the root node
        m_view() { // Initialize the view (can be configured later)
        // Set up the default view or customize it here
    }

    // Update the entire scene
    void Scene::update(const sf::Time time) {
        if (m_rootNode) {
            m_rootNode->update(time); // Update the root node and all its children
        }
    }

    // Draw the entire scene
    void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (m_rootNode) {
            target.setView(m_view);   // Set the view for rendering
            m_rootNode->draw(target, states); // Draw the root node and its children
        }
    }

} // namespace Harmony::Core
