#include "pch.h"
#include "Music.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(music, Harmony::Resources::Music)

namespace Harmony::Resources
{

	Music::Music(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		music_{},
		musicLoaded_(false)
	{
		HARMONY_DEBUG("Music resource created with ID: {}", id);
	}

	Music::~Music()
	{
		if (musicLoaded_)
		{
			UnloadMusicStream(music_);
			musicLoaded_ = false;
		}
	}

	const char* Music::getType() const
	{
		return "music";
	}

	void Music::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Music resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading music from file: {}", filepath);

		music_ = LoadMusicStream(filepath.c_str());

		if (music_.frameCount == 0)
		{
			HARMONY_ERROR("Failed to load music from file: {}", filepath);
			throw Exceptions::MusicLoadException(filepath, "Raylib LoadMusicStream returned invalid music data");
		}

		musicLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("Music resource loaded successfully from: {}", filepath);
	}

	void Music::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading music resource");

		if (musicLoaded_)
		{
			UnloadMusicStream(music_);
			music_ = ::Music{};
			musicLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("Music resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload music resource that was not loaded");
		}
	}

	::Music Music::getMusic() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return music_;
	}

}
