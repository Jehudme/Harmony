#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
	class BackGroundLayer : public harmony::core::SceneNode
	{
	public:
		BackGroundLayer() = default;

		void onCreate(harmony::core::Scene& scene) override;
		void onDestroy(harmony::core::Scene& scene) override;

		void onUpdate(const sf::Time& time, harmony::core::EventQueue& eventQueue) override;

		sf::Clock clock;
	};
}

