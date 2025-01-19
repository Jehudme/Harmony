#pragma once
#include "Harmony/Scene.h"

namespace Asteroid
{
	class BackGroundLayer : public Harmony::Core::SceneNode
	{
	public:
		BackGroundLayer() = default;

		void onEnterCurrent(Harmony::Core::Scene& scene) override;
		void onExitCurrent(Harmony::Core::Scene& scene) override;

		void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;

		sf::Clock clock;
	};
}

