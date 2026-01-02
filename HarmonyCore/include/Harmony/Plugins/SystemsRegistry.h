#include "Harmony/Interfaces/ISystem.h"
#include "Harmony/Plugins/PluginsFactoriesRegistry.h"

namespace Harmony {
	class SystemsRegistry {

	public:
		SystemsRegistry() = default;
		~SystemsRegistry() = default;

		void Create(const std::string name);

		void Delete(const std::string role);

		ISystem* Get(const std::string role);

		bool Contains(const std::string role) const;



	private:
		std::unordered_map<std::string, std::unique_ptr<ISystem>> m_systems;
	};
}

#define HARMONY_REGISTER_SYSTEM(Concrete, Name) \
	HARMONY_REGISTER_PLUGIN(Harmony::ISystem, Concrete, Name)

#define HARMONY_OVERRIDE_SYSTEM(Concrete, Name) \
	HARMONY_OVERRIDE_PLUGIN(Harmony::ISystem, Concrete, Name)

#define HARMONY_CREATE_SYSTEM(Name) \
	HARMONY_CREATE_PLUGIN(Harmony::ISystem, Name)