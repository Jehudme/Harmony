#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Configuration.h"

namespace Harmony::Scenes
{
	class Scene : public sf::Drawable, public std::enable_shared_from_this<Scene> 
	{
	public:
		friend class Management::ComponentManager;
		Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine);
		~Scene();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime);

	public:
		Engine& engine;
		const Utilities::UUID sceneId;

		entt::entity createEntity(const Utilities::Configuration& configuration);
		void destroyEntity(const entt::entity entityId);

	private:
		entt::registry registry_;
		const Utilities::Configuration& configuration_;
	};
}