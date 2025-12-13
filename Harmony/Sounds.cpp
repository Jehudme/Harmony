#include "pch.h"
#include "Sounds.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(sound, Harmony::Resources::Sounds)

namespace Harmony::Resources
{

	Sounds::Sounds(ResourceID id, const Configuration& configuration, Harmony::Internals::ResourcesHandler& handler) :
		Resource(id, configuration, handler),
		sound_{}
	{
		HARMONY_DEBUG("Sounds resource created with ID: {}", id);
	}

	Sounds::~Sounds()
	{
		if (loaded_)
		{
			UnloadSound(sound_);
			loaded_ = false;
		}
	}

	const char* Sounds::getType() const
	{
		return "sound";
	}

	void Sounds::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Sounds resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading sound from file: {}", filepath);

		sound_ = LoadSound(filepath.c_str());

		if (sound_.frameCount == 0)
		{
			HARMONY_ERROR("Failed to load sound from file: {}", filepath);
			throw Exceptions::SoundLoadException(filepath, "Raylib LoadSound returned invalid sound data");
		}

		loaded_ = true;
		HARMONY_INFO("Sounds resource loaded successfully from: {}", filepath);
	}

	void Sounds::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading sounds resource");

		if (loaded_)
		{
			UnloadSound(sound_);
			sound_ = Sound{};
			loaded_ = false;
			HARMONY_INFO("Sounds resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload sounds resource that was not loaded");
		}
	}

	Sound Sounds::getSound() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return sound_;
	}

}
