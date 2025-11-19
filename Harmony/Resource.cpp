#include "pch.h"
#include "Resource.h"

namespace Harmony {

	Resource_t::Resource_t(ResourceID id, const std::string& filePath, std::size_t fileSize,
		                   float unloadDelay, bool alwaysLoaded)
		: id_(id)
		, filePath_(filePath)
		, fileSize_(fileSize)
		, lastUsedTime_(std::chrono::steady_clock::now())
		, unloadDelay_(unloadDelay)
		, alwaysLoaded_(alwaysLoaded)
	{
	}

	void Resource_t::updateLastUsedTime()
	{
		lastUsedTime_ = std::chrono::steady_clock::now();
	}

	bool Resource_t::shouldUnload() const
	{
		if (alwaysLoaded_) {
			return false;
		}

		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastUsedTime_);
		return elapsed.count() >= unloadDelay_;
	}

} // namespace Harmony
