#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include "Transform.h"
#include "Script.h"
#include "View.h"
#include "Configuration.h"
#include "RenderManager.h"
#include <SFML/Graphics.hpp>
#include <Entt/entt.hpp>

namespace Harmony::Scenes 
{
	// PImpl struct to hide entt::registry
	struct Scene::SceneImpl {
		entt::registry registry;
		// View is now stored in registry ctx as a global component
	};

	// Helper functions for template access to registry
	entt::registry& getRegistryFromScene(Scene& scene) {
		return scene.impl_->registry;
	}

	const entt::registry& getRegistryFromScene(const Scene& scene) {
		return scene.impl_->registry;
	}

	Scene::Scene(const Utilities::Configuration& configuration, const Utilities::UUID sceneId, Engine& engine) :
		drawOrder(configuration.get<int>({ "drawOrder" }).value_or(0)),
		impl_(std::make_unique<SceneImpl>()),
		configuration_(configuration),
		sceneId(sceneId),
		engine(engine) {
		initialize();
	}

	void Scene::initialize()
	{
		// Clear existing entities if any
		for (auto entity : impl_->registry.view<entt::entity>()) {
			impl_->registry.destroy(static_cast<entt::entity>(entity));
		}

		std::vector<std::string> entitiesKeys = configuration_.extractKeys({ "entities" });
		for (std::string& entityKey : entitiesKeys)
			createEntity(configuration_.subsection({ "entities", entityKey }).value(), std::stoull(entityKey));

		// Initialize View component (stored in registry ctx as global)
		if (std::optional<Utilities::Configuration> viewConfig = configuration_.subsection({ "view" })) {
			createGlobalComponent<Components::View>(viewConfig.value(), *this);
			HARMONY_DEBUG("View created from configuration for scene {}", sceneId);
		}
		else {
			createGlobalComponent<Components::View>(engine.renderManager->getDefaultView());
			HARMONY_DEBUG("Default view created for scene {}", sceneId);
		}

		HARMONY_INFO("Scene {} initialized", sceneId);
	}

	Scene::~Scene() {
		std::lock_guard<std::mutex> lock(entityMutex_);
		
		// Inline destroy logic to avoid recursive locking
		for (auto entity : impl_->registry.view<entt::entity>()) {
			if (impl_->registry.try_get<Components::Script>(entity))
				getComponent<Components::Script>(static_cast<EntityID>(entity)).onDestroy();

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
		
		// Set the view before drawing the scene - now retrieved from registry ctx
		if (auto* view = impl_->registry.ctx().find<Components::View>()) {
			renderTarget.setView(*view);
		}
		
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
			Components::Script& scriptComponent = getComponent<Components::Script>(static_cast<EntityID>(entity));
			scriptComponent.onPreUpdate();
		}

		for (const entt::entity entity : entitiesView) {
			Components::Script& scriptComponent = getComponent<Components::Script>(static_cast<EntityID>(entity));
			scriptComponent.onPostUpdate();
		}
	}

	void Scene::createComponent(const std::string& componentName, const Utilities::Configuration& configuration, entt::entity entity){
		Harmony::Management::ComponentManager::createComponent(componentName, configuration, entity, *this);
	}

	void Scene::deleteComponent(const std::string& componentName, entt::entity entityId) {
		Harmony::Management::ComponentManager::deleteComponent(componentName, entityId, *this);
	}

	void Scene::createGlobalComponent(const std::string& componentName, const Utilities::Configuration& configuration) {
		Harmony::Management::ComponentManager::createComponent(componentName, configuration, entt::null, *this);
	}

	void Scene::deleteGlobalComponent(const std::string& componentName) {
		Harmony::Management::ComponentManager::deleteComponent(componentName, entt::null, *this);
	}

	EntityID Scene::createEntity(const Utilities::Configuration& configuration)
	{
		std::lock_guard<std::mutex> lock(entityMutex_);
		configuration.debugPrint();
		
		const entt::entity entity = impl_->registry.create();

		for (const std::string& componentName : configuration.extractKeys({ "components" }))
			Management::ComponentManager::createComponent(componentName, configuration.subsection({ "components", componentName}).value(), entity, *this);

		if (std::optional<std::string> scriptName = configuration.get<std::string>({ "script" })) {
			createComponent(scriptName.value(), configuration.subsection({ "script" }).value(), entity);

			Components::Script& script = getComponent<Components::Script>(static_cast<EntityID>(entity));
			script.entityId = entity; script.scene_ = *this;
			script.onCreate();
		}

		HARMONY_DEBUG("Entity {} created", static_cast<std::uint32_t>(entity));
		return static_cast<EntityID>(entity);
	}

	EntityID Scene::createEntity(const Utilities::Configuration& configuration, Utilities::UUID premadeId)
	{
		Utilities::Configuration mergedConfiguration;
		if (std::optional<Utilities::Configuration> premadeConfiguration = engine.configuration.subsection({ "entities", std::to_string(premadeId) }))
			mergedConfiguration.merge(premadeConfiguration.value());

		else HARMONY_WARN("Premade entity {} has no configuration defined in engine configuration", premadeId);

		mergedConfiguration.merge(configuration);
		return createEntity(mergedConfiguration);
	}

	void Scene::destroyEntity(EntityID entityId)
	{
		std::lock_guard<std::mutex> lock(entityMutex_);
		
		entt::entity entity = static_cast<entt::entity>(entityId);
		
		if (impl_->registry.try_get<Components::Script>(entity))
			getComponent<Components::Script>(entityId).onDestroy();

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

	Components::View& Scene::getView()
	{
		auto* view = impl_->registry.ctx().find<Components::View>();
		if (!view) {
			throw Exceptions::ComponentNotFoundException(0);  // View is not tied to a specific entity
		}
		return *view;
	}

	const Components::View& Scene::getView() const
	{
		const auto* view = impl_->registry.ctx().find<Components::View>();
		if (!view) {
			throw Exceptions::ComponentNotFoundException(0);  // View is not tied to a specific entity
		}
		return *view;
	}

	void Scene::setView(const Utilities::Configuration& configuration)
	{
		// Remove existing view if present
		impl_->registry.ctx().erase<Components::View>();
		// Create new view in ctx
		impl_->registry.ctx().emplace<Components::View>(configuration, *this);
		HARMONY_DEBUG("View created for scene {}", sceneId);
	}

	void Scene::reset()
	{
		HARMONY_INFO("Resetting scene {}", sceneId);
		initialize();
	}
}
