#pragma once

#include "Resource.h"
#include "SFML/Graphics/Texture.hpp"
#include "Configuration.h"

namespace Harmony::Resources
{
	class Texture : public Resource, public sf::Texture
	{
	public:
		Texture(const Utilities::Configuration& configuration);
		~Texture();
	};
}

