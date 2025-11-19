#include "pch.h"
#include "ResourceHandler.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assert.h"

#include <algorithm>

namespace Harmony::Internals {

	ResourcesHandler::ResourcesHandler(const Configuration& configuration)
		: memoryCap_(1024 * 1024 * 1024)  // Default 1GB
		, defaultUnloadDelay_(60.0f)       // Default 60 seconds
		, enableLogging_(true)
		, currentMemoryUsage_(0)
	{
		HARMONY_INFO("Initializing ResourcesHandler");

		// Read configuration
		if (auto cap = configuration.get<std::size_t>({ "resources", "memoryCap" })) {
			memoryCap_ = cap.value();
			HARMONY_INFO("Resource memory cap set to {} bytes ({} MB)", memoryCap_, memoryCap_ / (1024 * 1024));
		}

		if (auto delay = configuration.get<float>({ "resources", "defaultUnloadDelay" })) {
			defaultUnloadDelay_ = delay.value();
			HARMONY_INFO("Default unload delay set to {} seconds", defaultUnloadDelay_);
		}

		if (auto logging = configuration.get<bool>({ "resources", "enableLogging" })) {
			enableLogging_ = logging.value();
		}

		// Read preload list
		if (auto preloadOpt = configuration.subsection({ "resources", "preload" })) {
			auto keys = preloadOpt->extractKeys({ "resources", "preload" });
			for (const auto& key : keys) {
				if (auto id = configuration.get<ResourceID>({ "resources", "preload", key })) {
					preloadList_.push_back(id.value());
				}
			}
			if (!preloadList_.empty()) {
				HARMONY_INFO("Preload list contains {} resources", preloadList_.size());
			}
		}

		// Read always-loaded list
		if (auto alwaysOpt = configuration.subsection({ "resources", "alwaysLoaded" })) {
			auto keys = alwaysOpt->extractKeys({ "resources", "alwaysLoaded" });
			for (const auto& key : keys) {
				if (auto id = configuration.get<ResourceID>({ "resources", "alwaysLoaded", key })) {
					alwaysLoadedList_.push_back(id.value());
				}
			}
			if (!alwaysLoadedList_.empty()) {
				HARMONY_INFO("Always-loaded list contains {} resources", alwaysLoadedList_.size());
			}
		}

		HARMONY_INFO("ResourcesHandler initialized successfully");
	}

	ResourcesHandler::~ResourcesHandler()
	{
		HARMONY_INFO("Shutting down ResourcesHandler");
		unloadAll();
	}

	void ResourcesHandler::registerResource(std::shared_ptr<Resource_t> resource)
	{
		HARMONY_ASSERT_NOT_NULL(resource, "Cannot register null resource");

		std::unique_lock lock(resourcesMutex_);

		auto id = resource->getId();
		if (resources_.find(id) != resources_.end()) {
			HARMONY_WARN("Resource with ID {} already registered, replacing", id);
		}

		// Check if this resource should be marked as always-loaded
		if (std::find(alwaysLoadedList_.begin(), alwaysLoadedList_.end(), id) != alwaysLoadedList_.end()) {
			resource->setAlwaysLoaded(true);
			if (enableLogging_) {
				HARMONY_INFO("Resource {} marked as always-loaded", id);
			}
		}

		resources_[id] = resource;

		if (enableLogging_) {
			HARMONY_INFO("Registered {} resource (ID: {}, Path: {})", 
				resource->getTypeName(), id, resource->getFilePath());
		}

		// Check if this resource should be preloaded
		if (std::find(preloadList_.begin(), preloadList_.end(), id) != preloadList_.end()) {
			lock.unlock();  // Unlock before calling load
			load(id);
		}
	}

	void ResourcesHandler::unregisterResource(ResourceID id)
	{
		std::unique_lock lock(resourcesMutex_);

		auto it = resources_.find(id);
		if (it == resources_.end()) {
			HARMONY_WARN("Attempted to unregister non-existent resource {}", id);
			return;
		}

		auto resource = it->second;
		if (resource->isLoaded()) {
			lock.unlock();
			unload(id);
			lock.lock();
		}

		resources_.erase(it);

		if (enableLogging_) {
			HARMONY_INFO("Unregistered resource {}", id);
		}
	}

	void ResourcesHandler::load(ResourceID id)
	{
		try {
			loadInternal(id);
		}
		catch (const Exceptions::ResourceException& e) {
			HARMONY_ERROR("Failed to load resource {}: {}", id, e.what());
			throw;
		}
	}

