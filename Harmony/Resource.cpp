#include "pch.h"
#include "Resource.h"

namespace Harmony::Resources {
	Resource::Resource(ResourceID id, Configuration configuration, Harmony::Internals::ResourcesHandler& handler)
		: m_configuration(configuration), m_id(id), m_handler(handler) {

		m_permanently_loaded = m_configuration.get<bool>({ "alwaysLoaded" }).value_or(false);
		m_lifespan = Time::fromSeconds(m_configuration.get<float>({ "lifespan" }).value_or(5));
	}

	Resource::~Resource() = default;

	ResourceID Resource::resourceId() const 
	{
		return m_id;
	}

	bool Resource::unloadable() const
	{
		std::shared_lock lock(m_mutex);

		if (loaded() && !m_permanently_loaded && timeSinceLastAccess() >= m_lifespan)
			return true;

		return false;
	}

	Time Resource::timeSinceLastAccess() const 
	{
		return m_access_clock.getElapsedTime();
	}

	Time Resource::restartAccessClock()
	{
		return m_access_clock.restart();
	}

	ScopedResourceAccess::ScopedResourceAccess(std::shared_lock<std::shared_mutex>&& lock, Resource& resource) :
		m_sharedLock(std::move(lock)), m_resource(resource) 
	{
	}

	ScopedResourceAccess::~ScopedResourceAccess() 
	{
		m_resource.restartAccessClock();
	}
}
