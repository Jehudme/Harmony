#include "pch.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Exceptions.h"
#include "Resource.h"
#include "Texture.h"
#include "Font.h"
#include "Shader.h"
#include "Sound.h"
#include "Music.h"

namespace Harmony::Management 
{
	Harmony::Management::ResourceManager::ResourceManager(Engine& engine)
		: engine_(engine) {}

	ResourceManager::~ResourceManager() = default;

	Resources::Resource& ResourceManager::get(const std::string& type, const Utilities::UUID resourceId)
	{
		if (!resources_.contains(resourceId))
			load(type, resourceId);
			
		return *resources_[resourceId];
	}
	void ResourceManager::load(const std::string& type, const Utilities::UUID resourceId)
	{
		if (resources_.contains(resourceId)) {
			HARMONY_WARN("Resource {} of type '{}' already loaded", resourceId, type);
			return;
		}

		std::optional<Utilities::Configuration> configuration;
		if (configuration = engine_.configuration.subsection({ "resources", type, std::to_string(resourceId) }); !configuration.has_value())
			throw Exceptions::ConfigurationExceptions("Resource configuration not found for resource ID " + std::to_string(resourceId));

		std::unique_ptr<Resources::Resource> resource;
		if (type == "texture") {
			resource = std::make_unique<Resources::Texture>(configuration.value());

			resources_[resourceId] = std::move(resource);
			HARMONY_INFO("Texture resource {} loaded", resourceId);
		}
		else if (type == "font") {
			resource = std::make_unique<Resources::Font>(configuration.value());

			resources_[resourceId] = std::move(resource);
			HARMONY_INFO("Font resource {} loaded", resourceId);
		}
		else if (type == "shader") {
			resource = std::make_unique<Resources::Shader>(configuration.value());

			resources_[resourceId] = std::move(resource);
			HARMONY_INFO("Shader resource {} loaded", resourceId);
		}
		else if (type == "sound") {
			resource = std::make_unique<Resources::Sound>(configuration.value());

			resources_[resourceId] = std::move(resource);
			HARMONY_INFO("Sound resource {} loaded", resourceId);
		}
		else if (type == "music") {
			resource = std::make_unique<Resources::Music>(configuration.value());

			resources_[resourceId] = std::move(resource);
			HARMONY_INFO("Music resource {} loaded", resourceId);
		}
		else throw Exceptions::ResourceLoadException(type, resourceId, "Unsupported resource type");
	}
	void ResourceManager::unload(const Utilities::UUID resourceId)
	{
		if (!resources_.contains(resourceId)) {
			HARMONY_WARN("Resource {} not loaded, cannot unload", resourceId);
			return;
		}
		resources_.erase(resourceId);
		HARMONY_INFO("Resource {} unloaded", resourceId);
	}

	void ResourceManager::unloadAll()
	{
		resources_.clear();
		HARMONY_INFO("All resources unloaded");
	}
}
