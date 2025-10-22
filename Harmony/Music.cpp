#include "pch.h"
#include "Music.h"
#include "Exceptions.h"
#include "Configuration.h"

namespace Harmony::Resources
{
	Music::Music(const Utilities::Configuration& configuration)
	{
		std::optional<std::string> filepath;
		if(filepath = configuration.get<std::string>({ "filepath" }); !filepath.has_value())
			throw Exceptions::ConfigurationExceptions("Music resource missing 'filepath' configuration");

		if (!this->openFromFile(filepath.value()))
			throw Exceptions::ResourceLoadException("Music", 0, "Failed to load music from file: " + filepath.value());
	}
	Music::~Music() = default;


	template<>
	Music& convert(Resource& resource)
	{
		Music* music = dynamic_cast<Music*>(&resource);
		if (!music)
			throw Exceptions::ResourceConversionException("Music", typeid(resource).name());
		return *music;
	}
}
