#include "pch.h"
#include "ModelAnimations.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(modelanimation, Harmony::Resources::ModelAnimations)

namespace Harmony::Resources
{

	ModelAnimations::ModelAnimations(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		animations_(nullptr),
		animationCount_(0),
		animationsLoaded_(false)
	{
		HARMONY_DEBUG("ModelAnimations resource created with ID: {}", id);
	}

	ModelAnimations::~ModelAnimations()
	{
		if (animationsLoaded_ && animations_ != nullptr)
		{
			UnloadModelAnimations(animations_, animationCount_);
			animations_ = nullptr;
			animationCount_ = 0;
			animationsLoaded_ = false;
		}
	}

	const char* ModelAnimations::getType() const
	{
		return "modelanimation";
	}

	void ModelAnimations::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("ModelAnimations resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading model animations from file: {}", filepath);

		animations_ = LoadModelAnimations(filepath.c_str(), &animationCount_);

		if (animations_ == nullptr || animationCount_ == 0)
		{
			HARMONY_ERROR("Failed to load model animations from file: {}", filepath);
			throw Exceptions::ModelAnimationLoadException(filepath, "Raylib LoadModelAnimations returned no animations");
		}

		animationsLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("ModelAnimations resource loaded successfully from: {} with {} animations", filepath, animationCount_);
	}

	void ModelAnimations::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading model animations resource");

		if (animationsLoaded_ && animations_ != nullptr)
		{
			UnloadModelAnimations(animations_, animationCount_);
			animations_ = nullptr;
			animationCount_ = 0;
			animationsLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("ModelAnimations resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload model animations resource that was not loaded");
		}
	}

	ModelAnimation* ModelAnimations::getAnimations() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return animations_;
	}

	int ModelAnimations::getAnimationCount() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return animationCount_;
	}

}
