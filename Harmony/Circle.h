#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include "Shape.h"

// NOTE: This class currently inherits from SFML types
// This is a known limitation that should be addressed in a future refactoring
// by using composition and interfaces instead of inheritance

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Components
{
	/// @brief Circle shape component
	/// @warning Currently exposes SFML CircleShape through inheritance
	class Circle : public Harmony::Components::Shape<sf::CircleShape>
	{
	public:
		Circle(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Circle();
	};
}

