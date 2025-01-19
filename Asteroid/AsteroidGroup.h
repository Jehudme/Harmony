#pragma once
#pragma once

#include "Harmony/SceneNode.h"
#include "Asteroid.h"
#include <vector>
#include <random>

namespace Asteroid
{
    extern const uint64_t AsteroidGroupUniqueId;

    class AsteroidGroup : public Harmony::Core::SceneNode
    {
    public:
        AsteroidGroup();

        void onEnterCurrent(Harmony::Core::Scene& scene) override;
        void onExitCurrent(Harmony::Core::Scene& scene) override;

        void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;

    private:

    };
}

