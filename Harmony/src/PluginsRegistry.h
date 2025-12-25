#pragma once
#include "Harmony/Context.h"
#include "Harmony/Interfaces/IPluginsRegistry.h"

#include <shared_mutex>

namespace Harmony {
	class PluginsRegistry : public IPluginsRegistry {
	public:
		PluginsRegistry(const Harmony::Context& context, const Harmony::Properties& properties);
		~PluginsRegistry();

		IPlugins* GetPlugin(const std::string& tag) const override;
		IPlugins* operator[](const std::string& tag) const override;

		bool Contains(const std::string& tag) const;

	private:
		void AddPlugin(const std::string& tag, const Properties& properties) override;
		void RemovePlugin(const std::string& tag) override;

	private:
		Context& m_context;
		mutable std::shared_mutex m_mutex;
		std::unordered_map<std::string, std::unique_ptr<IPlugins>> m_plugins;
	};
} // namespace Ham_rmony