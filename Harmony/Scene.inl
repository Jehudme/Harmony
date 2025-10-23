// Scene.inl - Template implementations for Scene class
// This file is included at the end of Scene.h
// It contains entt-specific code that needs to be in a header for templates

#pragma once

#include <Entt/entt.hpp>
#include "Exceptions.h"

namespace Harmony::Scenes
{
	// Helper function for component access
	template<typename Type, typename Registry>
	static Type& getComponentReferenceImpl(Registry& registry, EntityID entityId) {
		// Cast EntityID to entt::entity for internal use
		entt::entity entity = static_cast<entt::entity>(entityId);
		if (auto* component = registry.template try_get<std::unique_ptr<Type>>(entity)) {
			return *component->get();
		}
		else {
			throw Exceptions::ComponentNotFoundException(entityId);
		}
	}

	// Scene member template implementations
	template<typename Type>
	inline Type& Scene::componentReference(EntityID entityId) {
		// Forward to implementation - will be defined in Scene.cpp
		extern entt::registry& getRegistryFromScene(Scene& scene);
		return getComponentReferenceImpl<Type>(getRegistryFromScene(*this), entityId);
	}

	template<typename Type>
	inline Type& Scene::componentReference(EntityID entityId) const {
		extern const entt::registry& getRegistryFromScene(const Scene& scene);
		return getComponentReferenceImpl<Type>(getRegistryFromScene(*this), entityId);
	}

	// Global component management template implementations
	template<typename Type, typename... Args>
	inline Type& Scene::createGlobalComponent(Args&&... args) {
		extern entt::registry& getRegistryFromScene(Scene& scene);
		return getRegistryFromScene(*this).ctx().emplace<Type>(std::forward<Args>(args)...);
	}

	template<typename Type>
	inline void Scene::deleteGlobalComponent() {
		extern entt::registry& getRegistryFromScene(Scene& scene);
		getRegistryFromScene(*this).ctx().erase<Type>();
	}

	template<typename Type>
	inline Type* Scene::getGlobalComponent() {
		extern entt::registry& getRegistryFromScene(Scene& scene);
		return getRegistryFromScene(*this).ctx().find<Type>();
	}

	template<typename Type>
	inline const Type* Scene::getGlobalComponent() const {
		extern const entt::registry& getRegistryFromScene(const Scene& scene);
		return getRegistryFromScene(*this).ctx().find<Type>();
	}
}
