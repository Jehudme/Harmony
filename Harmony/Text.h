#pragma once

#include <SFML/Graphics/Text.hpp>

// NOTE: This class currently inherits from SFML types
// This is a known limitation that should be addressed in a future refactoring
// by using composition and interfaces instead of inheritance

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Utilities {
	class Configuration;
}

namespace Harmony::Components
{
	/// @brief Text component
	/// @warning Currently exposes SFML Text through inheritance
	class Text : public sf::Text
	{
	public:
		Text(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Text();
	};
}

