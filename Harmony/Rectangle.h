#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "Shape.h"

namespace Harmony::Components
{
	class Rectangle : public Harmony::Components::Shape<sf::RectangleShape>
	{
	public:
		Rectangle(const Utilities::Configuration& configuration);
		~Rectangle();
	};
}

