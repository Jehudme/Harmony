#pragma once
#include "Harmony/SceneNode.h"

namespace Asteroid
{
	class Player : public Harmony::Core::SceneNode
	{
	public:
		Player() = default;

		void onEnterCurrent(Harmony::Core::Scene& scene) override;
		void onExitCurrent(Harmony::Core::Scene& scene) override;

		void updateCurrent(const sf::Time& time) override;

	public:
		float velocity;
		float acceleration;

		sf::Clock bulletClock;
	};
}

