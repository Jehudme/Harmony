#include "pch.h"
#include "Text.h"
#include "Configuration.h"
#include "Font.h"

namespace Harmony
{
    namespace
    {
        // Configuration keys
        constexpr const char* CONFIG_STRING = "String";
        constexpr const char* CONFIG_FONT = "Font";
        constexpr const char* CONFIG_CHARACTER_SIZE = "CharacterSize";
        constexpr const char* CONFIG_FILL_COLOR = "FillColor";
        constexpr const char* CONFIG_OUTLINE_COLOR = "OutlineColor";
        constexpr const char* CONFIG_OUTLINE_THICKNESS = "OutlineThickness";
        constexpr const char* CONFIG_STYLE = "Style";
        constexpr const char* CONFIG_ORIGIN = "Origin";

        // Color keys
        constexpr const char* CONFIG_COLOR_R = "R";
        constexpr const char* CONFIG_COLOR_G = "G";
        constexpr const char* CONFIG_COLOR_B = "B";
        constexpr const char* CONFIG_COLOR_A = "A";

        // Default values
        constexpr const char* DEFAULT_FONT_NAME = "UnknownFont.ttf";
        constexpr sf::Uint8 DEFAULT_COLOR_R = 255;
        constexpr sf::Uint8 DEFAULT_COLOR_G = 255;
        constexpr sf::Uint8 DEFAULT_COLOR_B = 255;
        constexpr sf::Uint8 DEFAULT_COLOR_A = 255;
    }

    Text::Text(std::shared_ptr<Configuration> configuration)
        : SceneNode(configuration)
    {
        initialize();
    }

    sf::FloatRect Text::getGlobalBound() const
    {
        return getGlobalTransform().transformRect(sprite.getLocalBounds());
    }

    void Text::initialize()
    {
        if (isReseting_)
        {
            SceneNode::initialize();
        }

        // Set the string (text content)
        if (const auto stringData = configuration_->get({ CONFIG_STRING }))
        {
            sprite.setString(stringData.value().get<std::string>());
        }

        // Set the font
        if (const auto fontData = configuration_->get({ CONFIG_FONT }))
        {
            const auto fontConfiguration = create<Configuration>(fontData.value());
            font = Resource::find<Font>(fontConfiguration, DEFAULT_FONT_NAME);

            sprite.setFont(font->getResource());
        }

        // Set the character size
        if (const auto characterSizeData = configuration_->get({ CONFIG_CHARACTER_SIZE }))
        {
            sprite.setCharacterSize(characterSizeData.value().get<unsigned int>());
        }

        // Set the fill color
        if (const auto fillColorData = configuration_->get({ CONFIG_FILL_COLOR }))
        {
            const auto colorConfiguration = create<Configuration>(fillColorData.value());
            sf::Color fillColor = {
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_R }).value_or(DEFAULT_COLOR_R)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_G }).value_or(DEFAULT_COLOR_G)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_B }).value_or(DEFAULT_COLOR_B)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_A }).value_or(DEFAULT_COLOR_A))
            };
            sprite.setFillColor(fillColor);
        }

        // Set the outline color
        if (const auto outlineColorData = configuration_->get({ CONFIG_OUTLINE_COLOR }))
        {
            const auto colorConfiguration = create<Configuration>(outlineColorData.value());
            sf::Color outlineColor = {
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_R }).value_or(0)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_G }).value_or(0)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_B }).value_or(0)),
                static_cast<sf::Uint8>(colorConfiguration->get<int>({ CONFIG_COLOR_A }).value_or(DEFAULT_COLOR_A))
            };
            sprite.setOutlineColor(outlineColor);
        }

        // Set the outline thickness
        if (const auto outlineThicknessData = configuration_->get({ CONFIG_OUTLINE_THICKNESS }))
        {
            sprite.setOutlineThickness(outlineThicknessData.value().get<float>());
        }

        // Set the style (e.g., bold, italic, underlined, etc.)
        if (const auto styleData = configuration_->get({ CONFIG_STYLE }))
        {
            sf::Uint32 style = sf::Text::Regular; // Default style

            // Parse the style array
            for (const auto& styleName : styleData.value().get<std::vector<std::string>>())
            {
                if (styleName == "Bold") {
                    style |= sf::Text::Bold;
                }
                else if (styleName == "Italic") {
                    style |= sf::Text::Italic;
                }
                else if (styleName == "Underlined") {
                    style |= sf::Text::Underlined;
                }
                else if (styleName == "StrikeThrough") {
                    style |= sf::Text::StrikeThrough;
                }
            }

            sprite.setStyle(style);
        }

        if (configuration_->get({ CONFIG_ORIGIN }))
        {
            const auto bounds = sprite.getGlobalBounds();
            setOrigin(bounds.width / 2, bounds.height / 2);
        }
    }

    void Text::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(sprite, states);
    }
}