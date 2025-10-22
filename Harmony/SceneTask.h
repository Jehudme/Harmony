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
		CreateSceneTask(const Utilities::UUID sceneId, int drawOrder = 0);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const int drawOrder_;
	};

	// Task to reset a scene
	class ResetSceneTask : public Tasks::Task
	{
	public:
		ResetSceneTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to enable scene drawing
	class EnableSceneDrawingTask : public Tasks::Task
	{
	public:
		EnableSceneDrawingTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to disable scene drawing
	class DisableSceneDrawingTask : public Tasks::Task
	{
	public:
		DisableSceneDrawingTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to enable scene updating
	class EnableSceneUpdatingTask : public Tasks::Task
	{
	public:
		EnableSceneUpdatingTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to disable scene updating
	class DisableSceneUpdatingTask : public Tasks::Task
	{
	public:
		DisableSceneUpdatingTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to remove a scene from the current state
	class RemoveSceneFromStateTask : public Tasks::Task
	{
	public:
		RemoveSceneFromStateTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to remove a scene from the current state by draw order
	class RemoveSceneByOrderTask : public Tasks::Task
	{
	public:
		RemoveSceneByOrderTask(int drawOrder);

	private:
		void run() override;

	private:
		const int drawOrder_;
	};
}

