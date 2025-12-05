namespace Harmony::Internals {
    template<typename Base, typename Type>
    inline void ComponentsHandler::registerComponent(const std::string& name) {
        static_assert(std::is_constructible_v<Type, const Harmony::Internals::EntityID, const Configuration&, Scene&>,
            "Type must have a constructor taking const Harmony::Configuration& and Harmony::Scene&"
            );

        // Thread-safe write access when registering component
        std::lock_guard<std::mutex> lock(getMutex());

        getComponentConstructorFactories()[name] =
            [](const Configuration& configuration, EntityID entityId, Scene& scene)
            {
                if (entityId == entt::null) scene.createGlobalComponent<Base, Type>(configuration, scene);
                else scene.createComponent<Base, Type>(entityId, configuration, scene);
            };

        getComponentDestructorFactories()[name] =
            [](EntityID entityId, Scene& scene)
            {
                if (entityId == entt::null) scene.deleteGlobalComponent<Type>();
                else scene.deleteComponent<Type>(entityId);
            };

        getComponentContainsFactories()[name] =
            [](EntityID entityId, const Scene& scene) -> bool
            {
                if (entityId == entt::null) return scene.containsGlobalComponent<Type>();
                else return scene.containsComponent<Type>(entityId);
			};
    }
}
