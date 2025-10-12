#pragma once

#include "Configuration.h"
#include "Scene.h"

namespace Harmony::Internals {
	class Engine;
	class Scene;

	class SceneManagement {
	public:
		SceneManagement(Engine& engine);

		void create(const Utilities::UUID sceneId);
		void destroy(const Utilities::UUID sceneId);

		std::optional<std::reference_wrapper<Scene>> get(const Utilities::UUID sceneId) const;
	
	private:
		Engine& engine_;
		const Configuration& configuration_;

		std::unordered_map<std::uint64_t, Scene> scenes_;
	};
}