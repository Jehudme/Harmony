#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "Shape.h"

// NOTE: This class currently inherits from SFML types
// This is a known limitation that should be addressed in a future refactoring
// by using composition and interfaces instead of inheritance

namespace Harmony::Components
{
	/// @brief Rectangle shape component
	/// @warning Currently exposes SFML RectangleShape through inheritance
	class Rectangle : public Harmony::Components::Shape<sf::RectangleShape>
	{
	public:
		Rectangle(const Utilities::Configuration& configuration);
		~Rectangle();
	};
}

