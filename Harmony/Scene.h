#pragma once

#include <memory>

#include "Configuration.h"
#include "Logger.h"
#include "Exceptions.h"

// Forward declare entt types - hide entt header from users
namespace entt {
	class registry;
	enum class entity : unsigned int;
}

namespace Harmony::Scenes
{
	// Type alias to allow changing ECS implementation later
	using EntityID = unsigned int;

	class Scene : public std::enable_shared_from_this<Scene> 
	{
	public:
		friend class Management::ComponentManager;
		Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine);
		~Scene();

		// Hide SFML drawing - use internal method
		void internalDraw(void* renderTarget) const;
		
		/// @param deltaTime - time since last frame in seconds
		void update(float deltaTime);

	public:
		Engine& engine;
		const Utilities::UUID sceneId;

		template<typename Type>
		Type& componentReference(EntityID entityId);

		template<typename Type>
		Type& componentReference(EntityID entityId) const;

		EntityID createEntity(const Utilities::Configuration& configuration);
		void destroyEntity(EntityID entityId);

	private:
		// PImpl to hide entt::registry details
		struct SceneImpl;
		std::unique_ptr<SceneImpl> impl_;
		const Utilities::Configuration& configuration_;
	};
}

// Include template implementations
#include "Scene.inl"