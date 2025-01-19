#pragma once
#include <Harmony/Scene.h>


namespace Asteroid
{
	class BackgroundStar : public Harmony::Core::SceneNode
	{
	public:
		BackgroundStar() {};

		void onEnterCurrent(Harmony::Core::Scene& scene) override;
		void onExitCurrent(Harmony::Core::Scene& scene) override;

		void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;

	public:
		sf::Clock clock;
		float offset;
		float duration;
	};
}

