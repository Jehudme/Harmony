#include "pch.h"
#include "Resource.h"

namespace Harmony::Resources {
	Resource::Resource(ResourceID id, Configuration configuration)
		: configuration_(configuration), available_(false), id_(id) {

		alwaysLoaded_ = configuration_.get<bool>({ "AlwaysLoaded" }).value_or(false);
		cooledownTime_ = Time::fromSeconds(configuration_.get<float>({ "CooledownTime" }).value_or(5));
		required_ = configuration_.get<bool>({ "Required" }).value_or(false);
	}

	Resource::~Resource() = default;

	ResourceID Resource::getID() const {
		return id_;
	}

	void Resource::setAvailable(bool available) {
		std::unique_lock lock(mutex_);
		available_ = available;
	}

	bool Resource::canUnload() const
	{
		std::shared_lock lock(mutex_);
		if (!available_)									return false;
		else if (alwaysLoaded_)								return false;
		else if (getTimeSinceLastAccess() < cooledownTime_) return false;
		else												return true;
	}

	bool Resource::isAvailable() const {
		std::shared_lock lock(mutex_);
		return available_;
	}

	Time Resource::getTimeSinceLastAccess() const {
		std::shared_lock lock(mutex_);
		return accessClock_.getElapsedTime();
	}

	Time Resource::restartAccessClock()
	{
		std::unique_lock lock(mutex_);
		return Time();
	}


	ResourceAcquirement::ResourceAcquirement(std::shared_lock<std::shared_mutex>&& lock, Resource& resource) :
		resourceLock_(std::move(lock)), resource_(resource) {}

	ResourceAcquirement::~ResourceAcquirement() = default;

	Resource& ResourceAcquirement::getResource() const {
		return resource_;
	}

	Resource& ResourceAcquirement::operator*() const {
		return resource_;
	}

}
