#pragma once

#include "Harmony/Context.h"
#include "Harmony/Properties.h"
#include "Harmony/Interfaces/IPlugins.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace Harmony {
	using PluginsFactory = std::function<std::unique_ptr<IPlugins>(HARMONY_PLUGIN_INIT_ARGS)>;
	using PluginsFactories = std::unordered_map<std::string, PluginsFactory>;

	class PluginsFactoriesRegistry {
	public:
		static PluginsFactories& GetPluginsFactories();

		static void RegisterFactory(const std::string& name, PluginsFactory factory);
		static void UnregisterFactory(const std::string& name);

		static std::unique_ptr<IPlugins> Create(const std::string& name, HARMONY_PLUGIN_INIT_ARGS);

		template<typename PluginType>
		static void RegisterFactory(const std::string& name);
		
		template<typename PluginType>
		static std::unique_ptr<PluginType> Create(const std::string& name, HARMONY_PLUGIN_INIT_ARGS);

	};
} // namespace Harmony

#include "Harmony/PluginsFactoriesRegistry.inl"