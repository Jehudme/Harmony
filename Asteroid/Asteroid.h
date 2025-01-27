#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
    class Asteroid_t : public harmony::core::SceneNode
    {
    public:
        Asteroid_t(const sf::View& sceneView);

        void onUpdate(const sf::Time& time, harmony::core::EventQueue& eventQueue) override;
    };
}

