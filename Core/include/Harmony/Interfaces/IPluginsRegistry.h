#pragma once
#include "Harmony/Properties.h"
#include "Harmony/Interfaces/IPlugins.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Harmony {
	class IPluginsRegistry {
	public:
		virtual void Initialize(const Properties& properties) = 0;

		virtual IPlugins* GetPlugin(const std::string& tag) const = 0;
		virtual IPlugins* operator[](const std::string& tag) const = 0;

	private:
		virtual void Register(const std::string& tag, const std::string& pluginName) = 0;
		virtual void Unregister(const std::string& tag) = 0;

		std::unordered_map<std::string, std::unique_ptr<IPlugins>> m_plugins;

	};
} // namespace Harmony