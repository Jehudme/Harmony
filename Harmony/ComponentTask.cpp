#include "pch.h"
#include "ComponentTask.h"
#include "Engine.h"
#include "Scene.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include <Entt/entt.hpp>

namespace Harmony::Tasks
{
	// AddComponentTask implementation
	AddComponentTask::AddComponentTask(const Utilities::UUID sceneId, Scenes::EntityID entityId, 
		const std::string& componentName, const Utilities::Configuration& componentConfig) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), 
		componentName_(componentName), componentConfig_(componentConfig) {}

	void AddComponentTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("Failed to add component: Scene {} not found", sceneId_);
				return;
			}

			entt::entity entity = static_cast<entt::entity>(entityId_);
			Management::ComponentManager::createComponent(componentName_, componentConfig_, entity, *scene);
			HARMONY_INFO("Component '{}' added to entity {} in scene {}", componentName_, entityId_, sceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to add component '{}' to entity {}: {}", componentName_, entityId_, e.what());
		}
	}

	// BatchCreateEntitiesTask implementation
	BatchCreateEntitiesTask::BatchCreateEntitiesTask(const Utilities::UUID sceneId, 
		const std::vector<Utilities::Configuration>& entityConfigs) :
		Task(75, SlowMultiThreaded), sceneId_(sceneId), entityConfigs_(entityConfigs) {}

	void BatchCreateEntitiesTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to batch create entities: Scene {} not found", sceneId_);
			return;
		}

		HARMONY_INFO("Batch creating {} entities in scene {}", entityConfigs_.size(), sceneId_);
		int successCount = 0;
		int failCount = 0;

		for (const auto& entityConfig : entityConfigs_) {
			try {
				Scenes::EntityID entityId = scene->createEntity(entityConfig);
				successCount++;
				HARMONY_DEBUG("Entity {} created in scene {}", entityId, sceneId_);
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Failed to create entity: {}", e.what());
			}
		}

		HARMONY_INFO("Batch entity creation completed: {} succeeded, {} failed", successCount, failCount);
	}

	// BatchDestroyEntitiesTask implementation
	BatchDestroyEntitiesTask::BatchDestroyEntitiesTask(const Utilities::UUID sceneId, 
		const std::vector<Scenes::EntityID>& entityIds) :
		Task(75, SlowMultiThreaded), sceneId_(sceneId), entityIds_(entityIds) {}

	void BatchDestroyEntitiesTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to batch destroy entities: Scene {} not found", sceneId_);
			return;
		}

		HARMONY_INFO("Batch destroying {} entities in scene {}", entityIds_.size(), sceneId_);
		int successCount = 0;
		int failCount = 0;

		for (const auto& entityId : entityIds_) {
			try {
				scene->destroyEntity(entityId);
				successCount++;
				HARMONY_DEBUG("Entity {} destroyed in scene {}", entityId, sceneId_);
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Failed to destroy entity {}: {}", entityId, e.what());
			}
		}

		HARMONY_INFO("Batch entity destruction completed: {} succeeded, {} failed", successCount, failCount);
	}
}
