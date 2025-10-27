#pragma once
#include "Task.h"
#include "Configuration.h"

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
		DestroyEntityTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
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

	// Task to delete a scene from the current state
	class DeleteSceneTask : public Tasks::Task
	{
	public:
		DeleteSceneTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to create a global View component in a scene
	class CreateSceneViewTask : public Tasks::Task
	{
	public:
		CreateSceneViewTask(const Utilities::UUID sceneId, const Utilities::Configuration& viewConfig);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const Utilities::Configuration viewConfig_;
	};

	// Task to delete a global View component from a scene
	class DeleteSceneViewTask : public Tasks::Task
	{
	public:
		DeleteSceneViewTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to set scene draw order
	class SetSceneDrawOrderTask : public Tasks::Task
	{
	public:
		SetSceneDrawOrderTask(const Utilities::UUID sceneId, int drawOrder);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const int drawOrder_;
	};

	// Task to check if scene exists
	class CheckSceneExistsTask : public Tasks::Task
	{
	public:
		CheckSceneExistsTask(const Utilities::UUID sceneId, 
			std::function<void(bool)> callback);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		std::function<void(bool)> callback_;
	};
}

