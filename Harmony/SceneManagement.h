#pragma once
#include "Scene.h"

namespace Harmony::Internals {
	class SceneManagement {
	public:
		SceneManagement(Engine& engine);

		std::shared_ptr<Scene> create(const Utilities::UUID sceneId);
		void destroy(const Utilities::UUID sceneId);

		std::shared_ptr<Scene> get(const Utilities::UUID sceneId) const;
	
	private:
		Engine& engine_;
		std::unordered_map<std::uint64_t, std::weak_ptr<Scene>> scenes_;
	};
}