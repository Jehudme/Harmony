#pragma once

#include <stdexcept>

namespace Harmony
{
    // ========================================================
    // Register Factory Implementation
    // ========================================================
    template<typename BasePlugin, typename ConcretePlugin, typename... Args>
    void PluginsFactoriesRegistry::RegisterFactory(const std::string& name)
    {
        // Access the specific registry for this interface and argument list
        auto& registry = GetRegistry<BasePlugin, Args...>();

        // Store a lambda that creates the ConcretePlugin
        registry[name] = [](Args&&... args) -> std::unique_ptr<BasePlugin> {
            return std::make_unique<ConcretePlugin>(std::forward<Args>(args)...);
            };
    }

    // ========================================================
    // Unregister Factory Implementation
    // ========================================================
    template<typename BasePlugin, typename... Args>
    void PluginsFactoriesRegistry::UnregisterFactory(const std::string& name)
    {
        auto& registry = GetRegistry<BasePlugin, Args...>();
        registry.erase(name);
    }

    // ========================================================
    // Create Implementation
    // ========================================================
    template<typename BasePlugin, typename... Args>
    std::unique_ptr<BasePlugin> PluginsFactoriesRegistry::Create(const std::string& name, Args&&... args)
    {
        auto& registry = GetRegistry<BasePlugin, Args...>();

        auto it = registry.find(name);
        if (it != registry.end())
        {
            // Invoke the factory function with forwarded arguments
            return it->second(std::forward<Args>(args)...);
        }

        // Return nullptr if not found (or throw std::runtime_error if preferred)
        return nullptr;
    }

    // ========================================================
    // GetRegistry Implementation (Meyers Singleton)
    // ========================================================
    template<typename BasePlugin, typename... Args>
    std::unordered_map<std::string, std::function<std::unique_ptr<BasePlugin>(Args...)>>&
        PluginsFactoriesRegistry::GetRegistry()
    {
        // This static variable is unique for every combination of template arguments.
        static std::unordered_map<std::string, std::function<std::unique_ptr<BasePlugin>(Args...)>> registry;
        return registry;
    }

} // namespace Harmony