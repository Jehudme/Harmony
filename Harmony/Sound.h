#pragma once

#include "Resource.h"
#include "SFML/Audio/SoundBuffer.hpp"
#include "Configuration.h"

namespace Harmony::Resources
{
	class Sound : public Resource, public sf::SoundBuffer
	{
	public:
		Sound(const Utilities::Configuration& configuration);
		~Sound();
	};
}
