#include "SystemsRegistry.h"
#include <Harmony/Interfaces/ISystem.h>
#include <Harmony/Logger.h>
#include <Harmony/Plugins/PluginsFactoriesRegistry.h>

namespace Harmony
{
SystemsRegistry::State SystemsRegistry::GetState() const
{
    return m_state;
}
ISystem* SystemsRegistry::GetSystem(const std::string role)
{
    if (!m_systems.contains(role))
    {
        HARMONY_ERROR("System with role '{}' not found in SystemsRegistry.", role);
    }

    return m_systems[role].get();
}
void SystemsRegistry::CreateSystem(const std::string name)
{
    if (m_systems.contains(name))
    {
        HARMONY_ERROR("System with role '{}' already exists in SystemsRegistry.", name);
        return;
    }

    std::unique_ptr<ISystem> system = HARMONY_CREATE_SYSTEM(name);

    std::string role = system->GetRole();
    m_systems[role] = std::move(system);
}
void SystemsRegistry::DeleteSystem(const std::string role)
{
    if (!m_systems.contains(role))
    {
        HARMONY_ERROR("System with role '{}' not found in SystemsRegistry.", role);
        return;
    }

    m_systems.erase(role);
}
bool SystemsRegistry::ContainsSystem(const std::string role) const
{
    return m_systems.contains(role);
}
void SystemsRegistry::Initialize(const Properties& properties)
{
    std::vector<std::string> systemsListKeys = properties.ExtractKeys({});

    for (const auto& systemKey : systemsListKeys)
    {
        CreateSystem(systemKey);
    }

    for (auto& [role, system] : m_systems)
    {
        system->Initialize(properties[role]);

        HARMONY_INFO("System '{}' with role '{}' initialized.", system->GetName(), role);
    }
}
void SystemsRegistry::Finalize() {
    for (auto& [role, system] : m_systems)
    {
        system->Finalize();
        HARMONY_INFO("System '{}' with role '{}' initialized.", system->GetName(), role);
    }
}
} // namespace Harmony