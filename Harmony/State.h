#pragma once

#include <memory>
#include <map>

namespace Harmony::Scenes
{
	class State : public std::enable_shared_from_this<State>  
	{
	public:
		State(const Utilities::Configuration& configuration, Engine& engine);
		~State();

		// Hide SFML drawing implementation
		void internalDraw(void* renderTarget) const;
		
		/// @param deltaTime - time since last frame in seconds
		void update(float deltaTime);
	
	public:
		Engine& engine;

	private:
		std::map<Utilities::UUID, std::shared_ptr<Scene>> scenes_;
	};
}
