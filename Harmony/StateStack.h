#pragma once
#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <stack>
namespace Harmony
{
	class State;
	class TaskQueue;
	struct Script;

	class StateStack : public Object, public sf::Drawable
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

		void push(const std::string& stateName);
		void push(const uint64_t& uniqueId);

		void pop();

		std::shared_ptr<State> getCurrent();

		std::shared_ptr<State> get(const uint64_t& uniqueId);
		std::shared_ptr<State> get(const std::string& name);

	private:
		std::vector<std::shared_ptr<State>> m_states;
		std::stack<std::shared_ptr<State>> m_buffer;
		std::shared_ptr<Script> m_script;
	};
}

