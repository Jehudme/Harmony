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
			const std::string resourceType = resourceConfiguration.get<std::string>({ "Type" }).value();

			std::unique_ptr<Resources::Resource> resource = resourcesFactories_[resourceType](resourceID, resourceConfiguration);
			resources_.emplace(resourceID, std::move(resource));
		}
	}
}