#pragma once

#include <string>
#include <cstddef>

#include "Clock.h"
#include "Time.h"

namespace Harmony::Internals {
	class ResourcesHandler;
}

namespace Harmony::Resources {
	class ScopedResourceAccess;
}

namespace Harmony::Resources {
	using ResourceID = uint64_t;

	class Resource {
		friend class Harmony::Internals::ResourcesHandler;
		friend class ScopedResourceAccess;

	public:
		Resource(ResourceID id, Configuration configuration, Harmony::Internals::ResourcesHandler& handler);
		virtual ~Resource();

		virtual const char* type() const = 0;

		ResourceID resourceId() const;

	private:
		bool unloadable() const;

		virtual void load() = 0;
		virtual void unload() = 0;
		virtual bool loaded() const = 0;

		Time timeSinceLastAccess() const;
		Time restartAccessClock();

	protected:
		mutable std::shared_mutex m_mutex;
		Configuration m_configuration;
		const ResourceID m_id;
		Harmony::Internals::ResourcesHandler& m_handler;

	private:
		Time m_lifespan;
		Clock m_access_clock;
		bool m_permanently_loaded;
	};

	class ScopedResourceAccess {
	public:
		ScopedResourceAccess(std::shared_lock<std::shared_mutex>&& lock, Resource& resource);
		virtual ~ScopedResourceAccess();

		ScopedResourceAccess(const ScopedResourceAccess&)				= delete;
		ScopedResourceAccess& operator=(const ScopedResourceAccess&)	= delete;
		ScopedResourceAccess(ScopedResourceAccess&&)					= delete;
		ScopedResourceAccess& operator=(ScopedResourceAccess&&)			= delete;

		template<typename Type>
		Type& resource() const {
			return dynamic_cast<Type&>(m_resource);
		}

	private:
		std::shared_lock<std::shared_mutex> m_sharedLock;
		Resource& m_resource;
	};
} // namespace Harmony
