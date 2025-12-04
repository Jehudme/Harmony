#pragma once
#include "Configuration.h"
#include <entt/entt.hpp>
#include <vector>

namespace Harmony::Internals {
	class Engine;
}

namespace Harmony::Components {
	class Script;
}

namespace Harmony::Internals {
	using ComponentID = entt::entity;
	using EntityID = uint32_t;
	using EntityIDs = std::vector<uint32_t>;
	using SceneID = unsigned int;
	using PrefabID = unsigned int;

	class Scene
	{
		friend class Engine;

	public:
		Scene(Engine& engine,const SceneID sceneId);
		~Scene();

		void initialize();

		bool isActiveRendering() const;
		bool isActiveUpdating() const;

		void setActiveRendering(bool active);
		void setActiveUpdating(bool active);

		int getUpdateOrder() const;
		int getRenderOrder() const;

		void setUpdateOrder(int order);
		void setRenderOrder(int order);

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

		void createComponent(const std::string& componentName, const Configuration& configuration, EntityID entityId);
		void deleteComponent(const std::string& componentName, EntityID entityId);

		bool containsComponent(const std::string& componentName, EntityID entityId) const;
		bool containsGlobalComponent(const std::string& componentName) const;

		template<typename Type>
		auto getComponentsView();

		template<typename Type>
		bool containsComponent(EntityID entityId) const;

		template<typename Type>
		bool containsGlobalComponent() const;

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

		void createGlobalComponent(const std::string& componentName, const Configuration& configuration);
		void deleteGlobalComponent(const std::string& componentName);

		EntityID createEntity(const Configuration& configuration);
		EntityID createEntity(const EntityID entityId);

		void createEntities(const Configuration& configuration);
		void createEntities(const EntityIDs entities);


		void destroyEntity(EntityID entityId);

	public:
		SceneID sceneId;
		Engine& engine;

	private:
		void render();
		void update();

		void initializeComponents();
		void initializeEntities();

	private:
		Configuration configuration_;
		entt::registry registry_;

		std::atomic_bool activeRender;
		std::atomic_bool activeUpdate;

		std::atomic_int updateOrder_;
		std::atomic_int renderOrder_;
	};
}

#include "Scene.inl"