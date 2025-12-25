#pragma once

#include "Harmony/PluginsFactoriesRegistry.h"

namespace Harmony {
	template<typename PluginType>
	void PluginsFactoriesRegistry::RegisterFactory(const std::string& name) {
		RegisterFactory(name, [](HARMONY_PLUGIN_INIT_ARGS) -> std::unique_ptr<IPlugins> {
			return std::make_unique<PluginType>(context, properties);
		});
	}
	template<typename PluginType>
	std::unique_ptr<PluginType> PluginsFactoriesRegistry::Create(const std::string& name, HARMONY_PLUGIN_INIT_ARGS) {
		auto basePlugin = Create(name, context, properties);
		if (!basePlugin) {
			return nullptr; // log error: failed to create plugin with name
		}

		if (auto derivedPlugin = dynamic_cast<PluginType*>(basePlugin.release())) {
			return std::unique_ptr<PluginType>(derivedPlugin);
		}

		return nullptr; // log error: failed to cast plugin to desired type
	}
} // namespace Harmony