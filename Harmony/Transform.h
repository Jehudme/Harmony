#pragma once

#include <SFML/Graphics/Transform.hpp>

namespace Harmony::Components
{
	/// @brief Transform component - position, rotation, and scale
	/// Uses composition to hide SFML implementation details
	class Transform : public sf::Transformable
	{
	public:
		Transform(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Transform();
	};
}

