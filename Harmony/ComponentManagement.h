#pragma once

#include "Scene.h"

namespace Harmony::Management
{
	class ComponentManager
	{
	public:
		template<typename Base, typename Type>
		static void registerComponent(const std::string& name);
		static void createComponent(const std::string& name, const Utilities::Configuration& configuation, const entt::entity entityId, Scenes::Scene & scene);

	private:
		static inline std::shared_mutex mutex_;
		static inline std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, const entt::entity, Scenes::Scene&)>> componentFactories_;
	};

	template<typename Base, typename Type>
	inline void ComponentManager::registerComponent(const std::string& name) {

		componentFactories_[name] = [](const Utilities::Configuration& configuration, entt::entity entityId, Scenes::Scene& scene) 
		{
			std::unique_ptr<Base> component = std::make_unique<Type>(configuration);

			std::lock_guard<std::shared_mutex> lock(mutex_);
			scene.registry_.emplace<std::unique_ptr<Base>>(entityId, std::move(component));
		};
	}
}

namespace Harmony::Exceptions 
{
	struct ComponentNotRegistered : std::runtime_error {
		explicit ComponentNotRegistered(const std::string& componentName);
	};
}

// Automatic component registration macro
#define HARMONY_REGISTER_COMPONENT_FULL(ComponentBase, ComponentType, ComponentName)								\
namespace Harmony::Components::Registrations {																		\
    struct ComponentName##Registration {																			\
        ComponentName##Registration() {																				\
			HARMONY_INFO("Registering component: {}", #ComponentName);												\
            Harmony::Management::ComponentManager::registerComponent<ComponentBase, ComponentType>(#ComponentName); \
        }																											\
    };																												\
    static ComponentName##Registration _autoRegister##ComponentName;												\
}

// Simplified automatic component registration macro
#define HARMONY_REGISTER_COMPONENT(ComponentType, ComponentName)													\
    HARMONY_REGISTER_COMPONENT_FULL(ComponentType, ComponentType, ComponentName)