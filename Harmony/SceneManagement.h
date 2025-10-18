#pragma once
#include "Scene.h"

namespace Harmony::Errors {
	/// Exception type for scene management errors
	class SceneManagerError : public std::runtime_error {
	public:
		explicit SceneManagerError(const std::string& msg);
	};
}

namespace Harmony::Management 
{
	class SceneManager 
	{
	public:
		friend class Scenes::Scene;
		friend class Scenes::State;

		SceneManager(Harmony::Engine& engine);
		~SceneManager();

		std::shared_ptr<Scenes::Scene> create(const Utilities::UUID sceneId);
		std::shared_ptr<Scenes::Scene> get(const Utilities::UUID sceneId) const;

	private:
		void remove(const Utilities::UUID sceneId);

	private:
		Engine& engine;
		mutable std::mutex mutex_;
		std::unordered_map<std::uint64_t, std::weak_ptr<Scenes::Scene>> scenes_;
	};
}