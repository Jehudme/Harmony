#include "pch.h"
#include "Textures.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(texture, Harmony::Resources::Textures)

namespace Harmony::Resources
{

	Textures::Textures(ResourceID id, const Configuration& configuration, Harmony::Internals::ResourcesHandler& handler) :
		Resource(id, configuration, handler),
		texture_{}
	{
		HARMONY_DEBUG("Textures resource created with ID: {}", id);
	}

	Textures::~Textures()
	{
		if (loaded_)
		{
			UnloadTexture(texture_);
			loaded_ = false;
		}
	}

	const char* Textures::getType() const
	{
		return "texture";
	}

	void Textures::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Textures resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading texture from file: {}", filepath);

		texture_ = LoadTexture(filepath.c_str());

		if (texture_.id == 0)
		{
			HARMONY_ERROR("Failed to load texture from file: {}", filepath);
			throw Exceptions::TextureLoadException(filepath, "Raylib LoadTexture returned invalid texture ID");
		}

		loaded_ = true;
		HARMONY_INFO("Textures resource loaded successfully from: {}", filepath);
	}

	void Textures::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading textures resource");

		if (loaded_)
		{
			UnloadTexture(texture_);
			texture_ = Texture2D{};
			loaded_ = false;
			HARMONY_INFO("Textures resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload textures resource that was not loaded");
		}
	}

	Texture2D Textures::getTexture() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return texture_;
	}

}
