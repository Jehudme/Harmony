#include "pch.h"
#include "ResourceHandler.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assert.h"
#include "Engine.h"
#include "ConfigurationHandler.h"

#include <algorithm>

namespace Harmony::Internals {
	ResourcesHandler::ResourcesHandler(Engine& engine) :
		engine_(engine)
	{
		Configuration configuration = engine_.configurationHandler->getConfiguration().subsection({ "Resources" }).value_or(Configuration());
		for (const std::string& key : configuration.extractKeys({})) {
			const Configuration resourceConfiguration = configuration.subsection({ key }).value();
			const Resources::ResourceID resourceID = std::stoi(key);
			const std::string resourceType = resourceConfiguration.get<std::string>({ "type" }).value();

			std::unique_ptr<Resources::Resource> resource = resourcesFactories_[resourceType](resourceID, resourceConfiguration);
			resources_.emplace(resourceID, std::move(resource));
		}
	}

	ResourcesHandler::~ResourcesHandler() = default;

	void ResourcesHandler::update()
	{
		std::shared_lock lock(mutex_);
		for (auto& [id, resource] : resources_) {
			if (resource->canUnload()) {
				resource->unload();
			}
		}
	}

	void ResourcesHandler::loadResource(Resources::ResourceID id)
	{
		if (!resources_.contains(id)) throw Exceptions::ResourceNotFoundException("Resource", id);
		resources_.at(id)->load();
	}

	void ResourcesHandler::unloadResource(Resources::ResourceID id)
	{
		if (!resources_.contains(id)) throw Exceptions::ResourceNotFoundException("Resource", id);
		resources_.at(id)->unload();
	}
	
	std::unique_ptr<Resources::ResourceAcquirement> ResourcesHandler::acquireResource(Resources::ResourceID id)
	{
		if (!resources_.contains(id)) throw Exceptions::ResourceNotFoundException("Resource", id);
		if (!resources_[id]->isAvailable()) resources_[id]->load();

		return std::make_unique<Resources::ResourceAcquirement>(std::shared_lock<std::shared_mutex>(resources_.at(id)->mutex_), *resources_.at(id));
	}
	void ResourcesHandler::registerResourceType(const std::string& typeName, std::function<std::unique_ptr<Resources::Resource>(Resources::ResourceID, Configuration)> factoryFunction) {
		resourcesFactories_.emplace(typeName, factoryFunction);
	}
}