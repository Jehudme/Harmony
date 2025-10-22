#pragma once
#include "Task.h"
#include "Scene.h"

namespace Harmony::Tasks
{
	// Task to add a component to an entity
	class AddComponentTask : public Tasks::Task
	{
	public:
		AddComponentTask(const Utilities::UUID sceneId, Scenes::EntityID entityId, 
			const std::string& componentName, const Utilities::Configuration& componentConfig);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const Scenes::EntityID entityId_;
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
			const std::vector<Scenes::EntityID>& entityIds);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const std::vector<Scenes::EntityID> entityIds_;
	};
}
