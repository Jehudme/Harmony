#include "pch.h"
#include "Entity.h"
#include "Logger.h"

namespace Harmony
{
    Entity::Entity()
        : SceneNode(), isDrawEnabled_(true), isUpdateEnabled_(true)
    {
        HM_LOGGER_INFO("Entity created with default configuration.");
        initialize(*Object::create<Configuration>());
    }

    Entity::Entity(const uint64_t& configuration_id)
        : SceneNode(configuration_id), isDrawEnabled_(true), isUpdateEnabled_(true)
    {
        HM_LOGGER_INFO("Entity created with configuration ID: {}", configuration_id);
        initialize(*Object::find<Configuration>(configuration_id));
    }

    Entity::Entity(const std::shared_ptr<Configuration> configuration)
        : SceneNode(configuration), isDrawEnabled_(true), isUpdateEnabled_(true)
    {
        HM_LOGGER_INFO("Entity created with shared configuration ID: {}", configuration->getID());
        initialize(*configuration);
    }

    void Entity::enableDrawing(const bool option)
    {
        isDrawEnabled_ = option;
        HM_LOGGER_INFO("Drawing enabled: {}", option);
    }

    void Entity::enableUpdate(const bool option)
    {
        isUpdateEnabled_ = option;
        HM_LOGGER_INFO("Update enabled: {}", option);
    }

    void Entity::initialize(const Configuration& configuration)
    {
        HM_LOGGER_INFO("Initializing Entity with configuration...");
        try {
            isDrawEnabled_ = configuration.getParameterOrDefault<bool>("enable_drawing", true);
            isUpdateEnabled_ = configuration.getParameterOrDefault<bool>("enable_update", true);
            HM_LOGGER_INFO("Entity initialized successfully. Drawing: {}, Update: {}", isDrawEnabled_, isUpdateEnabled_);
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error initializing Entity: {}", e.what());
            throw;
        }
    }

    void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (isDrawEnabled_) {
            drawCurrent(target, states);
        }
        else {
            HM_LOGGER_TRACE("Drawing skipped because it is disabled for this entity.");
        }

        // Draw child nodes
        for (const auto& child : children_) {
            try
            {
                std::dynamic_pointer_cast<Entity>(child)->draw(target, states);
                HM_LOGGER_TRACE("Drawing child SceneNode ID: {}", child->getID());
            }
            catch (const std::exception& e)
            {
                HM_LOGGER_ERROR("Error while drawing child ID: {}: {}", child->getID(), e.what());
            }
        }
    }

    void Entity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Custom drawing logic for the entity (override in derived classes)
        HM_LOGGER_TRACE("Drawing current Entity (default: no-op).");
    }

    void Entity::update(const sf::Time& time)
    {
        if (isUpdateEnabled_) {
            updateCurrent(time);
        }
        else {
            HM_LOGGER_TRACE("Update skipped because it is disabled for this entity.");
        }

        // Update child nodes
        for (const auto& child : children_) {
            try
            {
                std::dynamic_pointer_cast<Entity>(child)->update(time);
                HM_LOGGER_TRACE("Updating child SceneNode ID: {}", child->getID());
            }
            catch (const std::exception& e)
            {
                HM_LOGGER_ERROR("Error while updating child ID: {}: {}", child->getID(), e.what());
            }
        }
    }

    void Entity::updateCurrent(const sf::Time& time)
    {
        // Custom update logic for the entity (override in derived classes)
        HM_LOGGER_TRACE("Updating current Entity (default: no-op).");
    }
}
