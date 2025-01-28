#pragma once
#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <stack>
namespace Harmony
{
	class State;
	class TaskQueue;
	class StateStack : public Object, private sf::Drawable
	{
	public:
		StateStack(std::shared_ptr<Configuration> configuration);

		void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
		void update(const sf::Time& time, TaskQueue& taskQueue);

		void add(std::shared_ptr<State> state);

		void remove(const uint64_t& uniqueId);
		void remove(const std::string& name);

		void removeAll();
		void clear();

		std::shared_ptr<State> findState(const uint64_t& uniqueId);
		std::shared_ptr<State> findState(const std::string& name);

	private:
		std::vector<std::shared_ptr<State>> m_states;
		std::stack<std::shared_ptr<State>> m_buffer;
	};
}

