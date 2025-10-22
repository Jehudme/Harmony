#include "pch.h"
#include "Sound.h"
#include "Exceptions.h"
#include "Configuration.h"

namespace Harmony::Resources
{
	Sound::Sound(const Utilities::Configuration& configuration)
	{
		std::optional<std::string> filepath;
		if(filepath = configuration.get<std::string>({ "filepath" }); !filepath.has_value())
			throw Exceptions::ConfigurationExceptions("Sound resource missing 'filepath' configuration");

		if (!this->loadFromFile(filepath.value()))
			throw Exceptions::ResourceLoadException("Sound", 0, "Failed to load sound from file: " + filepath.value());
	}
	Sound::~Sound() = default;


	template<>
	Sound& convert(Resource& resource)
	{
		Sound* sound = dynamic_cast<Sound*>(&resource);
		if (!sound)
			throw Exceptions::ResourceConversionException("Sound", typeid(resource).name());
		return *sound;
	}
}
