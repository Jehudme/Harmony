#include "pch.h"
#include "Rectangle.h"
#include "Configuration.h"
#include "Constants.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace Harmony {

    Rectangle::Rectangle(std::shared_ptr<Configuration> configuration)
        : SceneNode(configuration) {

        // Set the size of the rectangle if provided in the configuration
        if (const auto sizeData = configuration->get({ Config::SIZE })) {
            const auto sizeConfiguration = create<Configuration>(sizeData.value());
            sf::Vector2f size = {
                sizeConfiguration->get<float>({ Config::WIDTH }).value_or(Default::RECTANGLE_WIDTH),
                sizeConfiguration->get<float>({ Config::HEIGHT }).value_or(Default::RECTANGLE_HEIGHT)
            };
            sprite_.setSize(size);
        }

        // Set the fill color of the rectangle if provided in the configuration
        if (const auto fillColorData = configuration->get({ Config::FILL_COLOR })) {
            const auto colorConfiguration = create<Configuration>(fillColorData.value());
            sf::Color fillColor = {
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::R }).value_or(Default::COLOR_R)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::G }).value_or(Default::COLOR_G)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::B }).value_or(Default::COLOR_B)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::A }).value_or(Default::COLOR_A))
            };
            sprite_.setFillColor(fillColor);
        }

        // Set the outline color of the rectangle if provided in the configuration
        if (const auto outlineColorData = configuration->get({ Config::OUTLINE_COLOR })) {
            const auto colorConfiguration = create<Configuration>(outlineColorData.value());
            sf::Color outlineColor = {
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::R }).value_or(Default::OUTLINE_COLOR_R)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::G }).value_or(Default::OUTLINE_COLOR_G)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::B }).value_or(Default::OUTLINE_COLOR_B)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ Config::A }).value_or(Default::OUTLINE_COLOR_A))
            };
            sprite_.setOutlineColor(outlineColor);
        }

        // Set the outline thickness of the rectangle if provided in the configuration
        if (const auto outlineThicknessData = configuration->get({ Config::OUTLINE_THICKNESS })) {
            float outlineThickness = outlineThicknessData.value().get<float>();
            sprite_.setOutlineThickness(outlineThickness);
        }
        else {
            sprite_.setOutlineThickness(Default::OUTLINE_THICKNESS);
        }
    }

    void Rectangle::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        renderTarget.draw(sprite_, states);
    }
}