#pragma once

#include "Exceptions.h"
#include <queue>
#include <memory>
#include <shared_mutex>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Harmony::Management
{
	class StateManager
	{
	public:
		friend class Engine;
		StateManager(Engine& engine);
		~StateManager();

		void push(std::uint64_t stateId);
		void pop();
		
		std::shared_ptr<Scenes::State> getCurrentState() const;
	
	private:
		// Hide SFML drawing implementation
		void internalDraw(sf::RenderTarget& renderTarget) const;
		
		/// @param deltaTime - time since last frame in seconds
		void update(float deltaTime);

	private:
		Engine& engine;
		mutable std::shared_mutex mutex_;
		std::queue<std::shared_ptr<Scenes::State>> states_;
	};
}
