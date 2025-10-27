#include "pch.h"
#include "SceneTask.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "StateManagement.h"
#include "Scene.h"
#include "State.h"
#include "View.h"
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
		
		EntityID entityId = scene->createEntity(entityConfig_);
		HARMONY_INFO("Entity {} created in scene {}", static_cast<unsigned int>(entityId), sceneId_);
	}

	// DestroyEntityTask implementation
	DestroyEntityTask::DestroyEntityTask(const Utilities::UUID sceneId, EntityID entityId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void DestroyEntityTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to destroy entity: Scene {} not found", sceneId_);
			return;
		}
		
		scene->destroyEntity(entityId_);
		HARMONY_INFO("Entity {} destroyed in scene {}", static_cast<unsigned int>(entityId_), sceneId_);
	}

	// CreateSceneTask implementation
	CreateSceneTask::CreateSceneTask(const Utilities::UUID sceneId) :
		Task(100, SlowMultiThreaded), sceneId_(sceneId) {}

	void CreateSceneTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->create(sceneId_);
			HARMONY_INFO("Scene {} created successfully", sceneId_);
			
			// Add the scene to the current state
			auto currentState = getEngine().stateManagement->getCurrentState();
			if (currentState) {
				currentState->addScene(scene);
				HARMONY_INFO("Scene {} added to current state", sceneId_);
			}
			else {
				HARMONY_WARN("No current state available to add scene {}", sceneId_);
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

	// DeleteSceneTask implementation
	DeleteSceneTask::DeleteSceneTask(const Utilities::UUID sceneId) :
		Task(75, FastMultiThreaded), sceneId_(sceneId) {}

	void DeleteSceneTask::run()
	{
		auto currentState = getEngine().stateManagement->getCurrentState();
		if (!currentState) {
			HARMONY_ERROR("Failed to delete scene: No current state", sceneId_);
			return;
		}

		currentState->removeScene(sceneId_);
		HARMONY_INFO("Scene {} deleted from state", sceneId_);
	}

	// CreateSceneViewTask implementation
	CreateSceneViewTask::CreateSceneViewTask(const Utilities::UUID sceneId, const Utilities::Configuration& viewConfig) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), viewConfig_(viewConfig) {}

	void CreateSceneViewTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to create View: Scene {} not found", sceneId_);
			return;
		}

		scene->setView(viewConfig_);
		HARMONY_INFO("View created for scene {}", sceneId_);
	}

	// DeleteSceneViewTask implementation
	DeleteSceneViewTask::DeleteSceneViewTask(const Utilities::UUID sceneId) :
		Task(50, FastMultiThreaded), sceneId_(sceneId) {}

	void DeleteSceneViewTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to delete View: Scene {} not found", sceneId_);
			return;
		}

		scene->deleteGlobalComponent<Components::View>();
		HARMONY_INFO("View deleted from scene {}", sceneId_);
	}
}
