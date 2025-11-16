#pragma once

#include <memory>
#include <entt/entity/fwd.hpp>
#include <SFML/Graphics/Drawable.hpp>

// Forward declare to avoid including Configuration.h
namespace Harmony::Utilities {
	class Configuration;
	using UUID = std::uint64_t;
}

// Forward declare Engine
namespace Harmony {
	class Engine;
}

namespace Harmony::Management {
	class ComponentManager;
}

namespace Harmony::Components {
	class View;
}

namespace Harmony::Scenes {
	class State;
	class Scene;
}


namespace Harmony::Scenes
{
	entt::registry& getRegistryFromScene(Scene& scene);
	const entt::registry& getRegistryFromScene(const Scene& scene);

	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		friend class Management::ComponentManager;
		friend class Harmony::Scenes::State;
		friend entt::registry& Harmony::Scenes::getRegistryFromScene(Scene& scene);
		friend const entt::registry& Harmony::Scenes::getRegistryFromScene(const Scene& scene);
		Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine);
		~Scene();

	private:
		// Hide SFML drawing - use internal method
		void internalDraw(sf::RenderTarget& renderTarget) const;
		
		/// @brief Update the scene state
		/// @param deltaTime - time since last frame in seconds
		/// 
		/// This method performs the following steps:
		/// 1. Calls onPreUpdate() for all Script components
		/// 2. Synchronizes Transform data to PhysicsBody (if PhysicsWorld exists)
		/// 3. Steps the PhysicsWorld simulation
		/// 4. Synchronizes PhysicsBody data back to Transform
		/// 5. Calls onPostUpdate() for all Script components
		void update(float deltaTime);

	public:
		Engine& engine;
		const Utilities::UUID sceneId;
		const int drawOrder;

		template<typename Type>
		Type& getComponent(EntityID entityId);
		template<typename Type>
		Type& getComponent(EntityID entityId) const;

		template<typename Type, typename... Args>
		Type& createComponent(EntityID entityId, Args&&... args);
		template<typename Base, typename Type, typename... Args>
		Type& createComponent(EntityID entityId, Args&&... args);
		template<typename Type>
		void deleteComponent(EntityID entityId);

		void createComponent(const std::string& componentName, const Utilities::Configuration& configuration, EntityID entityId);
		void deleteComponent(const std::string& componentName, EntityID entityId);

		template<typename Type>
		Type& getGlobalComponent();
		template<typename Type>
		const Type& getGlobalComponent() const;
		
		template<typename Type, typename... Args>
		Type& createGlobalComponent(Args&&... args);
		template<typename Base, typename Type, typename... Args>
		Type& createGlobalComponent(Args&&... args);
		template<typename Type>
		void deleteGlobalComponent();
		
		void createGlobalComponent(const std::string& componentName, const Utilities::Configuration& configuration);
		void deleteGlobalComponent(const std::string& componentName);

		EntityID createEntity(const Utilities::Configuration& configuration);
		EntityID createEntity(const Utilities::Configuration& configuration, Utilities::UUID premadeId);

		void destroyEntity(EntityID entityId);

		// View management (singleton per scene)
		Components::View& getView();
		const Components::View& getView() const;
		void setView(const Utilities::Configuration& configuration);

		// Scene control functions
		void enableDrawing();
		void disableDrawing();
		bool isDrawingEnabled() const noexcept;

		void enableUpdating();
		void disableUpdating();
		bool isUpdatingEnabled() const noexcept;

		void reset();
		void initialize();

		struct EntityCounter;
		std::unique_ptr<EntityCounter> entityCounter;

	private:
		struct SceneImpl;
		std::unique_ptr<SceneImpl> impl_;

		const Utilities::Configuration& configuration_;

		// Scene control flags
		std::atomic<bool> drawingEnabled_{ true };
		std::atomic<bool> updatingEnabled_{ true };
		
		// Mutex for thread-safe entity operations
		mutable std::mutex entityMutex_;

		const entt::connection onConstructConnection_;
		const entt::connection onDestroyConnection_;
	};

	struct Scene::EntityCounter {
	public:
		friend class Scene;
		std::size_t getCount() const;

	private:
		void increment();
		void decrement();

		std::size_t count;
	};
}

// Include template implementations
#include "Scene.inl"