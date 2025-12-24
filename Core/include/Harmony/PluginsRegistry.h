#pragma once
#include "Properties.h"
#include "EngineContext.h"
#include "IPlugins.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Harmony {
	class PluginsRegistry {
	public:
		PluginsRegistry(const EngineContext& ctx);
		void Initialize(const Properties& properties);

		void Register(const std::string& tag, const std::string& pluginName);
		void Unregister(const std::string& tag);

		IPlugins* GetPlugin(const std::string& tag) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<IPlugins>> m_plugins;

	};
} // namespace Harmony