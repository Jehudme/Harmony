#pragma once
#include "SceneNode.h"
#include <SFML/Graphics/CircleShape.hpp>

namespace Harmony
{

    class Circle : public SceneNode
    {
    public:
        Circle(std::shared_ptr<Configuration> configuration);

    private:
        virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

    public:
        sf::CircleShape sprite;
    };
}