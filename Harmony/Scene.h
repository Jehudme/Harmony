#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <functional>

namespace Harmony::Internals {
	class Scene : public sf::Drawable, public std::enable_shared_from_this<Scene> {
	public:
		Scene(const Configuration& configuration, Engine& engine);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskManagement);

	private:
		std::function<void(Scene& scene, sf::RenderTarget& target, sf::RenderStates states)> onPreDrawFunction_;
		std::function<void(Scene& scene, sf::RenderTarget& target, sf::RenderStates states)> onPostDrawFunction_;
		std::function<void(Scene& scene, const sf::Time deltaTime, TaskManagement& taskManagement)> onPreUpdateFunction_;
		std::function<void(Scene& scene, const sf::Time deltaTime, TaskManagement& taskManagement)> onPostUpdateFunction_;

		Engine& engine_;
		const Configuration& configuration_;
	};
}