#include "PluginsRegistry.h"
#include "Harmony/PluginsFactoriesRegistry.h"
#include "Harmony/Logger.h"
#include "Harmony/Assert.h"

namespace Harmony 
{
    // ========================================================
    // PluginsRegistry Constructor and Destructor
    // ========================================================

    PluginsRegistry::PluginsRegistry(const Harmony::Context& context, const Harmony::Properties& properties) :
        m_context(const_cast<Harmony::Context&>(context))
    {
        std::vector<std::string> pluginTags = properties.ExtractKeys({});
        
        HARMONY_INFO("PluginsRegistry - Initializing with {} plugin(s)", pluginTags.size());

        for (const std::string& tag : pluginTags) 
        {
            std::optional<std::string> pluginNameOptional = properties.Get<std::string>({ tag, "name" });
            
            if (!pluginNameOptional.has_value()) 
            {
                HARMONY_ERROR("PluginsRegistry - Plugin name not found in properties for tag: {}", tag);
                continue;
            }

            std::optional<Properties> pluginSubsection = properties.Subsection({tag});
            
            if (pluginSubsection.has_value())
            {
                AddPlugin(tag, pluginSubsection.value());
            }
            else
            {
                HARMONY_ERROR("PluginsRegistry - Failed to get subsection for plugin tag: {}", tag);
            }
        }
    }

    PluginsRegistry::~PluginsRegistry() 
    {
        HARMONY_INFO("PluginsRegistry - Destroying registry with {} plugin(s)", m_plugins.size());
    }

    // ========================================================
    // PluginsRegistry Public Methods
    // ========================================================

    IPlugins* PluginsRegistry::GetPlugin(const std::string& tag) const
    {
        std::shared_lock lock(m_mutex);
        
        if (!m_plugins.contains(tag))
        {
            HARMONY_ERROR("PluginsRegistry::GetPlugin - Plugin not found for tag: {}", tag);
            return nullptr;
        }

        return m_plugins.at(tag).get();
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

    // ========================================================
    // PluginsRegistry Private Methods
    // ========================================================

    void PluginsRegistry::AddPlugin(const std::string& tag, const Properties& properties)
    {
        std::unique_lock lock(m_mutex);
        
        if (m_plugins.contains(tag)) 
        {
            HARMONY_ERROR("PluginsRegistry::AddPlugin - Plugin with tag '{}' already exists", tag);
            return;
        }

        std::optional<std::string> pluginNameOptional = properties.Get<std::string>({ "name" });
        
        if (!pluginNameOptional.has_value()) 
        {
            HARMONY_ERROR("PluginsRegistry::AddPlugin - Plugin name not found in properties for tag: {}", tag);
            return;
        }

        std::string pluginName = pluginNameOptional.value();
        std::unique_ptr<IPlugins> plugin = PluginsFactoriesRegistry::Create(pluginName, m_context, properties);
        
        if (!plugin) 
        {
            HARMONY_ERROR("PluginsRegistry::AddPlugin - Failed to create plugin '{}' for tag: {}", pluginName, tag);
            return;
        }

        m_plugins[tag] = std::move(plugin);
        HARMONY_INFO("PluginsRegistry::AddPlugin - Successfully added plugin '{}' with tag: {}", pluginName, tag);
    }

    void PluginsRegistry::RemovePlugin(const std::string& tag)
    {
        std::unique_lock lock(m_mutex);
        
        if (!m_plugins.contains(tag)) 
        {
            HARMONY_WARN("PluginsRegistry::RemovePlugin - Plugin with tag '{}' does not exist", tag);
            return;
        }

        m_plugins.erase(tag);
        HARMONY_INFO("PluginsRegistry::RemovePlugin - Removed plugin with tag: {}", tag);
    }

} // namespace Harmony