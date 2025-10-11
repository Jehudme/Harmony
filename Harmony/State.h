#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <functional>
#include <vector>

#include "Configuration.h"
#include "TaskManagement.h"

namespace Harmony::Internals {
	struct Scene;

	class State : public sf::Drawable {
	public:
		State(const Configuration& configuration);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskQueue& taskManagement);

	private:
		std::vector<std::shared_ptr<Scene>> scenes_;
	};
}
