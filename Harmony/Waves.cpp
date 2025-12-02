#include "pch.h"
#include "Waves.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(wave, Harmony::Resources::Waves)

namespace Harmony::Resources
{

	Waves::Waves(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		wave_{},
		waveLoaded_(false)
	{
		HARMONY_DEBUG("Waves resource created with ID: {}", id);
	}

	Waves::~Waves()
	{
		if (waveLoaded_)
		{
			UnloadWave(wave_);
			waveLoaded_ = false;
		}
	}

	const char* Waves::getType() const
	{
		return "wave";
	}

	void Waves::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Waves resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading wave from file: {}", filepath);

		wave_ = LoadWave(filepath.c_str());

		if (wave_.data == nullptr)
		{
			HARMONY_ERROR("Failed to load wave from file: {}", filepath);
			throw Exceptions::ResourceLoadException("Wave", filepath, "Raylib LoadWave returned null data");
		}

		waveLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("Waves resource loaded successfully from: {}", filepath);
	}

	void Waves::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading waves resource");

		if (waveLoaded_)
		{
			UnloadWave(wave_);
			wave_ = Wave{};
			waveLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("Waves resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload waves resource that was not loaded");
		}
	}

	Wave Waves::getWave() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return wave_;
	}

}
