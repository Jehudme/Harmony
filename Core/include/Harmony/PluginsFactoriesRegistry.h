#pragma once

#include "Base.h"
#include "Properties.h"
#include "EngineContext.h"
#include "IPlugins.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace Harmony {
	using PluginsFactory = std::function<std::unique_ptr<IPlugins>(HARMONY_INIT_ARGS)>;
	using PluginsFactories = std::unordered_map<std::string, PluginsFactory>;

	class PluginsRegistry {
	public:
		static PluginsFactories& GetPluginsFactories();

		static void Register(const std::string& name, PluginsFactory factory);
		static void Unregister(const std::string& name);

		static std::unique_ptr<IPlugins> create(const std::string& name, HARMONY_INIT_ARGS);
	};
} // namespace Harmony