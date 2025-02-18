#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Configuration.h"
#include "SceneNode.h"

namespace Harmony {
    class Font;
    class Text : public SceneNode {
    public:
        explicit Text(std::shared_ptr<Configuration> configuration);

        sf::FloatRect getGlobalBound() const override;

    private:
        void initialize() override;
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    public:
        sf::Text sprite;
        std::shared_ptr<Font> font;
    };
}