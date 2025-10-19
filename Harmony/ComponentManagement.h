#pragma once

#include "Scene.h"
#include "entt/entt.hpp"
#include "Logger.h"

namespace Harmony::Errors 
{
	class ComponentError : public std::runtime_error {
	public:
		explicit ComponentError(const std::string& msg);
	};
}

namespace Harmony::Management
{
	class ComponentManager
	{
	public:
		ComponentManager(Engine& engine);
		~ComponentManager();

	public:
		template<typename Base, typename Type>
		static void registerComponent(const std::string& name);
		static void create(const std::string& name, const Utilities::Configuration& configuation, const entt::entity entityId, Scenes::Scene & scene);

	private:
		Engine& engine_;
		static inline std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, const entt::entity, Scenes::Scene&)>> componentFactories_;
	};

	template<typename Base, typename Type>
	inline void ComponentManager::registerComponent(const std::string& name) {
		componentFactories_[name] =
			[](const Utilities::Configuration& configuration,
				entt::entity entityId,
				Scenes::Scene& scene) {
					std::unique_ptr<Base> component = std::make_unique<Type>(configuration);
					scene.registry_.emplace<std::unique_ptr<Base>>(entityId, std::move(component));
			};
	}
}

// Full form: allows different base/type/name
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

// Shorthand: base == type
#define HARMONY_REGISTER_COMPONENT(ComponentType, ComponentName) \
    HARMONY_REGISTER_COMPONENT_FULL(ComponentType, ComponentType, ComponentName)