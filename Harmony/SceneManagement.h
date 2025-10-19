#pragma once

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
		std::shared_ptr<Scenes::Scene> find(const Utilities::UUID sceneId) const;

	private:
		void remove(const Utilities::UUID sceneId);

	private:
		Engine& engine;
		mutable std::shared_mutex mutex_;
		std::unordered_map<std::uint64_t, std::weak_ptr<Scenes::Scene>> scenes_;
	};
}

namespace Harmony::Exceptions
{
	struct SceneConfigurationNotFoundError : std::runtime_error {
		explicit SceneConfigurationNotFoundError(const std::string& sceneKey);
	};

	struct ExpiredSceneError : std::runtime_error {
		explicit ExpiredSceneError(const Utilities::UUID sceneId);
	};
}