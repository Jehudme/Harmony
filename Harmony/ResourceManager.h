#pragma once
#include "Configuration.h"

namespace Harmony {
	class Engine;
}

namespace Harmony::Resources {
	class Resource;
}

namespace Harmony::Management 
{
	class ResourceManager
	{
	public:
		ResourceManager(Engine& engine);
		~ResourceManager();

		Resources::Resource& get(const std::string& type, const Utilities::UUID resourceId);

		void load(const std::string& type, const Utilities::UUID resourceId);
		void unload(const Utilities::UUID resourceId);
		void unloadAll();

	private:
		Engine& engine_;
		std::unordered_map<Utilities::UUID, std::unique_ptr<Resources::Resource>> resources_;
	};
}

