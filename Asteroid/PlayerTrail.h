#pragma once
#include "Harmony/Scene.h"

namespace Asteroid
{
	class PlayerTrail : public Harmony::Core::SceneNode
	{
	public:
		PlayerTrail(float angle, sf::Vector2f position);

		void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;

	public:
		sf::Clock clock;
	};
}

