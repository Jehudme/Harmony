#pragma once
#include "Task.h"
#include "Scene.h"
#include "Configuration.h"

namespace Harmony::Tasks
{
	// Task to add a component to an entity
	class AddComponentTask : public Tasks::Task
	{
	public:
		AddComponentTask(const Utilities::UUID sceneId, EntityID entityId, 
			const std::string& componentName, const Utilities::Configuration& componentConfig);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const std::string componentName_;
		const Utilities::Configuration componentConfig_;
	};

	// Task to perform batch entity operations
	class BatchCreateEntitiesTask : public Tasks::Task
	{
	public:
		BatchCreateEntitiesTask(const Utilities::UUID sceneId, 
			const std::vector<Utilities::Configuration>& entityConfigs);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const std::vector<Utilities::Configuration> entityConfigs_;
	};

	// Task to perform batch entity destruction
	class BatchDestroyEntitiesTask : public Tasks::Task
	{
	public:
		BatchDestroyEntitiesTask(const Utilities::UUID sceneId, 
			const std::vector<EntityID>& entityIds);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const std::vector<EntityID> entityIds_;
	};

	// Task to remove a component from an entity
	class RemoveComponentTask : public Tasks::Task
	{
	public:
		RemoveComponentTask(const Utilities::UUID sceneId, EntityID entityId, 
			const std::string& componentName);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const std::string componentName_;
	};

	// Task to clone an entity with all its components
	class CloneEntityTask : public Tasks::Task
	{
	public:
		CloneEntityTask(const Utilities::UUID sceneId, EntityID sourceEntityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID sourceEntityId_;
	};

	// Task to add the same component to multiple entities
	class BatchAddComponentsTask : public Tasks::Task
	{
	public:
		BatchAddComponentsTask(const Utilities::UUID sceneId, 
			const std::vector<EntityID>& entityIds,
			const std::string& componentName, 
			const Utilities::Configuration& componentConfig);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const std::vector<EntityID> entityIds_;
		const std::string componentName_;
		const Utilities::Configuration componentConfig_;
	};

	// Task to remove the same component from multiple entities
	class BatchRemoveComponentsTask : public Tasks::Task
	{
	public:
		BatchRemoveComponentsTask(const Utilities::UUID sceneId, 
			const std::vector<EntityID>& entityIds,
			const std::string& componentName);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const std::vector<EntityID> entityIds_;
		const std::string componentName_;
	};

	// Task to count entities in a scene
	class CountEntitiesTask : public Tasks::Task
	{
	public:
		CountEntitiesTask(const Utilities::UUID sceneId, 
			std::function<void(size_t)> callback);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		std::function<void(size_t)> callback_;
	};

	// Task to clear all entities from a scene
	class ClearSceneEntitiesTask : public Tasks::Task
	{
	public:
		ClearSceneEntitiesTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to move entity from one scene to another
	class MoveEntityTask : public Tasks::Task
	{
	public:
		MoveEntityTask(const Utilities::UUID sourceSceneId, 
			const Utilities::UUID targetSceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sourceSceneId_;
		const Utilities::UUID targetSceneId_;
		const EntityID entityId_;
	};

	// Task to batch move entities between scenes
	class BatchMoveEntitiesTask : public Tasks::Task
	{
	public:
		BatchMoveEntitiesTask(const Utilities::UUID sourceSceneId, 
			const Utilities::UUID targetSceneId, 
			const std::vector<EntityID>& entityIds);

	private:
		void run() override;

	private:
		const Utilities::UUID sourceSceneId_;
		const Utilities::UUID targetSceneId_;
		const std::vector<EntityID> entityIds_;
	};
}
