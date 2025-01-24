#pragma once
#include "State.h"

namespace harmony::core
{
	class StateManager : public sf::Drawable
	{
	public:
		StateManager();

		void draw(sf::RenderTarget& renderState, sf::RenderStates states) const override;
		void update(const sf::Time& time, EventPool& eventPool);

		void addState(const std::shared_ptr<State> state);
		void removeState(const uint64_t& uniqueId);

		void swichState(const uint64_t& uniqueId);
		void popState();

		void switchState(const uint64_t& name);

		std::shared_ptr<State> getCurrentState();

	private:
		std::shared_ptr<State> m_currentState;
		std::map<uint64_t, std::shared_ptr<State>> m_states;
	};
}

