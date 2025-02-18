#include "pch.h"
#include "Text.h"
#include "Configuration.h"
#include "Font.h"

namespace Harmony
{
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
        if (const auto stringData = configuration_->get({ "String" }))
        {
            sprite.setString(stringData.value().get<std::string>());
        }

        // Set the font
        if (const auto fontData = configuration_->get({ "Font" }))
        {
            const auto fontConfiguration = create<Configuration>(fontData.value());
            try
            {
                font = find<Font>(fontConfiguration->get<std::string>({ "Name" }).value_or("UnknownFont.ttf"));
            }
            catch (const std::exception&)
            {
                font = create<Font>(fontConfiguration);
            }
            sprite.setFont(font->getResource());
        }

        // Set the character size
        if (const auto characterSizeData = configuration_->get({ "CharacterSize" }))
        {
            sprite.setCharacterSize(characterSizeData.value().get<unsigned int>());
        }

        // Set the fill color
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

        // Set the outline color
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

        // Set the outline thickness
        if (const auto outlineThicknessData = configuration_->get({ "OutlineThickness" }))
        {
            sprite.setOutlineThickness(outlineThicknessData.value().get<float>());
        }

        // Set the style (e.g., bold, italic, underlined, etc.)
        if (const auto styleData = configuration_->get({ "Style" }))
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

        if (configuration_->get({ "Origin" })) {
            const auto bounds = sprite.getGlobalBounds();
            setOrigin(bounds.width / 2, bounds.height / 2);
        }
    }
    void Text::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(sprite, states);
    }
}
