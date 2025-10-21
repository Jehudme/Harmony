#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include "Transform.h"
#include "Script.h"
#include <SFML/Graphics.hpp>
#include <Entt/entt.hpp>

namespace Harmony::Scenes 
{
	// PImpl struct to hide entt::registry
	struct Scene::SceneImpl {
		entt::registry registry;
	};

	// Helper functions for template access to registry
	entt::registry& getRegistryFromScene(Scene& scene) {
		return scene.impl_->registry;
	}

	const entt::registry& getRegistryFromScene(const Scene& scene) {
		return scene.impl_->registry;
	}

	void on_create_entity(entt::registry& registry, const entt::entity entity);
	void on_destroy_entity(entt::registry& registry, const entt::entity entity);


	Scene::Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine) :
		configuration_(configuration),
		sceneId(sceneId),
		engine(engine),
		impl_(std::make_unique<SceneImpl>())
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
		for (auto entity : impl_->registry.view<entt::entity>()) {
			destroyEntity(static_cast<EntityID>(entity));
		}

		engine.sceneManagement->remove(sceneId);
		HARMONY_INFO("Scene {} destroyed", sceneId);
	}

	void Scene::internalDraw(void* renderTarget) const
	{
		if (!renderTarget) return;
		sf::RenderTarget& target = *static_cast<sf::RenderTarget*>(renderTarget);
		sf::RenderStates states;

		auto entitiesView = impl_->registry.view<std::unique_ptr<sf::Drawable>>();

		for (const entt::entity entity : entitiesView) {
			const sf::Drawable& drawable = componentReference<sf::Drawable>(static_cast<EntityID>(entity));

			sf::RenderStates entityStates = states;
			if (auto* transform = impl_->registry.try_get<std::unique_ptr<Components::Transform>>(entity)) {
				// Get internal SFML transformable for rendering
				const sf::Transformable* sfTransform = static_cast<const sf::Transformable*>((*transform)->getInternalTransform());
				if (sfTransform) {
					entityStates.transform *= sfTransform->getTransform();
				}
			}

			target.draw(drawable, entityStates);
		}
	}

	void Scene::update(float deltaTime) 
	{
		auto entitiesView = impl_->registry.view<std::unique_ptr<Components::Script>>();

		for (const entt::entity entity : entitiesView) {
			Components::Script& scriptComponent = componentReference<Components::Script>(static_cast<EntityID>(entity));
			scriptComponent.onPreUpdate();
		}

		for (const entt::entity entity : entitiesView) {
			Components::Script& scriptComponent = componentReference<Components::Script>(static_cast<EntityID>(entity));
			scriptComponent.onPostUpdate();
		}
	}

	EntityID Scene::createEntity(const Utilities::Configuration& configuration) 
	{
		const entt::entity entity = impl_->registry.create();

		for (const std::string& componentName : configuration.extractKeys({ "components" }))
			Management::ComponentManager::createComponent(componentName, configuration.subsection({ "components", componentName}).value(), entity, *this);

		if (std::optional<std::string> scriptName = configuration.get<std::string>({ "script" })) {
			Management::ComponentManager::createComponent(scriptName.value(), configuration.subsection({ "script" }).value(), entity, *this);
			Components::Script& script = componentReference<Components::Script>(static_cast<EntityID>(entity));
			script.scene_ = *this;
			script.onCreate();
		}

		HARMONY_DEBUG("Entity {} created", static_cast<std::uint32_t>(entity));
		return static_cast<EntityID>(entity);
	}

	void Scene::destroyEntity(EntityID entityId)
	{
		entt::entity entity = static_cast<entt::entity>(entityId);
		
		if (impl_->registry.try_get<Components::Script>(entity))
			componentReference<Components::Script>(entityId).onDestroy();

		impl_->registry.destroy(entity);

		HARMONY_DEBUG("Entity {} destroyed", entityId);
	}
}