	void ResourcesHandler::loadInternal(ResourceID id)
	{
		std::shared_lock readLock(resourcesMutex_);

		auto it = resources_.find(id);
		if (it == resources_.end()) {
			readLock.unlock();
			throw Exceptions::ResourceNotFoundException("Resource", id);
		}

		auto resource = it->second;
		readLock.unlock();

		// Check if already loaded
		if (resource->isLoaded()) {
			resource->updateLastUsedTime();
			if (enableLogging_) {
				HARMONY_TRACE("Resource {} already loaded, updating last used time", id);
			}
			return;
		}

		// Check memory cap before loading
		std::size_t resourceSize = resource->getFileSize();
		std::size_t currentUsage = currentMemoryUsage_.load();

		if (currentUsage + resourceSize > memoryCap_ && !resource->isAlwaysLoaded()) {
			// Try to free memory by evicting LRU resources
			while (currentUsage + resourceSize > memoryCap_) {
				evictLeastRecentlyUsed();
				currentUsage = currentMemoryUsage_.load();
			}
		}

		// Load the resource
		if (enableLogging_) {
			HARMONY_INFO("Loading {} resource (ID: {}, Path: {}, Size: {} bytes)",
				resource->getTypeName(), id, resource->getFilePath(), resourceSize);
		}

		resource->load();
		resource->updateLastUsedTime();

		// Update memory usage
		currentMemoryUsage_ += resourceSize;

		if (enableLogging_) {
			HARMONY_INFO("{} resource loaded successfully (Memory usage: {} / {} bytes)",
				resource->getTypeName(), currentMemoryUsage_.load(), memoryCap_);
		}
	}

	void ResourcesHandler::unload(ResourceID id)
	{
		try {
			unloadInternal(id);
		}
		catch (const Exceptions::ResourceException& e) {
			HARMONY_ERROR("Failed to unload resource {}: {}", id, e.what());
			throw;
		}
	}

	void ResourcesHandler::unloadInternal(ResourceID id)
	{
		std::shared_lock readLock(resourcesMutex_);

		auto it = resources_.find(id);
		if (it == resources_.end()) {
			readLock.unlock();
			HARMONY_WARN("Attempted to unload non-existent resource {}", id);
			return;
		}

		auto resource = it->second;
		readLock.unlock();

		if (!resource->isLoaded()) {
			if (enableLogging_) {
				HARMONY_TRACE("Resource {} already unloaded", id);
			}
			return;
		}

		if (resource->isAlwaysLoaded()) {
			HARMONY_WARN("Attempted to unload always-loaded resource {}", id);
			return;
		}

		std::size_t resourceSize = resource->getFileSize();

		if (enableLogging_) {
			HARMONY_INFO("Unloading {} resource (ID: {}, Size: {} bytes)",
				resource->getTypeName(), id, resourceSize);
		}

		resource->unload();

		// Update memory usage
		currentMemoryUsage_ -= resourceSize;

		if (enableLogging_) {
			HARMONY_INFO("{} resource unloaded (Memory usage: {} / {} bytes)",
				resource->getTypeName(), currentMemoryUsage_.load(), memoryCap_);
		}
	}

	std::shared_ptr<Resource_t> ResourcesHandler::get(ResourceID id)
	{
		std::shared_lock lock(resourcesMutex_);

		auto it = resources_.find(id);
		if (it == resources_.end()) {
			lock.unlock();
			throw Exceptions::ResourceNotFoundException("Resource", id);
		}

		auto resource = it->second;
		lock.unlock();

		// Auto-load if not loaded
		if (!resource->isLoaded()) {
			load(id);
		}
		else {
			resource->updateLastUsedTime();
		}

		return resource;
	}

	void ResourcesHandler::batchLoad(const std::vector<ResourceID>& ids)
	{
		if (enableLogging_) {
			HARMONY_INFO("Batch loading {} resources", ids.size());
		}

		for (auto id : ids) {
			try {
				load(id);
			}
			catch (const Exceptions::ResourceException& e) {
				HARMONY_ERROR("Failed to batch load resource {}: {}", id, e.what());
			}
		}

		if (enableLogging_) {
			HARMONY_INFO("Batch load complete");
		}
	}

