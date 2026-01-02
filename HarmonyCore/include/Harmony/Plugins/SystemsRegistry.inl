#include "Harmony/Interfaces/ISystem.h"
#include "Harmony/Logger.h"

namespace Harmony {
	template<typename SystemType>
	inline void SystemsRegistry::Create()
	{
		std::unique_ptr<ISystem> system = HARMONY_CREATE_SYSTEM(SystemType)
		
		if (m_systems.contains(system->GetRole()))
			return HARMONY_ERROR("System with role {} already exist");

		m_systems[system->GetRole()] = std::move(system);
	}

	template<typename SystemType>
	inline void SystemsRegistry::Delete()
	{
		if (m_systems.contains(GetRole<SystemType>()))
			return HARMONY_ERROR("System with role {} do not exist");

		m_systems.erase(GetRole<SystemType>());
	}

	template<typename SystemType>
	inline bool SystemsRegistry::Contains() const
	{
		return m_systems.contains(GetRole<SystemType>());
	}

	template<typename SystemType>
	inline std::string SystemsRegistry::GetRole()
	{
		static std::string role = HARMONY_CREATE_SYSTEM(SystemType).GetRole();
		return role;
	}
}