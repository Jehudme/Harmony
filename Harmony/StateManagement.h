#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <queue>
#include <memory>

#include "Configuration.h"
#include "TaskManagement.h"
#include "State.h"

namespace Harmony::Internals {

	
	class StateManagement : public sf::Drawable {
	public:
		StateManagement(const Configuration& configuration);

		void  push(std::uint64_t stateId);
		void  swap(std::uint64_t stateId);

		void  pop();

		std::optional<std::uint64_t> getId(const std::string& name) const;
	
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskQueue);

	private:
		const Configuration& configuration_;
		std::queue<std::shared_ptr<State>> states_;
	};
}

