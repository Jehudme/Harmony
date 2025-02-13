#pragma once

#include "SceneNode.h"
#include <SFML/Graphics/RectangleShape.hpp>

namespace Harmony {

    class Rectangle : public SceneNode {
    public:
        explicit Rectangle(std::shared_ptr<Configuration> configuration);

    private:
        void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

        sf::RectangleShape sprite_;
    };
}