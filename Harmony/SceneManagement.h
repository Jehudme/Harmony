#pragma once

#include "Configuration.h"

namespace Harmony::Internals {
	class Scene;

	class SceneManagement {
	public:
		SceneManagement(Configuration& configuration);

		std::shared_ptr<Scene> load(const std::uint64_t sceneId);
		std::shared_ptr<Scene> load(const std::string& name);

		std::optional<std::uint64_t> getSceneId(const std::string& name) const;

	private:
		const Configuration& configuration_;
	};
}