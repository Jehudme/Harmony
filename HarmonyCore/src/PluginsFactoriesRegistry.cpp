#include "Harmony/PluginsFactoriesRegistry.h"

namespace Harmony {
	PluginsFactories& PluginsFactoriesRegistry::GetPluginsFactories() {
		static PluginsFactories factories;
		return factories;
	}
	void PluginsFactoriesRegistry::RegisterFactory(const std::string& name, PluginsFactory factory) {
		auto& factories = GetPluginsFactories();
		factories[name] = factory;
	}
	void PluginsFactoriesRegistry::UnregisterFactory(const std::string& name) {
		auto& factories = GetPluginsFactories();
		factories.erase(name);
	}
	std::unique_ptr<IPlugins> PluginsFactoriesRegistry::Create(const std::string& name, HARMONY_PLUGIN_INIT_ARGS) {
		return GetPluginsFactories().contains(name) ? GetPluginsFactories().at(name)(HARMONY_PLUGIN_INIT_ARG_NAMES): nullptr;
		// log error: factory with name not found
	}
} // namespace Harmony