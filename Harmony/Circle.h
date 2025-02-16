#pragma once
#include "SceneNode.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "Resource.h"


namespace Harmony
{
    class Circle : public SceneNode
    {
    public:
        Circle(std::shared_ptr<Configuration> configuration);

        sf::FloatRect getGlobalBound() const override;

    private:
        virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

    public:
        sf::CircleShape sprite;
        std::shared_ptr<Texture> texture;
    };
}