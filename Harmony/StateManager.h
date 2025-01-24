#pragma once
#include "State.h"
#include <map>

namespace harmony::core
{
	class StateManager : public sf::Drawable
	{
	public:
		StateManager();

		void draw(sf::RenderTarget& renderState, sf::RenderStates states) const override;
		void update(const sf::Time& time, EventQueue& eventQueue);

		void addState(const std::shared_ptr<State> state);
		void removeState(const std::string& name);

		void swichState(const std::string& name);
		void popState();

		std::shared_ptr<State> getCurrentState() const;

	private:
		std::shared_ptr<State> m_currentState;
		std::map<std::string, std::shared_ptr<State>> m_states;
	};
}

