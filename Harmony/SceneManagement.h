#pragma once

#include "Configuration.h"

namespace Harmony::Internals {
	class Engine;
	class Scene;

	class SceneManagement {
	public:
		SceneManagement(Engine& engine);

		std::shared_ptr<Scene> load(const std::uint64_t sceneId);
		std::optional<std::uint64_t> getId(const std::string& name) const;

	private:
		const Configuration& configuration_;
		Engine& engine_;
	};
}