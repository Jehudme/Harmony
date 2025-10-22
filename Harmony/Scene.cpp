#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include "Transform.h"
#include "Script.h"
#include "Configuration.h"
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
		drawOrder(configuration.get<int>({"drawOrder"}).value_or(0)),
		impl_(std::make_unique<SceneImpl>())
	{
		initialize();
	}

	void Scene::initialize()
	{
		std::lock_guard<std::mutex> lock(entityMutex_);
		
		// Clear existing entities if any
		for (auto entity : impl_->registry.view<entt::entity>()) {
			impl_->registry.destroy(static_cast<entt::entity>(entity));
		}

		if (std::optional<Utilities::UUIDList> entitiesKeys = configuration_.get<Utilities::UUIDList>({"entities"})) {
			for (const Utilities::UUID entityKey : entitiesKeys.value()) {
				if (std::optional<Utilities::Configuration> configuration = engine.configuration.subsection({ "entities", std::to_string(entityKey) })) {
					// Temporarily unlock to call createEntity (which will lock internally)
					// Actually, we're already holding the lock, so we need to be careful here
					// Let me refactor this differently
					const entt::entity entity = impl_->registry.create();

					for (const std::string& componentName : configuration.value().extractKeys({ "components" }))
						Management::ComponentManager::createComponent(componentName, configuration.value().subsection({ "components", componentName}).value(), entity, *this);

					if (std::optional<std::string> scriptName = configuration.value().get<std::string>({ "script" })) {
						Management::ComponentManager::createComponent(scriptName.value(), configuration.value().subsection({ "script" }).value(), entity, *this);
						Components::Script& script = componentReference<Components::Script>(static_cast<EntityID>(entity));
						script.scene_ = *this;
						script.onCreate();
					}

					HARMONY_DEBUG("Entity {} created during initialization", static_cast<std::uint32_t>(entity));
				}
				else {
					HARMONY_ERROR("Entity {} has no configuration defined in engine configuration", entityKey);
				}
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
		std::lock_guard<std::mutex> lock(entityMutex_);
		
		// Inline destroy logic to avoid recursive locking
		for (auto entity : impl_->registry.view<entt::entity>()) {
			if (impl_->registry.try_get<Components::Script>(entity))
				componentReference<Components::Script>(static_cast<EntityID>(entity)).onDestroy();

			impl_->registry.destroy(entity);
		}

		engine.sceneManagement->remove(sceneId);
		HARMONY_INFO("Scene {} destroyed", sceneId);
	}

	void Scene::internalDraw(sf::RenderTarget& renderTarget) const
	{
		if (!drawingEnabled_.load()) {
			return;
		}

		std::lock_guard<std::mutex> lock(entityMutex_);
		
		auto entitiesView = impl_->registry.view<std::unique_ptr<sf::Drawable>>();

		for (const entt::entity entity : entitiesView) {
			auto& drawable = entitiesView.get<std::unique_ptr<sf::Drawable>>(entity);
			sf::RenderStates entityStates;

			if (const auto& transform = impl_->registry.try_get<std::unique_ptr<Components::Transform>>(entity)) {
				const sf::Transformable* sfTransform = static_cast<const sf::Transformable*>((*transform)->getInternalTransform());
				if (sfTransform) {
					entityStates.transform *= sfTransform->getTransform();
				}
			}

			renderTarget.draw(*drawable, entityStates);
		}
	}

	void Scene::update(float deltaTime) 
	{
		if (!updatingEnabled_.load()) {
			return;
		}

		std::lock_guard<std::mutex> lock(entityMutex_);
		
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
		std::lock_guard<std::mutex> lock(entityMutex_);
		
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
		std::lock_guard<std::mutex> lock(entityMutex_);
		
		entt::entity entity = static_cast<entt::entity>(entityId);
		
		if (impl_->registry.try_get<Components::Script>(entity))
			componentReference<Components::Script>(entityId).onDestroy();

		impl_->registry.destroy(entity);

		HARMONY_DEBUG("Entity {} destroyed", entityId);
	}

	void Scene::enableDrawing()
	{
		drawingEnabled_.store(true);
		HARMONY_DEBUG("Scene {} drawing enabled", sceneId);
	}

	void Scene::disableDrawing()
	{
		drawingEnabled_.store(false);
		HARMONY_DEBUG("Scene {} drawing disabled", sceneId);
	}

	bool Scene::isDrawingEnabled() const noexcept
	{
		return drawingEnabled_.load();
	}

	void Scene::enableUpdating()
	{
		updatingEnabled_.store(true);
		HARMONY_DEBUG("Scene {} updating enabled", sceneId);
	}

	void Scene::disableUpdating()
	{
		updatingEnabled_.store(false);
		HARMONY_DEBUG("Scene {} updating disabled", sceneId);
	}

	bool Scene::isUpdatingEnabled() const noexcept
	{
		return updatingEnabled_.load();
	}

	void Scene::reset()
	{
		HARMONY_INFO("Resetting scene {}", sceneId);
		initialize();
	}
}
