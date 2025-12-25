#include "Harmony/PluginsFactoriesRegistry.h"
#include "Harmony/Logger.h"
#include "Harmony/Assert.h"

namespace Harmony 
{
    // ========================================================
    // Plugin Factories Registry Implementation
    // ========================================================

    PluginsFactories& PluginsFactoriesRegistry::GetPluginsFactories() 
    {
        static PluginsFactories factories;
        return factories;
    }

    void PluginsFactoriesRegistry::RegisterFactory(const std::string& name, PluginsFactory factory) 
    {
        HARMONY_ASSERT(!name.empty(), "Plugin factory name cannot be empty");
        HARMONY_ASSERT(factory, "Plugin factory function cannot be empty");

        auto& factories = GetPluginsFactories();
        
        if (factories.contains(name))
        {
            HARMONY_WARN("PluginsFactoriesRegistry::RegisterFactory - Overriding existing factory: {}", name);
        }

        factories[name] = factory;
        HARMONY_INFO("PluginsFactoriesRegistry::RegisterFactory - Registered factory: {}", name);
    }

    void PluginsFactoriesRegistry::UnregisterFactory(const std::string& name) 
    {
        auto& factories = GetPluginsFactories();
        
        if (!factories.contains(name))
        {
            HARMONY_WARN("PluginsFactoriesRegistry::UnregisterFactory - Factory '{}' does not exist", name);
            return;
        }

        factories.erase(name);
        HARMONY_INFO("PluginsFactoriesRegistry::UnregisterFactory - Unregistered factory: {}", name);
    }

    std::unique_ptr<IPlugins> PluginsFactoriesRegistry::Create(const std::string& name, HARMONY_PLUGIN_INIT_ARGS) 
    {
        const auto& factories = GetPluginsFactories();
        
        if (!factories.contains(name))
        {
            HARMONY_ERROR("PluginsFactoriesRegistry::Create - Factory with name '{}' not found", name);
            return nullptr;
        }

        std::unique_ptr<IPlugins> plugin = factories.at(name)(HARMONY_PLUGIN_INIT_ARG_NAMES);
        
        if (!plugin)
        {
            HARMONY_ERROR("PluginsFactoriesRegistry::Create - Factory '{}' returned null plugin", name);
            return nullptr;
        }

        HARMONY_INFO("PluginsFactoriesRegistry::Create - Successfully created plugin: {}", name);
        return plugin;
    }

} // namespace Harmony