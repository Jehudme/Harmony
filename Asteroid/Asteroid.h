#pragma once
#include "Harmony/SceneNode.h"

namespace Asteroid
{
    class Asteroid_t : public Harmony::Core::SceneNode
    {
    public:
        Asteroid_t(const sf::View& sceneView);

        void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;
    };
}

