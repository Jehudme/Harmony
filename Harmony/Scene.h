#pragma once

namespace Harmony::Internals {
	class Engine;
}

namespace Harmony::Internals {
	using ComponentID = entt::entity;
	using EntityID = entt::entity;
	using EntityIDs = std::vector<EntityID>;
	using SceneID = unsigned int;
	using PrefabID = unsigned int;

	class Scene
	{
	public:
		Scene(const Configuration& configuration, const SceneID sceneId, Engine& engine);
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