#pragma once

#include "Scene.h"
#include "Exceptions.h"
#include <Entt/entt.hpp>

namespace Harmony::Management
{
	class ComponentManager
	{
	public:
		template<typename Base, typename Type>
		static void registerComponent(const std::string& name);
		static void createComponent(const std::string& name, const Utilities::Configuration& configuation, entt::entity entityId, Scenes::Scene & scene);

	private:
		static inline std::shared_mutex mutex_;
		static inline std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, entt::entity, Scenes::Scene&)>> componentFactories_;
	};

	// Helper to get registry from scene - defined in Scene.cpp
	extern entt::registry& getRegistryFromScene(Scenes::Scene& scene);

	template<typename Base, typename Type>
	inline void ComponentManager::registerComponent(const std::string& name) {

		componentFactories_[name] = [](const Utilities::Configuration& configuration, entt::entity entityId, Scenes::Scene& scene) 
		{
			std::unique_ptr<Base> component = std::make_unique<Type>(configuration);
			std::lock_guard<std::shared_mutex> lock(mutex_);
			// Use helper function instead of direct access
			getRegistryFromScene(scene).emplace<std::unique_ptr<Base>>(entityId, std::move(component));
		};
	}
}

#define HARMONY_REGISTER_COMPONENT_WITH_BASE(ComponentBase, ComponentType, ComponentName)							\
namespace Harmony::Components::Registrations::detail {																\
    struct ComponentName##Registration {																			\
        ComponentName##Registration() {																				\
            Harmony::Management::ComponentManager::registerComponent<ComponentBase, ComponentType>(#ComponentName); \
        }																											\
    };																												\
    inline ComponentName##Registration _autoRegister##ComponentName{};												\
}

// Simplified automatic component registration macro
#define HARMONY_REGISTER_COMPONENT(ComponentType, ComponentName)													\
    HARMONY_REGISTER_COMPONENT_WITH_BASE(ComponentType, ComponentType, ComponentName)
