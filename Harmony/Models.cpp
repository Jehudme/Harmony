#include "pch.h"
#include "Models.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(model, Harmony::Resources::Models)

namespace Harmony::Resources
{

	Models::Models(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		model_{},
		modelLoaded_(false)
	{
		HARMONY_DEBUG("Models resource created with ID: {}", id);
	}

	Models::~Models()
	{
		if (modelLoaded_)
		{
			UnloadModel(model_);
			modelLoaded_ = false;
		}
	}

	const char* Models::getType() const
	{
		return "model";
	}

	void Models::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Models resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading model from file: {}", filepath);

		model_ = LoadModel(filepath.c_str());

		if (model_.meshCount == 0)
		{
			HARMONY_ERROR("Failed to load model from file: {}", filepath);
			throw Exceptions::ModelLoadException(filepath, "Raylib LoadModel returned no meshes");
		}

		modelLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("Models resource loaded successfully from: {}", filepath);
	}

	void Models::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading models resource");

		if (modelLoaded_)
		{
			UnloadModel(model_);
			model_ = Model{};
			modelLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("Models resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload models resource that was not loaded");
		}
	}

	Model Models::getModel() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return model_;
	}

}
