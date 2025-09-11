#include "pch.h"
#include "Rectangle.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Configuration.h"
#include "Texture.h"

namespace Harmony
{
    namespace
    {
        // Constants for default sizes, colors, thickness, and strings
        constexpr float DEFAULT_RECTANGLE_WIDTH = 50.0f;
        constexpr float DEFAULT_RECTANGLE_HEIGHT = 50.0f;

        constexpr sf::Uint8 DEFAULT_FILL_COLOR_R = 255;
        constexpr sf::Uint8 DEFAULT_FILL_COLOR_G = 255;
        constexpr sf::Uint8 DEFAULT_FILL_COLOR_B = 255;
        constexpr sf::Uint8 DEFAULT_FILL_COLOR_A = 255;

        constexpr sf::Uint8 DEFAULT_OUTLINE_COLOR_R = 0;
        constexpr sf::Uint8 DEFAULT_OUTLINE_COLOR_G = 0;
        constexpr sf::Uint8 DEFAULT_OUTLINE_COLOR_B = 0;
        constexpr sf::Uint8 DEFAULT_OUTLINE_COLOR_A = 255;

        constexpr float DEFAULT_OUTLINE_THICKNESS = 0.0f;

        constexpr const char* DEFAULT_TEXTURE_NAME = "UnknowTexture";
        constexpr int DEFAULT_TEXTURE_LEFT = 0;
        constexpr int DEFAULT_TEXTURE_TOP = 0;

        // Constants for configuration keys
        constexpr const char* CONFIG_KEY_SIZE = "Size";
        constexpr const char* CONFIG_KEY_WIDTH = "Width";
        constexpr const char* CONFIG_KEY_HEIGHT = "Height";
        constexpr const char* CONFIG_KEY_FILL_COLOR = "FillColor";
        constexpr const char* CONFIG_KEY_OUTLINE_COLOR = "OutlineColor";
        constexpr const char* CONFIG_KEY_OUTLINE_THICKNESS = "OutlineThickness";
        constexpr const char* CONFIG_KEY_TEXTURE = "Texture";
        constexpr const char* CONFIG_KEY_R = "R";
        constexpr const char* CONFIG_KEY_G = "G";
        constexpr const char* CONFIG_KEY_B = "B";
        constexpr const char* CONFIG_KEY_A = "A";
        constexpr const char* CONFIG_KEY_LEFT = "left";
        constexpr const char* CONFIG_KEY_TOP = "top";
    }

    Rectangle::Rectangle(std::shared_ptr<Configuration> configuration)
        : SceneNode(configuration)
    {
        initialize();
    }

    sf::FloatRect Rectangle::getGlobalBound() const {
        return getGlobalTransform().transformRect(sprite.getLocalBounds());
    }

    void Rectangle::initialize()
    {
        if (isReseting_)
        {
            SceneNode::initialize();
        }

        if (const auto sizeData = configuration_->get({ CONFIG_KEY_SIZE }))
        {
            const auto sizeConfiguration = create<Configuration>(sizeData.value());
            sf::Vector2f size = {
                sizeConfiguration->get<float>({ CONFIG_KEY_WIDTH }).value_or(DEFAULT_RECTANGLE_WIDTH),
                sizeConfiguration->get<float>({ CONFIG_KEY_HEIGHT }).value_or(DEFAULT_RECTANGLE_HEIGHT)
            };
            sprite.setSize(size);
        }

        if (const auto fillColorData = configuration_->get({ CONFIG_KEY_FILL_COLOR }))
        {
            const auto colorConfiguration = create<Configuration>(fillColorData.value());
            sf::Color fillColor = {
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_R }).value_or(DEFAULT_FILL_COLOR_R)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_G }).value_or(DEFAULT_FILL_COLOR_G)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_B }).value_or(DEFAULT_FILL_COLOR_B)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_A }).value_or(DEFAULT_FILL_COLOR_A))
            };
            sprite.setFillColor(fillColor);
        }

        if (const auto outlineColorData = configuration_->get({ CONFIG_KEY_OUTLINE_COLOR }))
        {
            const auto colorConfiguration = create<Configuration>(outlineColorData.value());
            sf::Color outlineColor = {
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_R }).value_or(DEFAULT_OUTLINE_COLOR_R)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_G }).value_or(DEFAULT_OUTLINE_COLOR_G)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_B }).value_or(DEFAULT_OUTLINE_COLOR_B)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_KEY_A }).value_or(DEFAULT_OUTLINE_COLOR_A))
            };
            sprite.setOutlineColor(outlineColor);
        }

        if (const auto outlineThicknessData = configuration_->get({ CONFIG_KEY_OUTLINE_THICKNESS }))
        {
            float outlineThickness = outlineThicknessData.value().get<float>();
            sprite.setOutlineThickness(outlineThickness);
        }

        if (const auto textureData = configuration_->get({ CONFIG_KEY_TEXTURE }))
        {
            const auto textureConfiguration = create<Configuration>(textureData.value());
            texture = Resource::find<Texture>(textureConfiguration, DEFAULT_TEXTURE_NAME);

            const sf::Vector2u textureSize = texture->getResource().getSize();
            const int width = textureConfiguration->get<int>({ CONFIG_KEY_WIDTH }).value_or(static_cast<int>(textureSize.x));
            const int height = textureConfiguration->get<int>({ CONFIG_KEY_HEIGHT }).value_or(static_cast<int>(textureSize.y));
            const int left = textureConfiguration->get<int>({ CONFIG_KEY_LEFT }).value_or(DEFAULT_TEXTURE_LEFT);
            const int top = textureConfiguration->get<int>({ CONFIG_KEY_TOP }).value_or(DEFAULT_TEXTURE_TOP);

            sprite.setTexture(&texture->getResource());
            sprite.setTextureRect(sf::IntRect(left, top, width, height));
        }
    }

    void Rectangle::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
        renderTarget.draw(sprite, states);
    }
}