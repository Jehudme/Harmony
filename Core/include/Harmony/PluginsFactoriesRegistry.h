#pragma once

#include "Harmony/Properties.h"
#include "Harmony/Context.h"
#include "Harmony/Interfaces/IPlugins.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace Harmony {
	using PluginsFactory = std::function<std::unique_ptr<IPlugins>(HARMONY_INIT_ARGS)>;
	using PluginsFactories = std::unordered_map<std::string, PluginsFactory>;

	class IPluginsFactoriesRegistry {
	public:
		static PluginsFactories& GetPluginsFactories();

		static void Register(const std::string& name, PluginsFactory factory);
		static void Unregister(const std::string& name);

		static std::unique_ptr<IPlugins> create(const std::string& name, HARMONY_INIT_ARGS);
	};
} // namespace Harmony

#define HARMONY_REGISTER_PLUGIN(PluginClass, PluginName) \
	namespace { \
		struct PluginClass##FactoryRegistrar { \
			PluginClass##FactoryRegistrar() { \
				Harmony::PluginsRegistry::Register(PluginName, [](HARMONY_INIT_ARGS) -> std::unique_ptr<Harmony::IPlugins> { \
					auto plugin = std::make_unique<PluginClass>(); \
					plugin->Initialize(HARMONY_INIT_PASS); \
					return plugin; \
				}); \
			} \
		}; \
		static PluginClass##FactoryRegistrar global_##PluginClass##FactoryRegistrar_instance; \
	}