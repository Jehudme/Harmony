#pragma once

#include "Resource.h"
#include "SFML/Graphics/Font.hpp"
#include "Configuration.h"

namespace Harmony::Resources
{
	class Font : public Resource, public sf::Font
	{
	public:
		Font(const Utilities::Configuration& configuration);
		~Font();
	};
}
