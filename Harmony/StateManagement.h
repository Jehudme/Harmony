#pragma once

namespace Harmony::Internals 
{
	class StateManagement : public sf::Drawable 
	{
	public:
		StateManagement(Engine& engine);
		~StateManagement();

		void  push(std::uint64_t stateId);
		void  pop();
	
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskQueue);

	private:
		Engine& engine;
		std::queue<std::shared_ptr<State>> states_;
	};
}

