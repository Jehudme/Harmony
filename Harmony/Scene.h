#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace Harmony::Internals {
	class Configuration;
	class TaskManagement;

	class Scene : public sf::Drawable {
	public:
		Scene(const Configuration& configuration);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskManagement);
	};
}