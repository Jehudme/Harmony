
#pragma once

#include "Scene.h"
#include "Exceptions.h"
#include "Configuration.h"
#include <Entt/entt.hpp>
#include <shared_mutex>
#include <type_traits>

namespace Harmony::Internals {
    class Engine;
    class Scene;
}

namespace Harmony::Internals
{
    class ComponentsHandler
    {
    public:
        ComponentsHandler(Engine& engine);
        ~ComponentsHandler();

        template<typename Base, typename Type>
        static void registerComponent(const std::string& name);
        static void createComponent(const std::string& name, const Configuration& configuation, EntityID entityId, Scene& scene);
        static void deleteComponent(const std::string& name, EntityID entityId, Scene& scene);

    private:
        static std::mutex& getMutex();
        static std::unordered_map<std::string, std::function<void(const Configuration&, EntityID, Scene&)>>& getComponentConstructorFactories();
        static std::unordered_map<std::string, std::function<void(EntityID, Scene& scene)>>& getComponentDestructorFactories();

        Engine& engine_;
    };

    template<typename Base, typename Type>
    inline void ComponentsHandler::registerComponent(const std::string& name) {
        //static_assert(std::is_constructible_v<Type, const Configuration&, Scene&>,
        //    "Type must have a constructor taking const Harmony::Configuration& and Harmony::Scene&"
        //    );

        getComponentConstructorFactories()[name] =
            [](const Configuration& configuration, EntityID entityId, Scene& scene)
            {
                std::unique_ptr<Base> component = std::make_unique<Type>(configuration, scene);

                if (entityId == entt::null) scene.createGlobalComponent<Type>(configuration, scene);
                else scene.createComponent<Base, Type>(entityId, configuration, scene);
            };

        getComponentDestructorFactories()[name] =
            [](EntityID entityId, Scene& scene)
            {
                if (entityId == entt::null) scene.deleteGlobalComponent<Type>();
                else scene.deleteComponent<Type>(entityId);
            };
    }
}

// Automatic component registration macro with base class
#define HARMONY_REGISTER_COMPONENT_WITH_BASE(ComponentBase, ComponentType, ComponentName)							\
namespace Harmony::Components::Registrations::details {																\
    struct ComponentName##Registration {																			\
        ComponentName##Registration() {																				\
            Harmony::Internals::ComponentsHandler::registerComponent<ComponentBase, ComponentType>(#ComponentName); \
        }																											\
    };																												\
    inline ComponentName##Registration _autoRegister##ComponentName{};												\
}

// Simplified automatic component registration macro
#define HARMONY_REGISTER_COMPONENT(ComponentType, ComponentName)													\
    HARMONY_REGISTER_COMPONENT_WITH_BASE(ComponentType, ComponentType, ComponentName)

#define HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS \
const Harmony::Configuration& configuration, Harmony::Internals::Scene& scene