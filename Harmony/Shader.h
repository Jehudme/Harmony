#pragma once

#include "Resource.h"
#include "SFML/Graphics/Shader.hpp"
#include "Configuration.h"

namespace Harmony::Resources
{
	class Shader : public Resource, public sf::Shader
	{
	public:
		Shader(const Utilities::Configuration& configuration);
		~Shader();
	};
}
