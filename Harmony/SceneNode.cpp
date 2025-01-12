#include "pch.h"
#include "SceneNode.h"

namespace Harmony {

    SceneNode::SceneNode()
        : Object(INVALID_UNIQUE_ID), parent_(nullptr)
    {
        initialize(*DefaultConfiguration);
    }

    SceneNode::SceneNode(const uint64_t& configurationId)
        : Object(configurationId), parent_(nullptr)
    {
        initialize(*Object::find<Configuration>(configurationId));
    }

    SceneNode::SceneNode(const std::shared_ptr<Configuration>& configuration)
        : Object(configuration->getParameterOrDefault<uint64_t>("unique_id", INVALID_UNIQUE_ID)), parent_(nullptr)
    {
        initialize(*configuration);
    }

    void SceneNode::attachChild(std::shared_ptr<SceneNode> sceneNode) {
        try {
            if (!sceneNode) {
                HM_LOGGER_WARN("Attempted to attach a null child to SceneNode with ID: {}", getID());
                return;
            }

            sceneNode->parent_ = this;
            children_.emplace_back(std::move(sceneNode));
            HM_LOGGER_INFO("Child SceneNode attached to parent with ID: {}", getID());
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Failed to attach child to SceneNode with ID: {}: {}", getID(), e.what());
            throw;
        }
    }

    void SceneNode::detachChild(std::shared_ptr<SceneNode> sceneNode) {
        try {
            if (!sceneNode) {
                HM_LOGGER_WARN("Attempted to detach a null child from SceneNode with ID: {}", getID());
                return;
            }

            auto it = std::remove(children_.begin(), children_.end(), sceneNode);
            if (it != children_.end()) {
                (*it)->parent_ = nullptr;
                children_.erase(it);
                HM_LOGGER_INFO("Child SceneNode detached from parent with ID: {}", getID());
            }
            else {
                HM_LOGGER_WARN("Child SceneNode not found for detachment in parent with ID: {}", getID());
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Failed to detach child from SceneNode with ID: {}: {}", getID(), e.what());
            throw;
        }
    }

    sf::Vector2f SceneNode::getGlobalPosition() {
        try {
            // Start with the local position of this node
            sf::Vector2f globalPosition = getPosition();

            // Traverse up the hierarchy, accumulating parent transformations
            SceneNode* currentParent = parent_;
            while (currentParent != nullptr) {
                globalPosition += currentParent->getPosition();
                currentParent = currentParent->parent_;
            }

            HM_LOGGER_TRACE("Global position calculated: ({}, {})", globalPosition.x, globalPosition.y);
            return globalPosition;
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error calculating global position: {}", e.what());
            throw;
        }
    }

    template<typename... Args>
    void SceneNode::execute(const std::function<void(SceneNode&, Args...)>& func, Args&&... args) {
        try {
            HM_LOGGER_DEBUG("Executing function on SceneNode with ID: {}", getID());
            func(*this, std::forward<Args>(args)...);

            for (const auto& child : children_) {
                if (child) {
                    child->execute(func, std::forward<Args>(args)...);
                }
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error executing function on SceneNode with ID: {}: {}", getID(), e.what());
            throw;
        }
    }

    void SceneNode::addChildConfigurationID(std::shared_ptr<Configuration> configuration_parent, std::shared_ptr<Configuration> configuration_child) {
        const uint64_t& childConfigurationId = configuration_child->getID();
        try {
            auto childIds = configuration_parent->getParameterOrDefault<std::vector<uint64_t>>("child_configuration_ids", {});
            childIds.push_back(childConfigurationId);
            configuration_parent->setParameter("child_configuration_ids", childIds);
            HM_LOGGER_INFO("Added child configuration ID {} to parent configuration.", childConfigurationId);
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error adding child configuration ID to parent: {}", e.what());
            throw;
        }
    }

    void SceneNode::rmvChildConfigurationID(std::shared_ptr<Configuration> configuration_parent, std::shared_ptr<Configuration> configuration_child) {
        try {
            const uint64_t& childConfigurationId = configuration_child->getID();
            auto childIds = configuration_parent->getParameterOrDefault<std::vector<uint64_t>>("child_configuration_ids", {});
            auto it = std::remove(childIds.begin(), childIds.end(), childConfigurationId);
            if (it != childIds.end()) {
                childIds.erase(it);
                configuration_parent->setParameter("child_configuration_ids", childIds);
                HM_LOGGER_INFO("Removed child configuration ID {} from parent configuration.", childConfigurationId);
            }
            else {
                HM_LOGGER_WARN("Child configuration ID {} not found in parent configuration.", childConfigurationId);
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error removing child configuration ID from parent: {}", e.what());
            throw;
        }
    }

    void SceneNode::initialize(const Configuration& configuration) {
        try {
            // Set Transformable properties
            const float posX = configuration.getParameterOrDefault<float>("position_x", 0.0f);
            const float posY = configuration.getParameterOrDefault<float>("position_y", 0.0f);
            const float rotation = configuration.getParameterOrDefault<float>("rotation", 0.0f);
            const float scaleX = configuration.getParameterOrDefault<float>("scale_x", 1.0f);
            const float scaleY = configuration.getParameterOrDefault<float>("scale_y", 1.0f);
            const float originX = configuration.getParameterOrDefault<float>("origin_x", 0.0f);
            const float originY = configuration.getParameterOrDefault<float>("origin_y", 0.0f);

            setPosition(posX, posY);
            setRotation(rotation);
            setScale(scaleX, scaleY);
            setOrigin(originX, originY);

            HM_LOGGER_INFO("Transformable properties initialized: position=({}, {}), rotation={}, scale=({}, {}), origin=({}, {})",
                posX, posY, rotation, scaleX, scaleY, originX, originY);

            // Attach child SceneNodes
            const std::vector<uint64_t> childrenIds = configuration.getParameterOrDefault("child_configuration_ids", std::vector<uint64_t>());
            for (const uint64_t& childId : childrenIds) {
                try {
                    auto childConfig = Object::find<Configuration>(childId);
                    if (!childConfig) {
                        HM_LOGGER_WARN("Child configuration with ID {} not found. Skipping.", childId);
                        continue;
                    }

                    attachChild(childConfig->create<SceneNode>());
                    HM_LOGGER_INFO("Successfully attached child SceneNode with configuration ID: {}", childId);
                }
                catch (const std::exception& e) {
                    HM_LOGGER_ERROR("Error during SceneNode initialization for child ID: {}. Error: {}", childId, e.what());
                    throw;
                }
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_CRITICAL("Error initializing SceneNode with ID: {}. Exception: {}", getID(), e.what());
            throw;
        }
    }

} // namespace Harmony

