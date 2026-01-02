#pragma once

#include <Harmony/Assert.h>
#include <stdexcept>

namespace Harmony
{
template <typename BasePlugin, typename ConcretePlugin, typename... Args>
void PluginsFactoriesRegistry::RegisterFactory(const std::string& name)
{
    auto& registry = GetRegistry<BasePlugin, Args...>();
    registry[name] = [](Args&&... args) -> std::unique_ptr<BasePlugin>
    { return std::make_unique<ConcretePlugin>(std::forward<Args>(args)...); };
}

template <typename BasePlugin, typename... Args>
void PluginsFactoriesRegistry::UnregisterFactory(const std::string& name)
{
    auto& registry = GetRegistry<BasePlugin, Args...>();
    registry.erase(name);
}

template <typename BasePlugin, typename... Args>
std::unique_ptr<BasePlugin> PluginsFactoriesRegistry::Create(const std::string& name,
                                                             Args&&... args)
{
    auto& registry = GetRegistry<BasePlugin, Args...>();

    auto it = registry.find(name);
    if (it != registry.end()) {
        return it->second(std::forward<Args>(args)...);
    }

    return nullptr;
}

template <typename BasePlugin, typename... Args>
std::unordered_map<std::string, std::function<std::unique_ptr<BasePlugin>(Args...)>>&
PluginsFactoriesRegistry::GetRegistry()
{
    static std::unordered_map<std::string, std::function<std::unique_ptr<BasePlugin>(Args...)>>
        registry;
    return registry;
}

} // namespace Harmony