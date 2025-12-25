#include "PluginsRegistry.h"
#include "Harmony/PluginsFactoriesRegistry.h"

namespace Harmony {

	PluginsRegistry::PluginsRegistry(const Harmony::Context& context, const Harmony::Properties& properties) :
		m_context(const_cast<Harmony::Context&>(context))
	{
		for (const std::string& tag : properties.extractKeys({})) {
			if (auto pluginNameOpt = properties.get<std::string>({ tag, "name" })) {
				AddPlugin(tag, properties.subsection({tag}).value());
			}
			//log error: plugin name not found in properties
		}
	}

	PluginsRegistry::~PluginsRegistry() = default;

	IPlugins* PluginsRegistry::GetPlugin(const std::string& tag) const
	{
		if (m_plugins.contains(tag))
			return  m_plugins.at(tag).get();

		// log error: plugin not found for tag
		return nullptr;
	}

	IPlugins* PluginsRegistry::operator[](const std::string& tag) const
	{
		return GetPlugin(tag);
	}
	bool PluginsRegistry::Contains(const std::string& tag) const
	{
		std::shared_lock lock(m_mutex);
		return m_plugins.contains(tag);
	}
	void PluginsRegistry::AddPlugin(const std::string& tag, const Properties& properties)
	{
		std::lock_guard lock(m_mutex);
		if (m_plugins.contains(tag)) {
			return; // log error : plugin with tag already exists
		}

		std::optional<std::string> pluginNameOpt;
		if (pluginNameOpt = properties.get<std::string>({ "name" }); !pluginNameOpt.has_value()) {
			return; // log error: plugin name not found in properties
		}

		if (auto plugin = PluginsFactoriesRegistry::Create(pluginNameOpt.value(), m_context, properties)) {
			m_plugins[tag] = std::move(plugin);
		}

		// log error: failed to create plugin with name
	}

	void PluginsRegistry::RemovePlugin(const std::string& tag)
	{
		std::lock_guard lock(m_mutex);
		if (!m_plugins.contains(tag)) {
			return; // log warn: plugin with tag does not exist
		}
		m_plugins.erase(tag);
	}
}