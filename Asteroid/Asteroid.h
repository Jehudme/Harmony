#pragma once
#include "Harmony/Scene.h"

namespace Asteroid
{
	class Asteroid : public Harmony::Core::SceneNode
	{
	public:
		Asteroid();

		void updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool) override;

	};
}

