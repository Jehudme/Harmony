#include "pch.h"
#include "Sprite.h"
#include "Logger.h"

namespace Harmony
{
    Sprite::Sprite()
    {
        initialize(*Object::create<Configuration>(INVALID_UNIQUE_ID));
    }

    Sprite::Sprite(const uint64_t& configuration_id)
    {
        initialize(*Object::find<Configuration>(configuration_id));
    }

    Sprite::Sprite(const std::shared_ptr<Configuration> configuration)
        : Entity(configuration)
    {
        initialize(*configuration);
    }

    void Sprite::initialize(const Configuration& configuration)
    {
        try {
            // Load parameters from configuration
            sf::Vector2f position = configuration.getParameterOrDefault<sf::Vector2f>("position", { 0.0f, 0.0f });
            sf::Vector2f scale = configuration.getParameterOrDefault<sf::Vector2f>("scale", { 1.0f, 1.0f });
            float rotation = configuration.getParameterOrDefault<float>("rotation", 0.0f);
            sf::Color color = configuration.getParameterOrDefault<sf::Color>("color", sf::Color::White);

            // Configure the Sprite
            drawableInstance_.setPosition(position);
            drawableInstance_.setScale(scale);
            drawableInstance_.setRotation(rotation);
            drawableInstance_.setColor(color);

            // Load texture
            uint64_t textureID = configuration.getParameterOrDefault<uint64_t>("texture_id", INVALID_UNIQUE_ID);
            if (textureID != INVALID_UNIQUE_ID) {
                texture_ = Object::find<Resource<sf::Texture>>(textureID);
                if (texture_) {
                    drawableInstance_.setTexture(*texture_->getResource());
                }
                else {
                    HM_LOGGER_WARN("Texture ID {} not found.", textureID);
                }
            }

            // Load shader
            uint64_t shaderID = configuration.getParameterOrDefault<uint64_t>("shader_id", INVALID_UNIQUE_ID);
            if (shaderID != INVALID_UNIQUE_ID) {
                shader_ = Object::find<Resource<sf::Shader>>(shaderID);
                if (!shader_) {
                    HM_LOGGER_WARN("Shader ID {} not found.", shaderID);
                }
            }

            HM_LOGGER_INFO("Sprite initialized (ID: {})", getID());
        }
        catch (const std::exception& e) {
            HM_LOGGER_CRITICAL("Failed to initialize Sprite (ID: {}): {}", getID(), e.what());
            throw;
        }
    }

    void Sprite::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
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
            HM_LOGGER_ERROR("Exception during Sprite drawing (ID: {}): {}", getID(), e.what());
            throw;
        }
    }

    void Sprite::updateCurrent(const sf::Time& time)
    {
        try {
            // Placeholder for sprite-specific updates
            HM_LOGGER_DEBUG("Updating Sprite (ID: {})", getID());
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during Sprite update (ID: {}): {}", getID(), e.what());
            throw;
        }
    }
}

