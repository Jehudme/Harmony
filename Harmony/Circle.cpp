#include "pch.h"
#include "Circle.h"
#include "Logger.h"

namespace Harmony
{
    Circle::Circle()
    {
        initialize(*Object::create<Configuration>(INVALID_UNIQUE_ID));
    }

    Circle::Circle(const uint64_t& configuration_id)
    {
        initialize(*Object::find<Configuration>(configuration_id));
    }

    Circle::Circle(const std::shared_ptr<Configuration> configuration)
        : Entity(configuration)
    {
        initialize(*configuration);
    }

    void Circle::initialize(const Configuration& configuration)
    {
        try {
            // Load parameters from configuration
            float radius = configuration.getParameterOrDefault<float>("circle_radius", 50.0f);
            sf::Vector2f position = configuration.getParameterOrDefault<sf::Vector2f>("circle_position", { 100.0f, 100.0f });
            sf::Color fillColor = configuration.getParameterOrDefault<sf::Color>("circle_fill_color", sf::Color::White);

            // Configure the CircleShape
            drawableInstance_.setRadius(radius);
            drawableInstance_.setPosition(position);
            drawableInstance_.setFillColor(fillColor);

            // Load texture if specified
            uint64_t textureID = configuration.getParameterOrDefault<uint64_t>("circle_texture_id", INVALID_UNIQUE_ID);
            if (textureID != INVALID_UNIQUE_ID) {
                texture_ = Object::find<Resource<sf::Texture>>(textureID);
                if (texture_) {
                    drawableInstance_.setTexture(texture_->getResource());
                }
            }

            // Load shader if specified
            uint64_t shaderID = configuration.getParameterOrDefault<uint64_t>("circle_shader_id", INVALID_UNIQUE_ID);
            if (shaderID != INVALID_UNIQUE_ID) {
                shader_ = Object::find<Resource<sf::Shader>>(shaderID);
            }

            HM_LOGGER_INFO("Circle initialized (ID: {})", getID());
        }
        catch (const std::exception& e) {
            HM_LOGGER_CRITICAL("Failed to initialize Circle (ID: {}): {}", getID(), e.what());
            throw;
        }
    }

    void Circle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        try {
            if (shader_) {
                target.draw(drawableInstance_, shader_->getResource());
            }
            else {
                target.draw(drawableInstance_, states);
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during Circle drawing (ID: {}): {}", getID(), e.what());
            throw;
        }
    }

    void Circle::updateCurrent(const sf::Time& time)
    {
        try {
            // Placeholder for circle-specific updates
            HM_LOGGER_DEBUG("Updating Circle (ID: {})", getID());
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during Circle update (ID: {}): {}", getID(), e.what());
            throw;
        }
    }
}


