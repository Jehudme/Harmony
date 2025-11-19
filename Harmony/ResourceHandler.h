#pragma once

#include "Resource.h"
#include "Configuration.h"

#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>
#include <shared_mutex>

namespace Harmony::Internals {

	/// Thread-safe resource management system for Harmony engine
	/// Handles all 2D, 3D, and Audio resources with automatic memory management
	/// Features:
	/// - ID-based resource access
	/// - Automatic loading/unloading
	/// - Memory cap enforcement with LRU eviction
	/// - Thread-safe operations
	/// - Batch operations for efficiency
	/// - Configuration-driven behavior
	class ResourcesHandler {
	public:
		explicit ResourcesHandler(const Configuration& configuration);
		~ResourcesHandler();

		// Single resource operations
		void load(ResourceID id);
		void unload(ResourceID id);
		std::shared_ptr<Resource_t> get(ResourceID id);

		// Batch operations
		void batchLoad(const std::vector<ResourceID>& ids);
		void batchUnload(const std::vector<ResourceID>& ids);
		void loadAll();
		void unloadAll();

		// Registration - used to register resources before loading
		void registerResource(std::shared_ptr<Resource_t> resource);
		void unregisterResource(ResourceID id);

		// Memory management
		std::size_t getCurrentMemoryUsage() const;
		std::size_t getMemoryCap() const { return memoryCap_; }
		void setMemoryCap(std::size_t cap);

		// Automatic cleanup - call this periodically (e.g., every frame)
		void updateAutoUnload();

		// Music streaming update - call every frame
		void updateMusicStreaming();

		// Statistics
		std::size_t getLoadedResourceCount() const;
		std::size_t getTotalResourceCount() const;

	private:
		// Internal helper methods
		void loadInternal(ResourceID id);
		void unloadInternal(ResourceID id);
		void enforceMemoryCap();
		void evictLeastRecentlyUsed();

		// Configuration
		std::size_t memoryCap_;           // Maximum total memory in bytes
		float defaultUnloadDelay_;        // Default idle time before unload
		std::vector<ResourceID> preloadList_;
		std::vector<ResourceID> alwaysLoadedList_;
		bool enableLogging_;

		// Resource storage
		std::unordered_map<ResourceID, std::shared_ptr<Resource_t>> resources_;
		mutable std::shared_mutex resourcesMutex_;

		// Current memory usage
		std::atomic<std::size_t> currentMemoryUsage_;
	};

} // namespace Harmony
