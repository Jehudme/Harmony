#pragma once

#include <Entt/entt.hpp>
#include "Exceptions.h"
#include "Scene.h"
#include "type_name.h"
#include "Assert.h"
#include <mutex>

namespace Harmony::Internals
{
	template<typename Type, typename Registry>
	static Type& getComponentReferenceImpl(Registry& registry, EntityID entityId) {
		auto* component = registry.try_get<std::unique_ptr<Type>>(static_cast<entt::entity>(entityId));
		HARMONY_ASSERT_NOT_NULL(component != nullptr, "Component not found on entity");
		HARMONY_ASSERT_NOT_NULL(component->get() != nullptr, "Component pointer is null");
		return *component->get();
	}

	template<typename Type>
	inline Type& Scene::getComponent(EntityID entityId) {
		std::lock_guard<std::mutex> lock(registryMutex_);
		return getComponentReferenceImpl<Type>(registry_, entityId);
	}

	template<typename Type>
	inline Type& Scene::getComponent(EntityID entityId) const {
		std::lock_guard<std::mutex> lock(registryMutex_);
		return getComponentReferenceImpl<Type>(registry_, entityId);
	}

	template<typename Type, typename ...Args>
	inline Type& Scene::createComponent(EntityID entityId, Args && ...args) {
		return createComponent<Type, Type>(entityId, std::forward<Args>(args)...);
	}

	template<typename Base, typename Type, typename ...Args>
	inline Type& Scene::createComponent(EntityID entityId, Args&&... args)
	{
		HARMONY_ASSERT(entityId != entt::null, "Cannot create component on null entity");
		
		std::lock_guard<std::mutex> lock(registryMutex_);
		
		try {
			auto& ptr = registry_.emplace_or_replace<std::unique_ptr<Base>>(
				static_cast<entt::entity>(entityId), 
				std::make_unique<Type>(entityId, std::forward<Args>(args)...)
			);
			
			HARMONY_TRACE("Component of type '{}' created for entity {}", typeid(Type).name(), entityId);
			return static_cast<Type&>(*ptr);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create component of type '{}' for entity {}: {}", 
				typeid(Type).name(), entityId, e.what());
			throw;
		}
	}

	template<typename Type>
	inline void Scene::deleteComponent(EntityID entityId) {
		HARMONY_ASSERT(entityId != entt::null, "Cannot delete component from null entity");
		
		std::lock_guard<std::mutex> lock(registryMutex_);
		
		if (!registry_.all_of<std::unique_ptr<Type>>(static_cast<entt::entity>(entityId))) {
			HARMONY_WARN("Attempted to delete non-existent component of type '{}' from entity {}", 
				typeid(Type).name(), entityId);
			return;
		}
		
		registry_.remove<std::unique_ptr<Type>>(static_cast<entt::entity>(entityId));
		HARMONY_TRACE("Component of type '{}' deleted from entity {}", typeid(Type).name(), entityId);
	}

	template<typename Type, typename... Args>
	inline Type& Scene::createGlobalComponent(Args&&... args) {
		return createGlobalComponent<Type, Type>(std::forward<Args>(args)...);
	}

	template<typename Base, typename Type, typename... Args>
	inline Type& Scene::createGlobalComponent(Args&&... args) {
		std::lock_guard<std::mutex> lock(registryMutex_);
		
		try {
			auto& ptr = registry_
				.ctx()
				.emplace<std::unique_ptr<Base>>(
					std::make_unique<Type>(static_cast<EntityID>(entt::null), std::forward<Args>(args)...)
				);

			HARMONY_TRACE("Global component of type '{}' created", typeid(Type).name());
			return static_cast<Type&>(*ptr);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create global component of type '{}': {}", 
				typeid(Type).name(), e.what());
			throw;
		}
	}

	template<typename Type>
	inline void Scene::deleteGlobalComponent() {
		std::lock_guard<std::mutex> lock(registryMutex_);
		
		if (!registry_.ctx().contains<std::unique_ptr<Type>>()) {
			HARMONY_WARN("Attempted to delete non-existent global component of type '{}'", 
				typeid(Type).name());
			return;
		}
		
		registry_.ctx().erase<std::unique_ptr<Type>>();
		HARMONY_TRACE("Global component of type '{}' deleted", typeid(Type).name());
	}

	template<typename Type>
	inline auto Scene::getComponentsView(){
		std::lock_guard<std::mutex> lock(registryMutex_);
		return registry_.view<std::unique_ptr<Type>>();
	}

	template<typename Type>
	inline bool Scene::containsComponent(EntityID entityId) const {
		std::lock_guard<std::mutex> lock(registryMutex_);
		return registry_.all_of<std::unique_ptr<Type>>(static_cast<entt::entity>(entityId));
	}

	template<typename Type>
	inline bool Scene::containsGlobalComponent() const {
		std::lock_guard<std::mutex> lock(registryMutex_);
		return registry_.ctx().contains<std::unique_ptr<Type>>();
	}

	template<typename Type>
	inline Type& Scene::getGlobalComponent() {
		std::lock_guard<std::mutex> lock(registryMutex_);
		
		auto* ptr = registry_.ctx().find<std::unique_ptr<Type>>();
		HARMONY_ASSERT_NOT_NULL(ptr != nullptr, "Global component not found");
		HARMONY_ASSERT_NOT_NULL(ptr->get() != nullptr, "Global component pointer is null");
		
		return **ptr;
	}

	template<typename Type>
	inline const Type& Scene::getGlobalComponent() const {
		std::lock_guard<std::mutex> lock(registryMutex_);
		
		auto* ptr = registry_.ctx().find<std::unique_ptr<Type>>();
		HARMONY_ASSERT_NOT_NULL(ptr != nullptr, "Global component not found");
		HARMONY_ASSERT_NOT_NULL(ptr->get() != nullptr, "Global component pointer is null");
		
		return **ptr;
	}
}