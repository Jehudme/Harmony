#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include "Transform.h"
#include "Script.h"

namespace Harmony::Scenes 
{
	void on_create_entity(entt::registry& registry, const entt::entity entity);
	void on_destroy_entity(entt::registry& registry, const entt::entity entity);

	Scene::Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine) :
		configuration_(configuration),
		sceneId(sceneId),
		engine(engine) 
	{
		if (std::optional<Utilities::UUIDList> entitiesKeys = configuration_.get<Utilities::UUIDList>({"entities"})) {
			for (const Utilities::UUID entityKey : entitiesKeys.value()) {
				if (std::optional<Utilities::Configuration> configuration = engine.configuration.subsection({ "entities", std::to_string(entityKey) }))
					createEntity(configuration.value());

				else HARMONY_ERROR("Entity {} has no configuration defined in engine configuration", entityKey);
			}
		}
		else {
			HARMONY_WARN("Scene {} has no entities defined in configuration", sceneId);
		}

		HARMONY_INFO("Scene {} initialized", sceneId);
	}

	void on_create_entity(entt::registry& registry, const entt::entity entity)
	{
		if (auto scriptComponent = registry.try_get<Components::Script>(entity)) {
			scriptComponent->onCreate();
		}
	}

	void on_destroy_entity(entt::registry& registry, const entt::entity entity)
	{
		if (auto scriptComponent = registry.try_get<Components::Script>(entity)) {
			scriptComponent->onDestroy();
		}
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
		auto entitiesView = registry_.view<std::unique_ptr<sf::Drawable>>();

		for (const entt::entity entity : entitiesView) {
			const sf::Drawable& drawable = componentReference<sf::Drawable>(entity);

			sf::RenderStates entityStates = states;
			if (const auto* transform = registry_.try_get<Components::Transform>(entity)) {
				entityStates.transform *= transform->getTransform();
			}

			target.draw(drawable, entityStates);
		}
	}

	void Scene::update(const sf::Time deltaTime) 
	{
		auto entitiesView = registry_.view<std::unique_ptr<Components::Script>>();

		for (const entt::entity entity : entitiesView) {
			Components::Script& scriptComponent = componentReference<Components::Script>(entity);
			scriptComponent.onPreUpdate();
		}

		for (const entt::entity entity : entitiesView) {
			Components::Script& scriptComponent = componentReference<Components::Script>(entity);
			scriptComponent.onPostUpdate();
		}
	}

	entt::entity Scene::createEntity(const Utilities::Configuration& configuration) 
	{
		const entt::entity entity = registry_.create();

		for (const std::string& componentName : configuration.extractKeys({ "components" }))
			Management::ComponentManager::createComponent(componentName, configuration.subsection({ "components", componentName}).value(), entity, *this);

		if (std::optional<std::string> scriptName = configuration.get<std::string>({ "script" })) {
			Management::ComponentManager::createComponent(scriptName.value(), configuration.subsection({ "script" }).value(), entity, *this);
			Components::Script& script = componentReference<Components::Script>(entity);
			script.scene_ = *this;
			script.onCreate();
		}

		HARMONY_DEBUG("Entity {} created", static_cast<std::uint32_t>(entity));
		return entity;
	}

	void Scene::destroyEntity(const entt::entity entityId)
	{
		if (registry_.try_get<Components::Script>(entityId))
			componentReference<Components::Script>(entityId).onDestroy();

		registry_.destroy(entityId);

		HARMONY_DEBUG("Entity {} destroyed", static_cast<std::uint32_t>(entityId));
	}
}
