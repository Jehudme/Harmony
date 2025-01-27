#pragma once
#include <Harmony/Engine.h>

namespace Asteroid
{
	class Bullet : public harmony::core::SceneNode
	{
	public:
		Bullet(const float angle, sf::Vector2f position);

		void onUpdate(const sf::Time& time, harmony::core::EventQueue& eventQueue) override;

	public:
		sf::Clock clock;
	};
}


