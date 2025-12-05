#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"
#include "Configuration.h"
#include "ComponentsHandler.h"
#include "Script.h"
#include "View3D.h"
#include "Renderable.h"

namespace Harmony::Internals
{
	Scene::Scene(Engine& engine, const SceneID sceneId) :
		sceneId(sceneId),
		engine(engine),
		activeRender(false),
		activeUpdate(false),
		updateOrder_(0),
		renderOrder_(0)
	{
		HARMONY_ASSERT_NOT_NULL(&engine != nullptr, "Engine reference cannot be null");
		
		HARMONY_DEBUG("Initializing Scene with ID {}", sceneId);
		
		try {
			const std::string key = std::to_string(static_cast<uint32_t>(sceneId));
			auto configSection = engine.configuration->subsection({ "scenes", key });
			
			if (!configSection.has_value()) {
				HARMONY_WARN("No configuration found for scene {}, using default configuration", sceneId);
				configuration_ = Configuration();
			} else {
				configuration_ = configSection.value();
			}
			
			initialize();
			
			HARMONY_INFO("Scene {} successfully initialized", sceneId);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Scene {}: {}", sceneId, e.what());
			throw Exceptions::SceneInitializationException(std::to_string(sceneId), e.what());
		}
	}

	Scene::~Scene() {
		HARMONY_DEBUG("Destroying Scene {}", sceneId);
		
		try {
			std::lock_guard<std::mutex> lock(registryMutex_);
			
			if (containsGlobalComponent<Components::Script>()) {
				getGlobalComponent<Components::Script>().onDestroy();
			}

			auto view = registry_.view<entt::entity>();
			std::vector<entt::entity> entities(view.begin(), view.end());
			for (entt::entity entity : entities) {
				destroyEntity(static_cast<EntityID>(entity));
			}
			
			HARMONY_INFO("Scene {} destroyed successfully", sceneId);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Scene {} destruction: {}", sceneId, e.what());
		}
	}

	void Scene::initialize()
	{
		HARMONY_DEBUG("Scene {} initialize() called", sceneId);
		
		try {
			std::lock_guard<std::mutex> lock(registryMutex_);
			registry_.clear();

			initializeEntities();
			initializeComponents();

			if (!containsGlobalComponent<Components::View3D>()) {
				HARMONY_DEBUG("Scene {} does not have View3D global component, creating default one", sceneId);
				createGlobalComponent<Components::View3D, Components::View3D>(Configuration(), *this);
			}

			if (containsGlobalComponent<Components::Script>()) {
				getGlobalComponent<Components::Script>().onCreate();
			}
			
			HARMONY_DEBUG("Scene {} initialization complete", sceneId);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Scene {} initialization: {}", sceneId, e.what());
			throw Exceptions::SceneOperationException("initialize", e.what());
		}
	}

