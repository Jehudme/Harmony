#include "pch.h"
#include "RenderTextures.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(rendertexture, Harmony::Resources::RenderTextures)

namespace Harmony::Resources
{

	RenderTextures::RenderTextures(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		renderTexture_{}
	{
		HARMONY_DEBUG("RenderTextures resource created with ID: {}", id);
	}

	RenderTextures::~RenderTextures()
	{
		if (loaded_)
		{
			UnloadRenderTexture(renderTexture_);
			loaded_ = false;
		}
	}

	const char* RenderTextures::getType() const
	{
		return "rendertexture";
	}

	void RenderTextures::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<int> widthOpt = configuration_.get<int>({ "width" });
		std::optional<int> heightOpt = configuration_.get<int>({ "height" });

		if (!widthOpt.has_value() || !heightOpt.has_value())
		{
			HARMONY_ERROR("RenderTextures resource load failed: 'width' or 'height' not specified in configuration");
			throw Exceptions::ConfigurationException("'width' and 'height' must be specified in configuration");
		}

		int width = widthOpt.value();
		int height = heightOpt.value();

		if (width <= 0 || height <= 0)
		{
			HARMONY_ERROR("RenderTextures resource load failed: invalid dimensions {}x{}", width, height);
			throw Exceptions::InvalidArgumentException("width/height", "Dimensions must be positive");
		}

		HARMONY_DEBUG("Loading render texture with dimensions: {}x{}", width, height);

		renderTexture_ = LoadRenderTexture(width, height);

		if (renderTexture_.id == 0)
		{
			HARMONY_ERROR("Failed to create render texture with dimensions: {}x{}", width, height);
			throw Exceptions::RenderTextureLoadException("Raylib LoadRenderTexture returned invalid texture ID");
		}

		loaded_ = true;
		HARMONY_INFO("RenderTextures resource loaded successfully with dimensions: {}x{}", width, height);
	}

	void RenderTextures::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading render textures resource");

		if (loaded_)
		{
			UnloadRenderTexture(renderTexture_);
			renderTexture_ = RenderTexture2D{};
			loaded_ = false;
			HARMONY_INFO("RenderTextures resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload render textures resource that was not loaded");
		}
	}

	RenderTexture2D RenderTextures::getRenderTexture() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return renderTexture_;
	}

}
