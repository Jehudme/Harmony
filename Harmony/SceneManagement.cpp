#include "pch.h"
#include "SceneManagement.h"
#include "Engine.h"

namespace Harmony::Internals {
	SceneManagement::SceneManagement(Engine& engine) :
		configuration_(engine.configuration.subsection({ "scenes-management" }).value_or(Configuration())),
		engine_(engine) {
	}

	std::shared_ptr<Scene> SceneManagement::load(const std::uint64_t sceneId)
	{
		return std::shared_ptr<Scene>();
	}
}
