#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
	extern const uint64_t PlayerUniqueId;

	class Player : public harmony::core::SceneNode
	{
	public:
		Player();

		void onCreate(harmony::core::Scene& scene) override;
		void onDestroy(harmony::core::Scene& scene) override;

		void onUpdate(const sf::Time& time, harmony::core::EventPool& eventPool) override;

		sf::Clock bulletClock;
		sf::Clock trailClock;
	};
}