	void ResourcesHandler::batchUnload(const std::vector<ResourceID>& ids)
	{
		if (enableLogging_) {
			HARMONY_INFO("Batch unloading {} resources", ids.size());
		}

		for (auto id : ids) {
			try {
				unload(id);
			}
			catch (const Exceptions::ResourceException& e) {
				HARMONY_ERROR("Failed to batch unload resource {}: {}", id, e.what());
			}
		}

		if (enableLogging_) {
			HARMONY_INFO("Batch unload complete");
		}
	}

	void ResourcesHandler::loadAll()
	{
		std::shared_lock lock(resourcesMutex_);
		std::vector<ResourceID> ids;
		ids.reserve(resources_.size());

		for (const auto& pair : resources_) {
			ids.push_back(pair.first);
		}
		lock.unlock();

		if (enableLogging_) {
			HARMONY_INFO("Loading all {} resources", ids.size());
		}

		batchLoad(ids);
	}

	void ResourcesHandler::unloadAll()
	{
		std::shared_lock lock(resourcesMutex_);
		std::vector<ResourceID> ids;
		ids.reserve(resources_.size());

		for (const auto& pair : resources_) {
			if (!pair.second->isAlwaysLoaded()) {
				ids.push_back(pair.first);
			}
		}
		lock.unlock();

		if (enableLogging_) {
			HARMONY_INFO("Unloading all non-permanent resources ({} resources)", ids.size());
		}

		batchUnload(ids);
	}

	void ResourcesHandler::updateAutoUnload()
	{
		std::shared_lock lock(resourcesMutex_);
		std::vector<ResourceID> toUnload;

		for (const auto& pair : resources_) {
			if (pair.second->isLoaded() && pair.second->shouldUnload()) {
				toUnload.push_back(pair.first);
			}
		}
		lock.unlock();

		if (!toUnload.empty()) {
			if (enableLogging_) {
				HARMONY_INFO("Auto-unloading {} idle resources", toUnload.size());
			}
			batchUnload(toUnload);
		}
	}

	void ResourcesHandler::updateMusicStreaming()
	{
		// This will be implemented in MusicResource
		// For now, just iterate through resources and update music streaming
		std::shared_lock lock(resourcesMutex_);

		for (const auto& pair : resources_) {
			if (pair.second->isLoaded()) {
				// Check if it's a music resource and update streaming
				// This will be handled by the specific resource type
			}
		}
	}

	void ResourcesHandler::setMemoryCap(std::size_t cap)
	{
		if (enableLogging_) {
			HARMONY_INFO("Changing memory cap from {} to {} bytes", memoryCap_, cap);
		}

		memoryCap_ = cap;

		// If current usage exceeds new cap, evict resources
		while (currentMemoryUsage_.load() > memoryCap_) {
			evictLeastRecentlyUsed();
		}
	}

	void ResourcesHandler::evictLeastRecentlyUsed()
	{
		std::shared_lock lock(resourcesMutex_);

		// Find the least recently used loaded resource that isn't always-loaded
		ResourceID lruId = 0;
		std::chrono::steady_clock::time_point oldestTime = std::chrono::steady_clock::now();
		bool found = false;

		for (const auto& pair : resources_) {
			auto resource = pair.second;
			if (resource->isLoaded() && !resource->isAlwaysLoaded()) {
				if (!found || resource->getLastUsedTime() < oldestTime) {
					oldestTime = resource->getLastUsedTime();
					lruId = pair.first;
					found = true;
				}
			}
		}

		lock.unlock();

		if (found) {
			if (enableLogging_) {
				HARMONY_INFO("Evicting least recently used resource {} to enforce memory cap", lruId);
			}
			unload(lruId);
		}
		else {
			if (enableLogging_) {
				HARMONY_WARN("No resources available for eviction (all are always-loaded)");
			}
		}
	}

	std::size_t ResourcesHandler::getCurrentMemoryUsage() const
	{
		return currentMemoryUsage_.load();
	}

	std::size_t ResourcesHandler::getLoadedResourceCount() const
	{
		std::shared_lock lock(resourcesMutex_);
		std::size_t count = 0;
		for (const auto& pair : resources_) {
			if (pair.second->isLoaded()) {
				++count;
			}
		}
		return count;
	}

	std::size_t ResourcesHandler::getTotalResourceCount() const
	{
		std::shared_lock lock(resourcesMutex_);
		return resources_.size();
	}

} // namespace Harmony
