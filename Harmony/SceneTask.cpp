#include "pch.h"
#include "SceneTask.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "StateManagement.h"
#include "Scene.h"
#include "Logger.h"

namespace Harmony::Tasks
{
	// CreateEntityTask implementation
	CreateEntityTask::CreateEntityTask(const Utilities::UUID sceneId, const Utilities::Configuration& entityConfig) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), entityConfig_(entityConfig) {}

	void CreateEntityTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to create entity: Scene {} not found", sceneId_);
			return;
		}
		
		Scenes::EntityID entityId = scene->createEntity(entityConfig_);
		HARMONY_INFO("Entity {} created in scene {}", entityId, sceneId_);
	}

	// DestroyEntityTask implementation
	DestroyEntityTask::DestroyEntityTask(const Utilities::UUID sceneId, Scenes::EntityID entityId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void DestroyEntityTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to destroy entity: Scene {} not found", sceneId_);
			return;
		}
		
		scene->destroyEntity(entityId_);
		HARMONY_INFO("Entity {} destroyed in scene {}", entityId_, sceneId_);
	}

	// CreateSceneTask implementation
	CreateSceneTask::CreateSceneTask(const Utilities::UUID sceneId, int drawOrder) :
		Task(100, SlowMultiThreaded), sceneId_(sceneId), drawOrder_(drawOrder) {}

	void CreateSceneTask::run()
	{
		try {
			// Create the scene with the specified draw order
			auto scene = getEngine().sceneManagement->create(sceneId_, drawOrder_);
			
			// Add the scene to the current state
			auto currentState = getEngine().stateManagement->getCurrentState();
			if (currentState) {
				currentState->addScene(sceneId_, scene);
				HARMONY_INFO("Scene {} created and added to current state with draw order {}", sceneId_, drawOrder_);
			} else {
				HARMONY_WARN("Scene {} created but no current state to add it to", sceneId_);
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create scene {}: {}", sceneId_, e.what());
		}
	}

	// ResetSceneTask implementation
	ResetSceneTask::ResetSceneTask(const Utilities::UUID sceneId) :
		Task(75, FastMultiThreaded), sceneId_(sceneId) {}

	void ResetSceneTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to reset scene: Scene {} not found", sceneId_);
			return;
		}

		scene->reset();
		HARMONY_INFO("Scene {} reset successfully", sceneId_);
	}

	// EnableSceneDrawingTask implementation
	EnableSceneDrawingTask::EnableSceneDrawingTask(const Utilities::UUID sceneId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId) {}

	void EnableSceneDrawingTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to enable drawing: Scene {} not found", sceneId_);
			return;
		}

		scene->enableDrawing();
		HARMONY_INFO("Scene {} drawing enabled", sceneId_);
	}

	// DisableSceneDrawingTask implementation
	DisableSceneDrawingTask::DisableSceneDrawingTask(const Utilities::UUID sceneId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId) {}

	void DisableSceneDrawingTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to disable drawing: Scene {} not found", sceneId_);
			return;
		}

		scene->disableDrawing();
		HARMONY_INFO("Scene {} drawing disabled", sceneId_);
	}

	// EnableSceneUpdatingTask implementation
	EnableSceneUpdatingTask::EnableSceneUpdatingTask(const Utilities::UUID sceneId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId) {}

	void EnableSceneUpdatingTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to enable updating: Scene {} not found", sceneId_);
			return;
		}

		scene->enableUpdating();
		HARMONY_INFO("Scene {} updating enabled", sceneId_);
	}

	// DisableSceneUpdatingTask implementation
	DisableSceneUpdatingTask::DisableSceneUpdatingTask(const Utilities::UUID sceneId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId) {}

	void DisableSceneUpdatingTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to disable updating: Scene {} not found", sceneId_);
			return;
		}

		scene->disableUpdating();
		HARMONY_INFO("Scene {} updating disabled", sceneId_);
	}

	// RemoveSceneFromStateTask implementation
	RemoveSceneFromStateTask::RemoveSceneFromStateTask(const Utilities::UUID sceneId) :
		Task(75, FastMultiThreaded), sceneId_(sceneId) {}

	void RemoveSceneFromStateTask::run()
	{
		auto currentState = getEngine().stateManagement->getCurrentState();
		if (!currentState) {
			HARMONY_ERROR("Failed to remove scene: No current state");
			return;
		}

		currentState->removeScene(sceneId_);
		HARMONY_INFO("Scene {} removed from current state", sceneId_);
	}

	// RemoveSceneByOrderTask implementation
	RemoveSceneByOrderTask::RemoveSceneByOrderTask(int drawOrder) :
		Task(75, FastMultiThreaded), drawOrder_(drawOrder) {}

	void RemoveSceneByOrderTask::run()
	{
		auto currentState = getEngine().stateManagement->getCurrentState();
		if (!currentState) {
			HARMONY_ERROR("Failed to remove scene: No current state");
			return;
		}

		currentState->removeSceneByOrder(drawOrder_);
		HARMONY_INFO("Scene with draw order {} removed from current state", drawOrder_);
	}
}
