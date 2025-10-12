#include "pch.h"
#include "SceneManagement.h"
#include "Engine.h"
#include "Scene.h"

namespace Harmony::Internals {
	SceneManagement::SceneManagement(Engine& engine) :
		configuration_(engine.configuration.subsection({ "scenes-management" }).value_or(Configuration())),
		engine_(engine) {
	}
	void SceneManagement::create(const Utilities::UUID sceneId)
	{
	}

	void SceneManagement::destroy(const Utilities::UUID sceneId)
	{
	}
	std::optional<std::reference_wrapper<Scene>> SceneManagement::get(const Utilities::UUID sceneId) const
	{
		return std::optional<std::reference_wrapper<Scene>>();
	}
}
