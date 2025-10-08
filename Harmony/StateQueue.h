#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <queue>

#include "Configuration.h"

namespace Harmony::Internals {

	class State;
	
	class StateQueue : public sf::Drawable {
		StateQueue(Configuration& configuration);

		void  push(std::unique_ptr<State> state);
		void  pop();

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime);

	private:
		Configuration& configuration_;
		std::queue<std::unique_ptr<State>> states_;
	};
}

