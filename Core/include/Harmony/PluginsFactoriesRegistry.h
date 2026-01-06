#pragma once

#include "Harmony/Properties.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Harmony
{
class PluginsFactoriesRegistry
{
public:
    template <typename BasePlugin, typename ConcretePlugin, typename... Args>
    static void RegisterFactory(const std::string& name);

    template <typename BasePlugin, typename... Args>
    static std::unique_ptr<BasePlugin> Create(const std::string& name, Args&&... args);

    template <typename BasePlugin, typename... Args>
    static void UnregisterFactory(const std::string& name);

    template <typename BasePlugin, typename... Args>
    using PluginFactoryFunction = std::function<std::unique_ptr<BasePlugin>(Args...)>;

    template <typename BasePlugin, typename... Args>
    using PluginsFactories =
        std::unordered_map<std::string, PluginFactoryFunction<BasePlugin, Args...>>;

    template <typename BasePlugin, typename... Args>
    static PluginsFactories<BasePlugin, Args...>& GetRegistry();
};

} // namespace Harmony

namespace Harmony::Internal
{
enum class RegistrationMode
{
    Standard,
    Override
};
template <typename BasePlugin, typename ConcretePlugin, typename... Args>
struct PluginRegistrar
{
    PluginRegistrar(const std::string& name, RegistrationMode mode) : m_name(name)
    {
        if (mode == RegistrationMode::Override)
        {
            PluginsFactoriesRegistry::UnregisterFactory<BasePlugin, Args...>(name);
        }

        PluginsFactoriesRegistry::RegisterFactory<BasePlugin, ConcretePlugin, Args...>(name);
    }

    ~PluginRegistrar()
    {
        PluginsFactoriesRegistry::UnregisterFactory<BasePlugin, Args...>(m_name);
    }

    std::string m_name;
};
} // namespace Harmony::Internal

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
// __VA_OPT__(,) ensures the comma is only present if variadic args exist.
#define HARMONY_REGISTER_PLUGIN(Base, Concrete, Name, ...)                                         \
    static Harmony::Internal::PluginRegistrar<Base, Concrete __VA_OPT__(, ) __VA_ARGS__>           \
    HARMONY_UNIQUE_VAR(harmony_reg_)(Name, Harmony::Internal::RegistrationMode::Standard)

// ========================================================
// 2. OVERRIDE (Static / Global Scope)
// ========================================================
// Unregisters any existing factory with this name, then registers this one.
#define HARMONY_OVERRIDE_PLUGIN(Base, Concrete, Name, ...)                                         \
    static Harmony::Internal::PluginRegistrar<Base, Concrete __VA_OPT__(, ) __VA_ARGS__>           \
    HARMONY_UNIQUE_VAR(harmony_ovr_)(Name, Harmony::Internal::RegistrationMode::Override)

// ========================================================
// 3. CREATE INSTANCE (Runtime)
// ========================================================
// Short-hand to create a plugin instance.
#define HARMONY_CREATE_PLUGIN(Base, Name, ...)                                                     \
    Harmony::PluginsFactoriesRegistry::Create<Base>(Name __VA_OPT__(, ) __VA_ARGS__)

// ========================================================
// 4. UNREGISTER (Runtime)
// ========================================================
// Manually unregister a factory at runtime.
#define HARMONY_UNREGISTER_PLUGIN(Base, Name, ...)                                                 \
    Harmony::PluginsFactoriesRegistry::UnregisterFactory<Base __VA_OPT__(, ) __VA_ARGS__>(Name)

#include "PluginsFactoriesRegistry.inl"
