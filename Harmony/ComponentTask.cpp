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
	AddComponentTask::AddComponentTask(const Utilities::UUID sceneId, EntityID entityId, 
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

			EntityID entity = static_cast<EntityID>(entityId_);
			Management::ComponentManager::createComponent(componentName_, componentConfig_, entity, *scene);
			HARMONY_INFO("Component '{}' added to entity {} in scene {}", componentName_, static_cast<unsigned int>(entityId_), sceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to add component '{}' to entity {}: {}", componentName_, static_cast<unsigned int>(entityId_), e.what());
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
				EntityID entityId = scene->createEntity(entityConfig);
				successCount++;
				HARMONY_DEBUG("Entity {} created in scene {}", static_cast<unsigned int>(entityId), sceneId_);
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
		const std::vector<EntityID>& entityIds) :
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
				HARMONY_DEBUG("Entity {} destroyed in scene {}", static_cast<unsigned int>(entityId), sceneId_);
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Failed to destroy entity {}: {}", static_cast<unsigned int>(entityId), e.what());
			}
		}

		HARMONY_INFO("Batch entity destruction completed: {} succeeded, {} failed", successCount, failCount);
	}

	// RemoveComponentTask implementation
	RemoveComponentTask::RemoveComponentTask(const Utilities::UUID sceneId, EntityID entityId, 
		const std::string& componentName) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), 
		componentName_(componentName) {}

	void RemoveComponentTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("Failed to remove component: Scene {} not found", sceneId_);
				return;
			}

			Management::ComponentManager::deleteComponent(componentName_, entityId_, *scene);
			HARMONY_INFO("Component '{}' removed from entity {} in scene {}", componentName_, static_cast<unsigned int>(entityId_), sceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to remove component '{}' from entity {}: {}", componentName_, static_cast<unsigned int>(entityId_), e.what());
		}
	}

	// CloneEntityTask implementation
	CloneEntityTask::CloneEntityTask(const Utilities::UUID sceneId, EntityID sourceEntityId) :
		Task(75, FastMultiThreaded), sceneId_(sceneId), sourceEntityId_(sourceEntityId) {}

	void CloneEntityTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("Failed to clone entity: Scene {} not found", sceneId_);
				return;
			}

			// Note: This is a simplified implementation
			// A full implementation would need to copy all components
			HARMONY_WARN("CloneEntityTask: Full component cloning not yet implemented");
			HARMONY_INFO("Entity {} clone attempted in scene {}", static_cast<unsigned int>(sourceEntityId_), sceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to clone entity {}: {}", static_cast<unsigned int>(sourceEntityId_), e.what());
		}
	}

	// BatchAddComponentsTask implementation
	BatchAddComponentsTask::BatchAddComponentsTask(const Utilities::UUID sceneId, 
		const std::vector<EntityID>& entityIds,
		const std::string& componentName, 
		const Utilities::Configuration& componentConfig) :
		Task(75, SlowMultiThreaded), sceneId_(sceneId), entityIds_(entityIds), 
		componentName_(componentName), componentConfig_(componentConfig) {}

	void BatchAddComponentsTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to batch add components: Scene {} not found", sceneId_);
			return;
		}

		HARMONY_INFO("Batch adding component '{}' to {} entities in scene {}", componentName_, entityIds_.size(), sceneId_);
		int successCount = 0;
		int failCount = 0;

		for (const auto& entityId : entityIds_) {
			try {
				Management::ComponentManager::createComponent(componentName_, componentConfig_, entityId, *scene);
				successCount++;
				HARMONY_DEBUG("Component '{}' added to entity {} in scene {}", componentName_, static_cast<unsigned int>(entityId), sceneId_);
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Failed to add component '{}' to entity {}: {}", componentName_, static_cast<unsigned int>(entityId), e.what());
			}
		}

		HARMONY_INFO("Batch component addition completed: {} succeeded, {} failed", successCount, failCount);
	}

	// BatchRemoveComponentsTask implementation
	BatchRemoveComponentsTask::BatchRemoveComponentsTask(const Utilities::UUID sceneId, 
		const std::vector<EntityID>& entityIds,
		const std::string& componentName) :
		Task(75, SlowMultiThreaded), sceneId_(sceneId), entityIds_(entityIds), 
		componentName_(componentName) {}

	void BatchRemoveComponentsTask::run()
	{
		auto scene = getEngine().sceneManagement->find(sceneId_);
		if (!scene) {
			HARMONY_ERROR("Failed to batch remove components: Scene {} not found", sceneId_);
			return;
		}

		HARMONY_INFO("Batch removing component '{}' from {} entities in scene {}", componentName_, entityIds_.size(), sceneId_);
		int successCount = 0;
		int failCount = 0;

		for (const auto& entityId : entityIds_) {
			try {
				Management::ComponentManager::deleteComponent(componentName_, entityId, *scene);
				successCount++;
				HARMONY_DEBUG("Component '{}' removed from entity {} in scene {}", componentName_, static_cast<unsigned int>(entityId), sceneId_);
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Failed to remove component '{}' from entity {}: {}", componentName_, static_cast<unsigned int>(entityId), e.what());
			}
		}

		HARMONY_INFO("Batch component removal completed: {} succeeded, {} failed", successCount, failCount);
	}

	// CountEntitiesTask implementation
	CountEntitiesTask::CountEntitiesTask(const Utilities::UUID sceneId, 
		std::function<void(size_t)> callback) :
		Task(50, FastMultiThreaded), sceneId_(sceneId), callback_(callback) {}

	void CountEntitiesTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("Failed to count entities: Scene {} not found", sceneId_);
				if (callback_) callback_(0);
				return;
			}

			auto& registry = Scenes::getRegistryFromScene(*scene);
			
			HARMONY_INFO("Scene {} has {} active entities", sceneId_, scene->entityCounter->getCount());
			
			if (callback_) {
				callback_(scene->entityCounter->getCount());
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to count entities in scene {}: {}", sceneId_, e.what());
			if (callback_) callback_(0);
		}
	}

	// ClearSceneEntitiesTask implementation
	ClearSceneEntitiesTask::ClearSceneEntitiesTask(const Utilities::UUID sceneId) :
		Task(100, FastMultiThreaded), sceneId_(sceneId) {}

	void ClearSceneEntitiesTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("Failed to clear entities: Scene {} not found", sceneId_);
				return;
			}

			auto& registry = Scenes::getRegistryFromScene(*scene);
			registry.clear();
			
			HARMONY_INFO("Cleared {} entities from scene {}", scene->entityCounter->getCount(), sceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to clear entities from scene {}: {}", sceneId_, e.what());
		}
	}

	// MoveEntityTask implementation
	MoveEntityTask::MoveEntityTask(const Utilities::UUID sourceSceneId, 
		const Utilities::UUID targetSceneId, EntityID entityId) :
		Task(75, FastMultiThreaded), sourceSceneId_(sourceSceneId), 
		targetSceneId_(targetSceneId), entityId_(entityId) {}

	void MoveEntityTask::run()
	{
		try {
			// Note: This is a simplified implementation
			// A full implementation would need to move all components
			HARMONY_WARN("MoveEntityTask: Full entity moving not yet implemented");
			HARMONY_INFO("Entity {} move attempted from scene {} to scene {}", 
				static_cast<unsigned int>(entityId_), sourceSceneId_, targetSceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to move entity {}: {}", static_cast<unsigned int>(entityId_), e.what());
		}
	}

	// BatchMoveEntitiesTask implementation
	BatchMoveEntitiesTask::BatchMoveEntitiesTask(const Utilities::UUID sourceSceneId, 
		const Utilities::UUID targetSceneId, 
		const std::vector<EntityID>& entityIds) :
		Task(100, SlowMultiThreaded), sourceSceneId_(sourceSceneId), 
		targetSceneId_(targetSceneId), entityIds_(entityIds) {}

	void BatchMoveEntitiesTask::run()
	{
		try {
			// Note: This is a simplified implementation
			// A full implementation would need to move all components
			HARMONY_WARN("BatchMoveEntitiesTask: Full entity moving not yet implemented");
			HARMONY_INFO("Batch moving {} entities from scene {} to scene {}", 
				entityIds_.size(), sourceSceneId_, targetSceneId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to batch move entities: {}", e.what());
		}
	}
}
