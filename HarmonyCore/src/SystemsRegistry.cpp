#include "Harmony/Interfaces/ISystem.h"
#include "Harmony/Plugins/SystemsRegistry.h"
#include "Harmony/Logger.h"

namespace Harmony {
	void SystemsRegistry::Create(const std::string name)
	{
		std::unique_ptr<ISystem> system = HARMONY_CREATE_SYSTEM(name);

		if (m_systems.contains(system->GetRole()))
			return HARMONY_ERROR("System with role {} already exist");

		m_systems[system->GetRole()] = std::move(system);
	}
	void SystemsRegistry::Delete(const std::string role)
	{
		if (!m_systems.contains(role))
			return HARMONY_ERROR("System with role {} do not exist");

		m_systems.erase(role);
	}
	ISystem* SystemsRegistry::Get(const std::string role)
	{
		if (!m_systems.contains(role))
		{
			HARMONY_ERROR("System with role {} do not exist", role);
			return nullptr;
		}

		return m_systems[role].get();
	}
	bool SystemsRegistry::Contains(const std::string role) const
	{
		return m_systems.contains(role);
	}
}