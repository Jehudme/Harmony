#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Configuration.h"
#include "Resource.h"
#include "SceneNode.h"

namespace Harmony {
    class Text : public SceneNode {
    public:
        explicit Text(std::shared_ptr<Configuration> configuration);

        sf::FloatRect getGlobalBound() const override;
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        sf::Text sprite;
        std::shared_ptr<Font> font;
    };
}