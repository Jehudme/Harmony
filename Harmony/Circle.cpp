#include "pch.h"
#include "Circle.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Configuration.h"
#include "Texture.h"

namespace {
    // Default color values
    constexpr sf::Uint8 DEFAULT_COLOR_MAX = 255;
    constexpr sf::Uint8 DEFAULT_COLOR_MIN = 0;
    constexpr sf::Uint8 DEFAULT_ALPHA = 255;

    // Default texture values
    constexpr int DEFAULT_LEFT = 0;
    constexpr int DEFAULT_TOP = 0;

    // Configuration key strings
    const std::string KEY_RADIUS = "Radius";
    const std::string KEY_POINT_COUNT = "PointCount";
    const std::string KEY_FILL_COLOR = "FillColor";
    const std::string KEY_OUTLINE_COLOR = "OutlineColor";
    const std::string KEY_OUTLINE_THICKNESS = "OutlineThickness";
    const std::string KEY_TEXTURE = "Texture";

    // Color component keys
    const std::string KEY_RED = "R";
    const std::string KEY_GREEN = "G";
    const std::string KEY_BLUE = "B";
    const std::string KEY_ALPHA = "A";

    // Texture configuration keys
    const std::string KEY_NAME = "Name";
    const std::string KEY_WIDTH = "Width";
    const std::string KEY_HEIGHT = "Height";
    const std::string KEY_LEFT = "left";
    const std::string KEY_TOP = "top";

    // Default texture name
    const std::string DEFAULT_TEXTURE_NAME = "UnknowTexture";
}

Harmony::Circle::Circle(std::shared_ptr<Configuration> configuration)
    : SceneNode(configuration)
{
    initialize();
}

sf::FloatRect Harmony::Circle::getGlobalBound() const {
    return getGlobalTransform().transformRect(sprite.getLocalBounds());
}

void Harmony::Circle::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(sprite, states);
}

void Harmony::Circle::initialize()
{
    if (isReseting_)
    {
        SceneNode::initialize();
    }

    // Set the radius of the circle if provided in the configuration
    if (const auto radiusData = configuration_->get({ KEY_RADIUS }))
    {
        float radius = radiusData.value().get<float>();
        sprite.setRadius(radius);
    }

    // Set the number of points (smoothness) of the circle if provided in the configuration
    if (const auto pointCountData = configuration_->get({ KEY_POINT_COUNT }))
    {
        unsigned int pointCount = pointCountData.value().get<unsigned int>();
        sprite.setPointCount(pointCount);
    }

    // Set the fill color of the circle if provided in the configuration
    if (const auto fillColorData = configuration_->get({ KEY_FILL_COLOR }))
    {
        const auto colorConfiguration = create<Configuration>(fillColorData.value());
        sf::Color fillColor = {
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_RED }).value_or(DEFAULT_COLOR_MAX)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_GREEN }).value_or(DEFAULT_COLOR_MAX)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_BLUE }).value_or(DEFAULT_COLOR_MAX)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_ALPHA }).value_or(DEFAULT_ALPHA))
        };
        sprite.setFillColor(fillColor);
    }

    // Set the outline color of the circle if provided in the configuration
    if (const auto outlineColorData = configuration_->get({ KEY_OUTLINE_COLOR }))
    {
        const auto colorConfiguration = create<Configuration>(outlineColorData.value());
        sf::Color outlineColor = {
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_RED }).value_or(DEFAULT_COLOR_MIN)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_GREEN }).value_or(DEFAULT_COLOR_MIN)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_BLUE }).value_or(DEFAULT_COLOR_MIN)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ KEY_ALPHA }).value_or(DEFAULT_ALPHA))
        };
        sprite.setOutlineColor(outlineColor);
    }

    // Set the outline thickness of the circle if provided in the configuration
    if (const auto outlineThicknessData = configuration_->get({ KEY_OUTLINE_THICKNESS }))
    {
        float outlineThickness = outlineThicknessData.value().get<float>();
        sprite.setOutlineThickness(outlineThickness);
    }

    if (const auto textureData = configuration_->get({ KEY_TEXTURE }))
    {
        const auto textureConfiguration = create<Configuration>(textureData.value());
        texture = Resource::find<Texture>(textureConfiguration, DEFAULT_TEXTURE_NAME);


        const sf::Vector2u textureSize = texture->getResource().getSize();
        const int width = textureConfiguration->get<int>({ KEY_WIDTH }).value_or(static_cast<int>(textureSize.x));
        const int height = textureConfiguration->get<int>({ KEY_HEIGHT }).value_or(static_cast<int>(textureSize.y));
        const int left = textureConfiguration->get<int>({ KEY_LEFT }).value_or(DEFAULT_LEFT);
        const int top = textureConfiguration->get<int>({ KEY_TOP }).value_or(DEFAULT_TOP);

        sprite.setTexture(&texture->getResource());
        sprite.setTextureRect(sf::IntRect(left, top, width, height));
    }
}