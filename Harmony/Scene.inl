#pragma once

#include <Entt/entt.hpp>
#include "Exceptions.h"
#include "Scene.h"
#include "type_name.h"

namespace Harmony::Internals
{
	template<typename Type, typename Registry>
	static Type& getComponentReferenceImpl(Registry& registry, EntityID entityId) {
		if (auto* component = registry.template try_get<std::unique_ptr<Type>>(entityId))
			return *component->get();
	}

	template<typename Type>
	inline Type& Scene::getComponent(EntityID entityId) {
		return getComponentReferenceImpl<Type>(registry_, entityId);
	}

	template<typename Type>
	inline Type& Scene::getComponent(EntityID entityId) const {
		return getComponentReferenceImpl<Type>(registry_, entityId);
	}

	template<typename Type, typename ...Args>
	inline Type& Scene::createComponent(EntityID entityId, Args && ...args) {
		return createComponent<Type, Type>(entityId, std::forward<Args>(args)...);
	}

	template<typename Base, typename Type, typename ...Args>
	inline Type& Scene::createComponent(EntityID entityId, Args&&... args)
	{
		auto& ptr = registry_.emplace_or_replace<std::unique_ptr<Base>>(static_cast<entt::entity>(entityId), std::make_unique<Type>(std::forward<Args>(args)...));

		return static_cast<Type&>(*ptr);
	}

	template<typename Type>
	inline void Scene::deleteComponent(EntityID entityId) {
		registry_.remove<std::unique_ptr<Type>>(static_cast<entt::entity>(entityId));
	}

	template<typename Type, typename... Args>
	inline Type& Scene::createGlobalComponent(Args&&... args) {
		return createGlobalComponent<Type, Type>(std::forward<Args>(args)...);
	}

	template<typename Base, typename Type, typename... Args>
	inline Type& Scene::createGlobalComponent(Args&&... args) {
		auto& ptr = registry_
			.ctx()
			.emplace<std::unique_ptr<Base>>(std::make_unique<Type>(std::forward<Args>(args)...));

		return static_cast<Type&>(*ptr);
	}

	template<typename Type>
	inline void Scene::deleteGlobalComponent() {
		registry_.ctx().erase<std::unique_ptr<Type>>();
	}

	template<typename Type>
	inline Type& Scene::getGlobalComponent() {
		auto* ptr = registry_.ctx().find<std::unique_ptr<Type>>();
		return **ptr;
	}

	template<typename Type>
	inline const Type& Scene::getGlobalComponent() const {
		auto* ptr = registry_.ctx().find<std::unique_ptr<Type>>();
		return **ptr;
	}
}