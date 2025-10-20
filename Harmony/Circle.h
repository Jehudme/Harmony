#pragma once
#include "Shape.h"

namespace Harmony::Components
{
	class Circle : public Harmony::Components::Shape<sf::CircleShape>
	{
	public:
		Circle(const Utilities::Configuration& configuration);
		~Circle();
	};
}

