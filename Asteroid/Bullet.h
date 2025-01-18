#pragma once
#include "Harmony/SceneNode.h"

namespace Asteroid
{
	class Bullet : public Harmony::Core::SceneNode
	{
	public:
		Bullet(const float angle);

		void updateCurrent(const sf::Time& time) override;

	public:
		sf::Clock clock;
	};
}


