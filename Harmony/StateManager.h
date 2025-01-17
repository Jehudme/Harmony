#pragma once
#include "State.h"

namespace Harmony::Core
{
	class StateManager : public sf::Drawable
	{
	public:
		StateManager();

		void draw(sf::RenderTarget& renderState, sf::RenderStates states) const override;
		void update(const sf::Time& time);

		void addState(const std::string& name, const std::shared_ptr<State> state);
		void removeState(const std::string& name);

		void pushState(const std::string& name);
		void popState();

		void switchState(const std::string& name);

		std::shared_ptr<State> getCurrentState();

	private:
		std::shared_ptr<State> m_currentState;
		std::map<std::string, std::shared_ptr<State>> m_states;
	};
}

