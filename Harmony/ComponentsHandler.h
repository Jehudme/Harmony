
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
		static bool containsComponent(const std::string& name, EntityID entityId, const Scene& scene);
        static void createComponent(const std::string& name, const Configuration& configuation, EntityID entityId, Scene& scene);
        static void deleteComponent(const std::string& name, EntityID entityId, Scene& scene);

    private:
        static std::mutex& getMutex();
        static std::unordered_map<std::string, std::function<void(const Configuration&, EntityID, Scene&)>>& getComponentConstructorFactories();
        static std::unordered_map<std::string, std::function<void(EntityID, Scene& scene)>>& getComponentDestructorFactories();
        static std::unordered_map<std::string, std::function<bool(EntityID, const Scene& scene)>>& getComponentContainsFactories();

        Engine& engine_;
    };

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

} // namespace Harmony::Internals

#include "ComponentsHandler.inl"