#pragma once

#include "Resource.h"
#include "Configuration.h"

#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>
#include <shared_mutex>
#include <functional>

#include "Resource.h"

namespace Harmony::Internals {
	class Engine;
}

namespace Harmony::Internals {
	class ResourcesHandler {
		friend class Engine;

		public:
			ResourcesHandler(Engine& engine);
			~ResourcesHandler();

			ResourcesHandler(const ResourcesHandler&) = delete;
			ResourcesHandler& operator=(const ResourcesHandler&) = delete;
			ResourcesHandler(ResourcesHandler&&) = delete;
			ResourcesHandler& operator=(ResourcesHandler&&) = delete;

			void update();

			void loadResource(Resources::ResourceID id);
			void unloadResource(Resources::ResourceID id);

			std::unique_ptr<Resources::ResourceAcquirement> acquireResource(Resources::ResourceID id);

	private:
		Engine& engine_;
		mutable std::shared_mutex mutex_;
		std::unordered_map<Resources::ResourceID, std::unique_ptr<Resources::Resource>> resources_;
		std::unordered_map<std::string, std::function<std::unique_ptr<Resources::Resource>(Resources::ResourceID, Configuration)>> resourcesFactories_;
	};

} // namespace Harmony