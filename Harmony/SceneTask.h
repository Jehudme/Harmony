#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	// Task to create a new entity in a scene
	class CreateEntityTask : public Tasks::Task
	{
	public:
		CreateEntityTask(const Utilities::UUID sceneId, const Utilities::Configuration& entityConfig);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const Utilities::Configuration entityConfig_;
	};

	// Task to destroy an entity in a scene
	class DestroyEntityTask : public Tasks::Task
	{
	public:
		DestroyEntityTask(const Utilities::UUID sceneId, Scenes::EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const Scenes::EntityID entityId_;
	};

	// Task to create a new scene
	class CreateSceneTask : public Tasks::Task
	{
	public:
		CreateSceneTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};
}

