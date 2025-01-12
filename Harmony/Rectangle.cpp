#include "pch.h"
#include "Rectangle.h"
#include "Logger.h"
#include "Object.h"

namespace Harmony
{
    Rectangle::Rectangle()
        : Entity(), texture_(std::make_shared<Resource<sf::Texture>>()), shader_(std::make_shared<Resource<sf::Shader>>())
    {
        HM_LOGGER_INFO("Rectangle created with default configuration.");
        initialize(*Object::create<Configuration>());
    }

    Rectangle::Rectangle(const uint64_t& configuration_id)
        : Entity(configuration_id), texture_(std::make_shared<Resource<sf::Texture>>()), shader_(std::make_shared<Resource<sf::Shader>>())
    {
        HM_LOGGER_INFO("Rectangle created with configuration ID: {}", configuration_id);
        initialize(*Object::find<Configuration>(configuration_id));
    }

    Rectangle::Rectangle(const std::shared_ptr<Configuration> configuration)
        : Entity(configuration), texture_(std::make_shared<Resource<sf::Texture>>()), shader_(std::make_shared<Resource<sf::Shader>>())
    {
        HM_LOGGER_INFO("Rectangle created with shared configuration ID: {}", configuration->getID());
        initialize(*configuration);
    }

    void Rectangle::initialize(const Configuration& configuration)
    {
        try {
            // Set size of the rectangle
            const sf::Vector2f size = configuration.getParameterOrDefault<sf::Vector2f>("rectangle_size", { 100.f, 50.f });
            drawableInstance_.setSize(size);
            HM_LOGGER_INFO("Rectangle size set to: ({}, {})", size.x, size.y);

            // Set fill color
            const sf::Color fillColor = configuration.getParameterOrDefault<sf::Color>("fill_color", sf::Color::White);
            drawableInstance_.setFillColor(fillColor);

            // Set outline color and thickness
            const sf::Color outlineColor = configuration.getParameterOrDefault<sf::Color>("outline_color", sf::Color::Black);
            const float outlineThickness = configuration.getParameterOrDefault<float>("outline_thickness", 0.f);
            drawableInstance_.setOutlineColor(outlineColor);
            drawableInstance_.setOutlineThickness(outlineThickness);

            // Load texture if specified
            const uint64_t textureId = configuration.getParameterOrDefault<uint64_t>("texture_id", 0);
            if (textureId != 0) {
                texture_ = Object::find<Resource<sf::Texture>>(textureId);
                if (texture_) {
                    drawableInstance_.setTexture(texture_->getResource());
                    HM_LOGGER_INFO("Rectangle texture set from resource ID: {}", textureId);
                }
                else {
                    HM_LOGGER_WARN("Texture with ID: {} not found. Skipping texture setup.", textureId);
                }
            }

            // Load shader if specified
            const uint64_t shaderId = configuration.getParameterOrDefault<uint64_t>("shader_id", 0);
            if (shaderId != 0) {
                shader_ = Object::find<Resource<sf::Shader>>(shaderId);
                if (shader_) {
                    HM_LOGGER_INFO("Rectangle shader set from resource ID: {}", shaderId);
                }
                else {
                    HM_LOGGER_WARN("Shader with ID: {} not found. Skipping shader setup.", shaderId);
                }
            }

        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error initializing Rectangle: {}", e.what());
            throw;
        }
    }

    void Rectangle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (shader_ && shader_->getResource()) {
            states.shader = shader_->getResource();
        }
        target.draw(drawableInstance_, states);
    }

    void Rectangle::updateCurrent(const sf::Time& time)
    {
        // Default implementation: No-op.
    }
}
