#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <functional>

namespace Harmony::Internals {
	class Scene : public sf::Drawable, public std::enable_shared_from_this<Scene> {
	public:
		Scene(const Configuration& configuration, const Utilities::UUID sceneId, Engine& engine_);
		~Scene();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskManagement& taskManagement);

	public:
		Engine& engine_;
		const Utilities::UUID sceneId;

	private:
		const Configuration& configuration_;
	};
}