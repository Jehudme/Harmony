#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include "Transform.h"
#include "Drawable.h"

namespace Harmony::Scenes 
{
	Scene::Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine) :
		configuration_(configuration),
		sceneId(sceneId),
		engine(engine) 
	{

		if (std::optional<Utilities::UUIDList> entitiesKeys = configuration_.get<Utilities::UUIDList>({"entities"}))
		{
			for (const Utilities::UUID entityKey : entitiesKeys.value()) 
			{
				if (std::optional<Utilities::Configuration> configuration = engine.configuration.subsection({ "entities", std::to_string(entityKey) }))
					createEntity(configuration.value());

				else HARMONY_ERROR("Entity {} has no configuration defined in engine configuration", entityKey);
			}
		}
		else
		{
			HARMONY_WARN("Scene {} has no entities defined in configuration", sceneId);
		}

		HARMONY_INFO("Scene {} initialized", sceneId);
	}

	Scene::~Scene() {
		for (auto entity : registry_.view<entt::entity>()) {
			destroyEntity(entity);
		}

		engine.sceneManagement->remove(sceneId);
		HARMONY_INFO("Scene {} destroyed", sceneId);
	}

	void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Iterate all entities that have a Drawable
		auto view = registry_.view<std::unique_ptr<Components::Drawable>>();

		for (auto entity : view) {
			const Components::Drawable& drawable = getComponent<Components::Drawable>(entity);
			sf::RenderStates entityStates = states;

			if (const auto* transform = registry_.try_get<Components::Transform>(entity)) {
				entityStates.transform *= transform->getTransform();
			}

			target.draw(drawable, entityStates);
		}
	}

	void Scene::update(const sf::Time deltaTime) 
	{

	}

	entt::entity Scene::createEntity(const Utilities::Configuration& configuration) 
	{
		const entt::entity entity = registry_.create();

		for (const std::string& componentName : configuration.extractKeys({ "components" }))
			Management::ComponentManager::createComponent(componentName, configuration.subsection({ "components", componentName}).value(), entity, *this);
	
		auto registeredEntities = registry_.view<entt::entity>();

		HARMONY_DEBUG("Entity {} created", static_cast<std::uint32_t>(entity));
		return entity;
	}

	void Scene::destroyEntity(const entt::entity entityId)
	{
		registry_.destroy(entityId);
		HARMONY_DEBUG("Entity {} destroyed", static_cast<std::uint32_t>(entityId));
	}
}
