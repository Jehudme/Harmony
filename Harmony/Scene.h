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

namespace Harmony::Scenes {
	class State;
	class Scene;
}


namespace Harmony::Scenes
{
	entt::registry& getRegistryFromScene(Scene& scene);
	const entt::registry& getRegistryFromScene(const Scene& scene);

	// Type alias to allow changing ECS implementation later
	using EntityID = unsigned int;
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
		
		/// @param deltaTime - time since last frame in seconds
		void update(float deltaTime);

	public:
		Engine& engine;
		const Utilities::UUID sceneId;
		const int drawOrder;

		template<typename Type>
		Type& componentReference(EntityID entityId);

		template<typename Type>
		Type& componentReference(EntityID entityId) const;

		EntityID createEntity(const Utilities::Configuration& configuration);
		void destroyEntity(EntityID entityId);

		// Scene control functions
		void enableDrawing();
		void disableDrawing();
		bool isDrawingEnabled() const noexcept;

		void enableUpdating();
		void disableUpdating();
		bool isUpdatingEnabled() const noexcept;

		void reset();
		void initialize();

	private:
		// PImpl to hide entt::registry details
		struct SceneImpl;
		std::unique_ptr<SceneImpl> impl_;
		const Utilities::Configuration& configuration_;

		// Scene control flags
		std::atomic<bool> drawingEnabled_{ true };
		std::atomic<bool> updatingEnabled_{ true };
		
		// Mutex for thread-safe entity operations
		mutable std::mutex entityMutex_;
	};
}

// Include template implementations
#include "Scene.inl"