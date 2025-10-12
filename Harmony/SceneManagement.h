#pragma once

#include "Configuration.h"

namespace Harmony::Internals {
	class Scene;

	class SceneManagement {
	public:
		SceneManagement(const Configuration& configuration);

		std::shared_ptr<Scene> load(const std::uint64_t sceneId);
		std::optional<std::uint64_t> getId(const std::string& name) const;

	private:
		const Configuration& configuration_;
	};
}