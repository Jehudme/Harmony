#pragma once

#include <string>
#include <chrono>
#include <cstddef>

namespace Harmony {

	/// Type alias for Resource ID (simple unsigned int redefinition as requested)
	using ResourceID = unsigned int;

	/// Base class for all resources in the Harmony engine
	/// Provides common interface for resource management including:
	/// - Loading/unloading lifecycle
	/// - Last used time tracking
	/// - Automatic unloading based on idle time
	/// - Always-loaded flag for permanent resources
	/// - File size tracking for memory management
	class Resource_t {
	public:
		Resource_t(ResourceID id, const std::string& filePath, std::size_t fileSize = 0, 
			       float unloadDelay = 60.0f, bool alwaysLoaded = false);
		virtual ~Resource_t() = default;

		// Core resource lifecycle
		virtual void load() = 0;
		virtual void unload() = 0;
		virtual bool isLoaded() const = 0;

		// Resource metadata
		ResourceID getId() const { return id_; }
		const std::string& getFilePath() const { return filePath_; }
		std::size_t getFileSize() const { return fileSize_; }

		// Time tracking
		std::chrono::steady_clock::time_point getLastUsedTime() const { return lastUsedTime_; }
		void updateLastUsedTime();
		float getUnloadDelay() const { return unloadDelay_; }
		void setUnloadDelay(float delay) { unloadDelay_ = delay; }

		// Always-loaded management
		bool isAlwaysLoaded() const { return alwaysLoaded_; }
		void setAlwaysLoaded(bool alwaysLoaded) { alwaysLoaded_ = alwaysLoaded; }

		// Check if resource should be unloaded based on idle time
		bool shouldUnload() const;

		// Get type name for logging/debugging
		virtual const char* getTypeName() const = 0;

	protected:
		void setFileSize(std::size_t size) { fileSize_ = size; }

	private:
		ResourceID id_;
		std::string filePath_;
		std::size_t fileSize_;
		std::chrono::steady_clock::time_point lastUsedTime_;
		float unloadDelay_;  // in seconds
		bool alwaysLoaded_;
	};

} // namespace Harmony
