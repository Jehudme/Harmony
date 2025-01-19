#pragma once
#include <Harmony/Engine.h>


namespace Asteroid
{
	class BackgroundStar : public harmony::core::SceneNode
	{
	public:
		BackgroundStar() {};

		void onCreate(harmony::core::Scene& scene) override;
		void onDestroy(harmony::core::Scene& scene) override;

		void onUpdate(const sf::Time& time, harmony::core::EventPool& eventPool) override;

	public:
		sf::Clock clock;
		float offset;
		float duration;
	};
}

