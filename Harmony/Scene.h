#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Configuration.h"
#include "Logger.h"

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

		template<typename Type>
		Type& componentReference(entt::entity entityId);

		template<typename Type>
		Type& componentReference(entt::entity entityId) const;

		entt::entity createEntity(const Utilities::Configuration& configuration);
		void destroyEntity(const entt::entity entityId);

	private:
		entt::registry registry_;
		const Utilities::Configuration& configuration_;
	};

	template<typename Type, typename Registry>
	static Type& getComponentReferenceImpl(Registry& registry, entt::entity entityId) {
		if (auto* component = registry.template try_get<std::unique_ptr<Type>>(entityId)) {
			return *component->get();
		}
		else {
			HARMONY_CRITICAL("Entity {} does not have component of requested type",
				static_cast<std::uint32_t>(entityId));
			throw std::runtime_error("Entity " + std::to_string(static_cast<std::uint32_t>(entityId)) +
				" missing requested component");
		}
	}

	template<typename Type>
	inline Type& Scene::componentReference(entt::entity entityId) {
		return getComponentReferenceImpl<Type>(registry_, entityId);
	}

	template<typename Type>
	inline Type& Scene::componentReference(entt::entity entityId) const {
		return getComponentReferenceImpl<Type>(registry_, entityId);
	}
}