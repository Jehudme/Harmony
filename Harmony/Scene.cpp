#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "Logger.h"
#include "Configuration.h"
#include "ComponentsHandler.h"
#include "Script.h"
#include "View3D.h"
#include "Renderable.h"

namespace Harmony::Internals
{
	Scene::Scene(Engine& engine, const SceneID sceneId) :
		sceneId(sceneId),
		engine(engine) 
	{
		const std::string key = std::to_string(static_cast<uint32_t>(sceneId));
		configuration_ = engine.configuration->subsection({ "scenes", key }).value();

		initialize();
	}

	Scene::~Scene() {
		if (containsGlobalComponent<Components::Script>()) {
			getGlobalComponent<Components::Script>().onDestroy();
		}

		auto view = registry_.view<entt::entity>();
		std::vector<entt::entity> entities(view.begin(), view.end());
		for (entt::entity entity : entities) {
			destroyEntity(static_cast<EntityID>(entity));
		}
	}

	void Scene::initialize()
	{
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
	}

	void Scene::createComponent(const std::string& componentName, const Configuration& configuration, EntityID entity) {
		Harmony::Internals::ComponentsHandler::createComponent(componentName, configuration, entity, *this);
	}

	void Scene::deleteComponent(const std::string& componentName, EntityID entityId) {
		Harmony::Internals::ComponentsHandler::deleteComponent(componentName, entityId, *this);
	}

	bool Scene::containsComponent(const std::string& componentName, EntityID entityId) const {
		return Harmony::Internals::ComponentsHandler::containsComponent(componentName, entityId, *this);
	}

	bool Scene::containsGlobalComponent(const std::string& componentName) const
	{
		return Harmony::Internals::ComponentsHandler::containsComponent(componentName, entt::null, *this);

	}

	auto Scene::getView() {
		return registry_.view<entt::entity>();
	}

	void Scene::createGlobalComponent(const std::string& componentName, const Configuration& configuration) {
		Harmony::Internals::ComponentsHandler::createComponent(componentName, configuration, entt::null, *this);
	}

	void Scene::deleteGlobalComponent(const std::string& componentName) {
		Harmony::Internals::ComponentsHandler::deleteComponent(componentName, entt::null, *this);
	}

	void Scene::createEntities(const EntityIDs entities)
	{
		Configuration entitiesConfiguration;

		for (const EntityID entityId : entities) {
			createEntity(entityId);
		}
	}

	void Scene::createEntities(const Configuration& configuration)
	{
		std::optional<EntityIDs> entitiesIds = configuration.get<EntityIDs>({});
		if (entitiesIds.has_value()) {
			createEntities(entitiesIds.value());
		}
	}

	EntityID Scene::createEntity(const Configuration& configuration)
	{
		const EntityID entity = static_cast<EntityID>(registry_.create());

		for (const std::string& componentName : configuration.extractKeys({ "components" }))
			ComponentsHandler::createComponent(componentName, configuration.subsection({ "components", componentName }).value(), entity, *this);

		if (containsComponent<Components::Script>(entity))
			getComponent<Components::Script>(entity).onCreate();

		HARMONY_DEBUG("Entity {} created", entity);
		return static_cast<EntityID>(entity);
	}

	EntityID Scene::createEntity(const EntityID entityId)
	{
		const std::string key = std::to_string(static_cast<uint32_t>(entityId));

		if (std::optional<Configuration> entityConfiguration = engine.configuration->subsection({ "entities", key })) {
			return createEntity(entityConfiguration.value());
		}

		return entt::null;
	}

	void Scene::destroyEntity(EntityID entityId)
	{
		if (containsComponent<Components::Script>(entityId))
			getComponent<Components::Script>(entityId).onDestroy();

		registry_.destroy(static_cast<entt::entity>(entityId));
		HARMONY_DEBUG("Entity {} destroyed", entityId);
	}

	void Scene::render()
	{
		const bool sceneContainsScript = containsGlobalComponent<Components::Script>();
		const auto view = getComponentsView<Components::Renderable>();
		const bool containsScript = containsGlobalComponent<Components::Script>();
		
		if (sceneContainsScript) { getGlobalComponent<Components::Script>().onPreRender(); }
		BeginMode3D(getGlobalComponent<Components::View3D>());

		for (auto [entity, renderable] : view.each()) {
			renderable->preRender();
			renderable->onRender();
			renderable->postRender();
		}

		EndMode3D();
		if (sceneContainsScript) { getGlobalComponent<Components::Script>().onPostRender(); }
	}

	void Scene::update()
	{
		const bool containsScript = containsGlobalComponent<Components::Script>();
		if (containsScript) { getGlobalComponent<Components::Script>().onPreUpdate(); }
		if (containsScript) { getGlobalComponent<Components::Script>().onPostUpdate(); }
	}

	void Scene::initializeComponents()
	{
		Configuration componentsConfiguration = configuration_.subsection({ "components" }).value_or(Configuration());
		for (const std::string& componentName : componentsConfiguration.extractKeys({})) {
			createGlobalComponent(componentName, componentsConfiguration.subsection({ componentName }).value());
		}
	}

	void Scene::initializeEntities()
	{
		Configuration entitiesConfiguration = configuration_.subsection({ "entities" }).value_or(Configuration());
		createEntities(entitiesConfiguration);
	}

	bool Scene::isActiveRendering() const {
		return activeUpdate;
	}

	bool Scene::isActiveUpdating() const {
		return activeUpdate;
	}

	void Scene::setActiveRendering(bool active) {
		activeRender = active;
	}

	void Scene::setActiveUpdating(bool active) {
		activeUpdate = active;
	}

	int Scene::getUpdateOrder() const {
		return updateOrder_;
	}

	int Scene::getRenderOrder() const {
		return renderOrder_;
	}

	void Scene::setUpdateOrder(int order) {
		updateOrder_ = order;
	}

	void Scene::setRenderOrder(int order) {
		renderOrder_ = order;
	}
}