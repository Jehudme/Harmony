#pragma once
#include "Harmony/SceneNode.h"

namespace Asteroid
{
	class Bullet : public Harmony::Core::SceneNode
	{
	public:
		Bullet(const float angle, sf::Vector2f position);

		void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;

	public:
		sf::Clock clock;
	};
}


