#pragma once
#include "Harmony/Scene.h"

namespace Asteroid
{
	class AsteroidGroup : public Harmony::Core::SceneNode
	{
	public:
		AsteroidGroup() = default;

		void onEnterCurrent(Harmony::Core::Scene& scene) override;
		void onExitCurrent(Harmony::Core::Scene& scene) override;

		void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool);

	public:

	};
}
