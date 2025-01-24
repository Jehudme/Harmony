#pragma once

#include <Harmony/Engine.h>
#include "Asteroid.h"
#include <vector>
#include <random>

namespace Asteroid
{
    extern const uint64_t AsteroidGroupUniqueId;

    class AsteroidGroup : public harmony::core::SceneNode
    {
    public:
        AsteroidGroup();

        void onCreate(harmony::core::Scene& scene) override;
        void onDestroy(harmony::core::Scene& scene) override;

        void onUpdate(const sf::Time& time, harmony::core::EventQueue& eventQueue) override;

    private:

    };
}

