#pragma once

#include "Harmony/PluginsFactoriesRegistry.h"
#include "Harmony/Logger.h"

namespace Harmony 
{
    // ========================================================
    // Plugin Factory Template Implementations
    // ========================================================

    template<typename PluginType>
    void PluginsFactoriesRegistry::RegisterFactory(const std::string& name) 
    {
        RegisterFactory(name, [](HARMONY_PLUGIN_INIT_ARGS) -> std::unique_ptr<IPlugins> 
        {
            return std::make_unique<PluginType>(context, properties);
        });
    }

    template<typename PluginType>
    std::unique_ptr<PluginType> PluginsFactoriesRegistry::Create(const std::string& name, HARMONY_PLUGIN_INIT_ARGS) 
    {
        auto basePlugin = Create(name, context, properties);
        
        if (!basePlugin) 
        {
            HARMONY_ERROR("PluginsFactoriesRegistry::Create - Failed to create plugin with name: {}", name);
            return nullptr;
        }

        PluginType* derivedPlugin = dynamic_cast<PluginType*>(basePlugin.release());
        
        if (!derivedPlugin) 
        {
            HARMONY_ERROR("PluginsFactoriesRegistry::Create - Failed to cast plugin '{}' to desired type", name);
            return nullptr;
        }

        return std::unique_ptr<PluginType>(derivedPlugin);
    }

} // namespace Harmony