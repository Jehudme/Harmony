#pragma once

namespace Harmony::Internals {
	class StateManagement : public sf::Drawable {
	public:
		StateManagement(Engine& engine);

		void  push(std::uint64_t stateId);
		void  pop();
	
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskQueue);

	private:
		Engine& engine_;
		std::queue<std::shared_ptr<State>> states_;
	};
}

