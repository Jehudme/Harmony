#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
	class PlayerTrail : public harmony::core::SceneNode
	{
	public:
		PlayerTrail(float angle, sf::Vector2f position);

		void onUpdate(const sf::Time& time, harmony::core::EventPool& eventPool) override;

	public:
		sf::Clock clock;
	};
}

