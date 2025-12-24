#include "Harmony/PluginsFactoriesRegistry.h"

namespace Harmony {
	PluginsFactories& IPluginsFactoriesRegistry::GetPluginsFactories() {
		static PluginsFactories factories;
		return factories;
	}
	void IPluginsFactoriesRegistry::Register(const std::string& name, PluginsFactory factory) {
		auto& factories = GetPluginsFactories();
		factories[name] = factory;
	}
	void IPluginsFactoriesRegistry::Unregister(const std::string& name) {
		auto& factories = GetPluginsFactories();
		factories.erase(name);
	}
	std::unique_ptr<IPlugins> IPluginsFactoriesRegistry::create(const std::string& name, HARMONY_INIT_ARGS) {
		auto& factories = GetPluginsFactories();
		auto it = factories.find(name);
		if (it != factories.end()) {
			return it->second(HARMONY_INIT_PASS);
		}
		return nullptr;
	}
} // namespace Harmony