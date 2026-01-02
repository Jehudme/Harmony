#include "Harmony/Interfaces/ISystem.h"
#include "Harmony/Interfaces/ISystemsRegistry.h"

#include <unordered_map>

namespace Harmony {
	class SystemsRegistry : public ISystemsRegistry {

	public:
		SystemsRegistry() = default;
		~SystemsRegistry() = default;
		
		ISystem* GetSystem(const std::string role) override;

		void CreateSystem(const std::string name) override;
		void DeleteSystem(const std::string role) override;
		bool ContainsSystem(const std::string role) const override;

	private:
		std::unordered_map<std::string, std::unique_ptr<ISystem>> m_systems;
	};
}