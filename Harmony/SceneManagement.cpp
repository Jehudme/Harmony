#include "pch.h"
#include "SceneManagement.h"

namespace Harmony::Internals {
	SceneManagement::SceneManagement(const Configuration& configuration) :
		configuration_(configuration)
	{
	}
	std::shared_ptr<Scene> SceneManagement::load(const std::uint64_t sceneId)
	{
		return std::shared_ptr<Scene>();
	}
}
