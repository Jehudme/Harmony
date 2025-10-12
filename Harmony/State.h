#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <functional>
#include <vector>

namespace Harmony::Internals {
	class SceneManagement;
	class TaskManagement;
	class Configuration;
	class Scene;
	class Engine;

	class State : public sf::Drawable {
	public:
		State(const Configuration& configuration, Engine& engine);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskManagement);

	private:
		std::vector<std::shared_ptr<Scene>> scenes_;
		Engine& engine_;
	};
}