	void Scene::createComponent(const std::string& componentName, const Configuration& configuration, EntityID entity) {
		HARMONY_ASSERT(!componentName.empty(), "Component name cannot be empty");
		HARMONY_TRACE("Creating component '{}' for entity {} in Scene {}", componentName, entity, sceneId);
		
		try {
			Harmony::Internals::ComponentsHandler::createComponent(componentName, configuration, entity, *this);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create component '{}' for entity {} in Scene {}: {}", 
				componentName, entity, sceneId, e.what());
			throw;
		}
	}

	void Scene::deleteComponent(const std::string& componentName, EntityID entityId) {
		HARMONY_ASSERT(!componentName.empty(), "Component name cannot be empty");
		HARMONY_TRACE("Deleting component '{}' from entity {} in Scene {}", componentName, entityId, sceneId);
		
		try {
			Harmony::Internals::ComponentsHandler::deleteComponent(componentName, entityId, *this);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to delete component '{}' from entity {} in Scene {}: {}", 
				componentName, entityId, sceneId, e.what());
			throw;
		}
	}

	bool Scene::containsComponent(const std::string& componentName, EntityID entityId) const {
		HARMONY_ASSERT(!componentName.empty(), "Component name cannot be empty");
		return Harmony::Internals::ComponentsHandler::containsComponent(componentName, entityId, *this);
	}

	bool Scene::containsGlobalComponent(const std::string& componentName) const
	{
		HARMONY_ASSERT(!componentName.empty(), "Component name cannot be empty");
		return Harmony::Internals::ComponentsHandler::containsComponent(componentName, entt::null, *this);
	}

	auto Scene::getView() {
		std::lock_guard<std::mutex> lock(registryMutex_);
		return registry_.view<entt::entity>();
	}

	void Scene::createGlobalComponent(const std::string& componentName, const Configuration& configuration) {
		HARMONY_ASSERT(!componentName.empty(), "Component name cannot be empty");
		HARMONY_TRACE("Creating global component '{}' in Scene {}", componentName, sceneId);
		
		try {
			Harmony::Internals::ComponentsHandler::createComponent(componentName, configuration, entt::null, *this);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create global component '{}' in Scene {}: {}", 
				componentName, sceneId, e.what());
			throw;
		}
	}

	void Scene::deleteGlobalComponent(const std::string& componentName) {
		HARMONY_ASSERT(!componentName.empty(), "Component name cannot be empty");
		HARMONY_TRACE("Deleting global component '{}' from Scene {}", componentName, sceneId);
		
		try {
			Harmony::Internals::ComponentsHandler::deleteComponent(componentName, entt::null, *this);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to delete global component '{}' from Scene {}: {}", 
				componentName, sceneId, e.what());
			throw;
		}
	}

	void Scene::createEntities(const EntityIDs entities)
	{
		HARMONY_DEBUG("Creating {} entities in Scene {}", entities.size(), sceneId);
		
		for (const EntityID entityId : entities) {
			try {
				createEntity(entityId);
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Failed to create entity {} in Scene {}: {}", entityId, sceneId, e.what());
				// Continue with other entities even if one fails
			}
		}
	}

	void Scene::createEntities(const Configuration& configuration)
	{
		std::optional<EntityIDs> entitiesIds = configuration.get<EntityIDs>({});
		if (entitiesIds.has_value()) {
			createEntities(entitiesIds.value());
		} else {
			HARMONY_TRACE("No entities found in configuration for Scene {}", sceneId);
		}
	}

	EntityID Scene::createEntity(const Configuration& configuration)
	{
		try {
			std::lock_guard<std::mutex> lock(registryMutex_);
			const EntityID entity = static_cast<EntityID>(registry_.create());

			for (const std::string& componentName : configuration.extractKeys({ "components" })) {
				auto componentConfig = configuration.subsection({ "components", componentName });
				if (componentConfig.has_value()) {
					ComponentsHandler::createComponent(componentName, componentConfig.value(), entity, *this);
				}
			}

			if (containsComponent<Components::Script>(entity)) {
				getComponent<Components::Script>(entity).onCreate();
			}

			HARMONY_DEBUG("Entity {} created in Scene {}", entity, sceneId);
			return static_cast<EntityID>(entity);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to create entity in Scene {}: {}", sceneId, e.what());
			throw Exceptions::SceneOperationException("createEntity", e.what());
		}
	}

	EntityID Scene::createEntity(const EntityID entityId)
	{
		const std::string key = std::to_string(static_cast<uint32_t>(entityId));

		if (std::optional<Configuration> entityConfiguration = engine.configuration->subsection({ "entities", key })) {
			return createEntity(entityConfiguration.value());
		}

		HARMONY_WARN("No configuration found for entity {} in Scene {}", entityId, sceneId);
		return entt::null;
	}

	void Scene::destroyEntity(EntityID entityId)
	{
		HARMONY_ASSERT(entityId != entt::null, "Cannot destroy null entity");
		
		try {
			std::lock_guard<std::mutex> lock(registryMutex_);
			
			if (!registry_.valid(static_cast<entt::entity>(entityId))) {
				HARMONY_WARN("Attempted to destroy invalid entity {} in Scene {}", entityId, sceneId);
				throw Exceptions::InvalidEntityException(entityId, "Entity is not valid");
			}
			
			if (containsComponent<Components::Script>(entityId)) {
				getComponent<Components::Script>(entityId).onDestroy();
			}

			registry_.destroy(static_cast<entt::entity>(entityId));
			HARMONY_DEBUG("Entity {} destroyed in Scene {}", entityId, sceneId);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to destroy entity {} in Scene {}: {}", entityId, sceneId, e.what());
			throw Exceptions::SceneOperationException("destroyEntity", e.what());
		}
	}

	void Scene::render()
	{
		try {
			const bool sceneContainsScript = containsGlobalComponent<Components::Script>();
			const auto view = getComponentsView<Components::Renderable>();
			
			if (sceneContainsScript) { 
				getGlobalComponent<Components::Script>().onPreRender(); 
			}
			
			BeginMode3D(getGlobalComponent<Components::View3D>());

			for (auto [entity, renderable] : view.each()) {
				HARMONY_ASSERT_NOT_NULL(renderable.get() != nullptr, "Renderable component pointer is null");
				renderable->preRender();
				renderable->onRender();
				renderable->postRender();
			}

			EndMode3D();
			
			if (sceneContainsScript) { 
				getGlobalComponent<Components::Script>().onPostRender(); 
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Scene {} render: {}", sceneId, e.what());
			// Don't rethrow to prevent render loop crash
		}
	}

	void Scene::update()
	{
		try {
			const bool containsScript = containsGlobalComponent<Components::Script>();
			if (containsScript) { 
				getGlobalComponent<Components::Script>().onPreUpdate(); 
			}
			if (containsScript) { 
				getGlobalComponent<Components::Script>().onPostUpdate(); 
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Scene {} update: {}", sceneId, e.what());
			// Don't rethrow to prevent update loop crash
		}
	}

	void Scene::initializeComponents()
	{
		HARMONY_TRACE("Initializing components for Scene {}", sceneId);
		
		Configuration componentsConfiguration = configuration_.subsection({ "components" }).value_or(Configuration());
		for (const std::string& componentName : componentsConfiguration.extractKeys({})) {
			try {
				auto componentConfig = componentsConfiguration.subsection({ componentName });
				if (componentConfig.has_value()) {
					createGlobalComponent(componentName, componentConfig.value());
				}
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Failed to initialize component '{}' in Scene {}: {}", 
					componentName, sceneId, e.what());
				// Continue with other components
			}
		}
	}

	void Scene::initializeEntities()
	{
		HARMONY_TRACE("Initializing entities for Scene {}", sceneId);
		
		Configuration entitiesConfiguration = configuration_.subsection({ "entities" }).value_or(Configuration());
		createEntities(entitiesConfiguration);
	}

	bool Scene::isActiveRendering() const {
		return activeRender.load();
	}

	bool Scene::isActiveUpdating() const {
		return activeUpdate.load();
	}

	void Scene::setActiveRendering(bool active) {
		activeRender.store(active);
		HARMONY_TRACE("Scene {} rendering active state set to {}", sceneId, active);
	}

	void Scene::setActiveUpdating(bool active) {
		activeUpdate.store(active);
		HARMONY_TRACE("Scene {} updating active state set to {}", sceneId, active);
	}

	int Scene::getUpdateOrder() const {
		return updateOrder_.load();
	}

	int Scene::getRenderOrder() const {
		return renderOrder_.load();
	}

	void Scene::setUpdateOrder(int order) {
		updateOrder_.store(order);
		HARMONY_TRACE("Scene {} update order set to {}", sceneId, order);
	}

	void Scene::setRenderOrder(int order) {
		renderOrder_.store(order);
		HARMONY_TRACE("Scene {} render order set to {}", sceneId, order);
	}
}