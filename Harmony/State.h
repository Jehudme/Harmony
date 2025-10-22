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

	private:
		std::map<Utilities::UUID, std::shared_ptr<Scene>> scenes_;
	};
}
