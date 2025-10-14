#pragma once
#include "Scene.h"

namespace Harmony::Internals 
{
	class SceneManagement 
	{
	public:
		friend class Scene;
		friend class State;

		SceneManagement(Engine& engine);
		~SceneManagement();

		std::shared_ptr<Scene> create(const Utilities::UUID sceneId);
		std::shared_ptr<Scene> get(const Utilities::UUID sceneId) const;

	private:
		void remove(const Utilities::UUID sceneId);

	private:
		Engine& engine;
		std::unordered_map<std::uint64_t, std::weak_ptr<Scene>> scenes_;
	};
}