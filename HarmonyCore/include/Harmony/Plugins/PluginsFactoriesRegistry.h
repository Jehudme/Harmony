#pragma once

#include "Harmony/Context.h"
#include "Harmony/Properties.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace Harmony
{
    class PluginsFactoriesRegistry
    {
    public:
        // ========================================================
        // Public Interface
        // ========================================================

        template<typename BasePlugin, typename ConcretePlugin, typename... Args>
        static void RegisterFactory(const std::string& name);

        // Unregisters an existing factory by name.
        template<typename BasePlugin, typename... Args>
        static void UnregisterFactory(const std::string& name);

		// Check if a factory is registered under 'name'.
		template<typename BasePlugin, typename... Args>
		static bool IsFactoryRegistered(const std::string& name);

        // Creates an instance of a plugin registered under 'name'.
        // Arguments passed here are forwarded to the plugin's constructor.
        template<typename BasePlugin, typename... Args>
        static std::unique_ptr<BasePlugin> Create(const std::string& name, Args&&... args);

    private:
        // ========================================================
        // Internal Types & Storage
        // ========================================================

        template<typename BasePlugin, typename... Args>
        using PluginFactoryFunction = std::function<std::unique_ptr<BasePlugin>(Args...)>;

        template<typename BasePlugin, typename... Args>
        using PluginsFactories = std::unordered_map<std::string, PluginFactoryFunction<BasePlugin, Args...>>;

        template<typename BasePlugin, typename... Args>
        static PluginsFactories<BasePlugin, Args...>& GetRegistry();
    };

} // namespace Harmony

namespace Harmony::Internal
{
    enum class RegistrationMode { Standard, Override };

    // ========================================================
    // PluginRegistrar Struct (RAII)
    // ========================================================
    template<typename BasePlugin, typename ConcretePlugin, typename... Args>
    struct PluginRegistrar
    {
        PluginRegistrar(const std::string& name, RegistrationMode mode)
            : m_name(name)
        {
            // If Overriding, ensure we remove any existing entry first
            if (mode == RegistrationMode::Override)
            {
                PluginsFactoriesRegistry::UnregisterFactory<BasePlugin, Args...>(name);
            }

            // Register the new factory
            PluginsFactoriesRegistry::RegisterFactory<BasePlugin, ConcretePlugin, Args...>(name);
        }

        ~PluginRegistrar()
        {
            // Auto-cleanup on program exit / DLL unload
            PluginsFactoriesRegistry::UnregisterFactory<BasePlugin, Args...>(m_name);
        }

        std::string m_name;
    };
}

// ========================================================
// Helper: Unique Variable Generator
// ========================================================
#define HARMONY_CAT_IMPL(s1, s2) s1##s2
#define HARMONY_CAT(s1, s2) HARMONY_CAT_IMPL(s1, s2)
#define HARMONY_UNIQUE_VAR(prefix) HARMONY_CAT(prefix, __LINE__)


// ========================================================
// 1. REGISTER (Static / Global Scope)
// ========================================================
// Registers a class at startup.
// Usage: HARMONY_REGISTER_PLUGIN(IWidget, Button, "MyButton", int, std::string)
#define HARMONY_REGISTER_PLUGIN(Base, Concrete, Name, ...) \
    static Harmony::Internal::PluginRegistrar<Base, Concrete, ##__VA_ARGS__> \
    HARMONY_UNIQUE_VAR(harmony_reg_)(Name, Harmony::Internal::RegistrationMode::Standard)


// ========================================================
// 2. OVERRIDE (Static / Global Scope)
// ========================================================
// Unregisters any existing factory with this name, then registers this one.
// Usage: HARMONY_OVERRIDE_PLUGIN(IWidget, NewButton, "MyButton", int, std::string)
#define HARMONY_OVERRIDE_PLUGIN(Base, Concrete, Name, ...) \
    static Harmony::Internal::PluginRegistrar<Base, Concrete, ##__VA_ARGS__> \
    HARMONY_UNIQUE_VAR(harmony_ovr_)(Name, Harmony::Internal::RegistrationMode::Override)


// ========================================================
// 3. CREATE INSTANCE (Runtime)
// ========================================================
// Short-hand to create a plugin instance.
// Usage: auto btn = HARMONY_CREATE_PLUGIN(IWidget, "MyButton", 10, "Label");
#define HARMONY_CREATE_PLUGIN(Base, Name, ...) \
    Harmony::PluginsFactoriesRegistry::Create<Base>(Name, ##__VA_ARGS__)


// ========================================================
// 4. UNREGISTER (Runtime)
// ========================================================
// Manually unregister a factory at runtime.
// Usage: HARMONY_UNREGISTER_PLUGIN(IWidget, "MyButton", int, std::string)
// Note: You must provide the argument types so the registry template can be found.
#define HARMONY_UNREGISTER_PLUGIN(Base, Name, ...) \
    Harmony::PluginsFactoriesRegistry::UnregisterFactory<Base, ##__VA_ARGS__>(Name)

#include "PluginsFactoriesRegistry.inl"
