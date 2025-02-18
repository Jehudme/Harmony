#include "pch.h"
#include "Circle.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Configuration.h"
#include "Texture.h"

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
    if(isReseting_)
    {
        SceneNode::initialize();
    }

    // Set the radius of the circle if provided in the configuration
    if (const auto radiusData = configuration_->get({ "Radius" }))
    {
        float radius = radiusData.value().get<float>();
        sprite.setRadius(radius);
    }

    // Set the number of points (smoothness) of the circle if provided in the configuration
    if (const auto pointCountData = configuration_->get({ "PointCount" }))
    {
        unsigned int pointCount = pointCountData.value().get<unsigned int>();
        sprite.setPointCount(pointCount);
    }

    // Set the fill color of the circle if provided in the configuration
    if (const auto fillColorData = configuration_->get({ "FillColor" }))
    {
        const auto colorConfiguration = create<Configuration>(fillColorData.value());
        sf::Color fillColor = {
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "R" }).value_or(255)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "G" }).value_or(255)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "B" }).value_or(255)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "A" }).value_or(255))
        };
        sprite.setFillColor(fillColor);
    }

    // Set the outline color of the circle if provided in the configuration
    if (const auto outlineColorData = configuration_->get({ "OutlineColor" }))
    {
        const auto colorConfiguration = create<Configuration>(outlineColorData.value());
        sf::Color outlineColor = {
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "R" }).value_or(0)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "G" }).value_or(0)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "B" }).value_or(0)),
            static_cast<sf::Uint8>(colorConfiguration->get<int>({ "A" }).value_or(255))
        };
        sprite.setOutlineColor(outlineColor);
    }

    // Set the outline thickness of the circle if provided in the configuration
    if (const auto outlineThicknessData = configuration_->get({ "OutlineThickness" }))
    {
        float outlineThickness = outlineThicknessData.value().get<float>();
        sprite.setOutlineThickness(outlineThickness);
    }

    if (const auto textureData = configuration_->get({ "Texture" }))
    {
        const auto textureConfiguration = create<Configuration>(textureData.value());
        try
        {
            texture = find<Texture>(textureConfiguration->get<std::string>({ "Name" }).value_or("UnknowTexture"));
        }
        catch (const std::exception&)
        {
            texture = create<Texture>(textureConfiguration);
        }

        const sf::Vector2u textureSize = texture->getResource().getSize();
        const int width = textureConfiguration->get<int>({ "Width" }).value_or(static_cast<int>(textureSize.x));
        const int height = textureConfiguration->get<int>({ "Height" }).value_or(static_cast<int>(textureSize.y));
        const int left = textureConfiguration->get<int>({ "left" }).value_or(0);
        const int top = textureConfiguration->get<int>({ "top" }).value_or(0);

        sprite.setTexture(&texture->getResource());
        sprite.setTextureRect(sf::IntRect(left, top, width, height));
    }
}
