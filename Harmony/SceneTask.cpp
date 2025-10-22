#include "pch.h"
#include "SceneTask.h"
#include "Engine.h"
#include "SceneManagement.h"
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
	CreateSceneTask::CreateSceneTask(const Utilities::UUID sceneId) :
		Task(100, SlowMultiThreaded), sceneId_(sceneId) {}

	void CreateSceneTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->create(sceneId_);
			HARMONY_INFO("Scene {} created successfully", sceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create scene {}: {}", sceneId_, e.what());
		}
	}
}
