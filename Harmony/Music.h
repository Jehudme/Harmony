#pragma once

#include "Resource.h"
#include "SFML/Audio/Music.hpp"
#include "Configuration.h"

namespace Harmony::Resources
{
	class Music : public Resource, public sf::Music
	{
	public:
		Music(const Utilities::Configuration& configuration);
		~Music();
	};
}
