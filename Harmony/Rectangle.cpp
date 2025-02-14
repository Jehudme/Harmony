#include "pch.h"
#include "Rectangle.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Configuration.h"

Harmony::Rectangle::Rectangle(std::shared_ptr<Configuration> configuration)
    : SceneNode(configuration)
{
    // Set the size of the rectangle if provided in the configuration
    if (const auto sizeData = configuration->get({ "Size" }))
    {
        const auto sizeConfiguration = create<Configuration>(sizeData.value());
        sf::Vector2f size = {
            sizeConfiguration->get<float>({ "Width" }).value_or(50),
            sizeConfiguration->get<float>({ "Height" }).value_or(50)
        };
        sprite.setSize(size);
    }

    // Set the fill color of the rectangle if provided in the configuration
    if (const auto fillColorData = configuration->get({ "FillColor" }))
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

    // Set the outline color of the rectangle if provided in the configuration
    if (const auto outlineColorData = configuration->get({ "OutlineColor" }))
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

    // Set the outline thickness of the rectangle if provided in the configuration
    if (const auto outlineThicknessData = configuration->get({ "OutlineThickness" }))
    {
        float outlineThickness = outlineThicknessData.value().get<float>();
        sprite.setOutlineThickness(outlineThickness);
    }
}


void Harmony::Rectangle::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
	renderTarget.draw(sprite, states);
}
