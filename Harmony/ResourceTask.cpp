#include "pch.h"
#include "ResourceTask.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "Logger.h"

namespace Harmony::Tasks
{
	// LoadResourceTask implementation
	LoadResourceTask::LoadResourceTask(const std::string& resourceType, const Utilities::UUID resourceId) :
		Task(75, SlowMultiThreaded), resourceType_(resourceType), resourceId_(resourceId) {}

	void LoadResourceTask::run()
	{
		try {
			getEngine().resourceManager->load(resourceType_, resourceId_);
			HARMONY_INFO("Resource {} of type '{}' loaded successfully", resourceId_, resourceType_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to load resource {} of type '{}': {}", resourceId_, resourceType_, e.what());
		}
	}

	// UnloadResourceTask implementation
	UnloadResourceTask::UnloadResourceTask(const Utilities::UUID resourceId) :
		Task(75, FastMultiThreaded), resourceId_(resourceId) {}

	void UnloadResourceTask::run()
	{
		try {
			getEngine().resourceManager->unload(resourceId_);
			HARMONY_INFO("Resource {} unloaded successfully", resourceId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to unload resource {}: {}", resourceId_, e.what());
		}
	}

	// UnloadAllResourcesTask implementation
	UnloadAllResourcesTask::UnloadAllResourcesTask() :
		Task(150, SlowMultiThreaded) {}

	void UnloadAllResourcesTask::run()
	{
		try {
			getEngine().resourceManager->unloadAll();
			HARMONY_INFO("All resources unloaded successfully");
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to unload all resources: {}", e.what());
		}
	}

	// BatchLoadResourcesTask implementation
	BatchLoadResourcesTask::BatchLoadResourcesTask(const std::vector<std::pair<std::string, Utilities::UUID>>& resources) :
		Task(100, SlowMultiThreaded), resources_(resources) {}

	void BatchLoadResourcesTask::run()
	{
		HARMONY_INFO("Batch loading {} resources", resources_.size());
		int successCount = 0;
		int failCount = 0;

		for (const auto& [resourceType, resourceId] : resources_) {
			try {
				getEngine().resourceManager->load(resourceType, resourceId);
				successCount++;
				HARMONY_DEBUG("Resource {} of type '{}' loaded successfully", resourceId, resourceType);
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Failed to load resource {} of type '{}': {}", resourceId, resourceType, e.what());
			}
		}

		HARMONY_INFO("Batch load completed: {} succeeded, {} failed", successCount, failCount);
	}
}
