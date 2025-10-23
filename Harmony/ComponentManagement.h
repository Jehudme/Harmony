#pragma once

#include "Scene.h"
#include "Exceptions.h"
#include "Configuration.h"
#include <Entt/entt.hpp>
#include <shared_mutex>
#include <type_traits>

namespace Harmony::Scenes {
    class Scene;
}

namespace Harmony::Management
{
	class ComponentManager
	{
	public:
        ComponentManager(Engine& engine);
		~ComponentManager();

		template<typename Base, typename Type>
		static void registerComponent(const std::string& name);
		static void createComponent(const std::string& name, const Utilities::Configuration& configuation, entt::entity entityId, Scenes::Scene & scene);

    private:
        static std::mutex& getMutex();
        static std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, entt::entity, Scenes::Scene&)>>& getComponentFactories();

		Engine& engine_;
	};

    template<typename Base, typename Type>
    inline void ComponentManager::registerComponent(const std::string& name) {
        static_assert( std::is_constructible_v<Type, const Utilities::Configuration&, Scenes::Scene&>,
            "Type must have a constructor taking const Harmony::Utilities::Configuration& and Harmony::Scenes::Scene"
        );

        getComponentFactories()[name] =
            [](const Utilities::Configuration& configuration, entt::entity entityId, Scenes::Scene& scene)
            {
                std::unique_ptr<Base> component = std::make_unique<Type>(configuration, scene);
                getRegistryFromScene(scene).emplace<std::unique_ptr<Base>>(entityId, std::move(component));
            };
    }
}

// Automatic component registration macro with base class
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
