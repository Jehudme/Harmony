#pragma once

#include <string>
#include <cstddef>

#include "Clock.h"
#include "Time.h"

namespace Harmony::Internals {
	class ResourcesHandler;
}

namespace Harmony::Resources {
	using ResourceID = uint64_t;

	class Resource {
		friend class Harmony::Internals::ResourcesHandler;

	public:
		Resource(ResourceID id, Configuration configuration);
		virtual ~Resource();

		virtual std::string getType() const = 0;

		ResourceID getID() const;

	protected:
		void setAvailable(bool available);

	private:
		bool canUnload() const;
		bool isAvailable() const;

		virtual void load() = 0;
		virtual void unload() = 0;

		Time getTimeSinceLastAccess() const;
		Time restartAccessClock();

	protected:
		mutable std::shared_mutex mutex_;
		Configuration configuration_;

	private:
		bool available_;
		ResourceID id_;

		Clock accessClock_;
		Time cooledownTime_;
		bool alwaysLoaded_;
		bool required_;
	};


	class ResourceAcquirement {
	public:
		ResourceAcquirement(std::shared_lock<std::shared_mutex>&& lock, Resource& resource);
		virtual ~ResourceAcquirement();

		ResourceAcquirement(const ResourceAcquirement&) = delete;
		ResourceAcquirement& operator=(const ResourceAcquirement&) = delete;
		ResourceAcquirement(ResourceAcquirement&&) = delete;
		ResourceAcquirement& operator=(ResourceAcquirement&&) = delete;

		Resource& getResource() const;
		Resource& operator*() const;

	private:
		std::shared_lock<std::shared_mutex> resourceLock_;
		Resource& resource_;
	};
} // namespace Harmony
