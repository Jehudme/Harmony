#include "pch.h"
#include "Images.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(image, Harmony::Resources::Images)

namespace Harmony::Resources
{

	Images::Images(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		image_{},
		imageLoaded_(false)
	{
		HARMONY_DEBUG("Images resource created with ID: {}", id);
	}

	Images::~Images()
	{
		if (imageLoaded_)
		{
			UnloadImage(image_);
			imageLoaded_ = false;
		}
	}

	const char* Images::getType() const
	{
		return "image";
	}

	void Images::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Images resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading image from file: {}", filepath);

		image_ = LoadImage(filepath.c_str());

		if (image_.data == nullptr)
		{
			HARMONY_ERROR("Failed to load image from file: {}", filepath);
			throw Exceptions::ImageLoadException(filepath, "Raylib LoadImage returned null data");
		}

		imageLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("Images resource loaded successfully from: {}", filepath);
	}

	void Images::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading images resource");

		if (imageLoaded_)
		{
			UnloadImage(image_);
			image_ = Image{};
			imageLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("Images resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload images resource that was not loaded");
		}
	}

	Image Images::getImage() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return image_;
	}

}
