#pragma once

#include <memory>
#include <map>
#include <SFML/Graphics/RenderTarget.hpp>

// Forward declarations to avoid including headers
namespace Harmony {
	class Engine;
	namespace Utilities {
		class Configuration;
		using UUID = std::uint64_t;
	}
	namespace Scenes {
		class Scene;
	}
}

namespace Harmony::Scenes
{
	class State : public std::enable_shared_from_this<State>  
	{
	public:
		friend class Management::StateManager;
		State(const Utilities::Configuration& configuration, Engine& engine);
		~State();

	private:
		// Hide SFML drawing implementation
		void internalDraw(sf::RenderTarget& renderTarget) const;
		
		/// @param deltaTime - time since last frame in seconds
		void update(float deltaTime);
	
	public:
		Engine& engine;

		// Scene management methods for the current state
		void addScene(const Utilities::UUID sceneId, std::shared_ptr<Scene> scene);
		void removeScene(const Utilities::UUID sceneId);
		void removeSceneByOrder(int drawOrder);
		std::shared_ptr<Scene> getScene(const Utilities::UUID sceneId) const;

	private:
		std::vector<std::shared_ptr<Scene>> scenes_;
		mutable std::shared_mutex scenesMutex_;
	};
}
