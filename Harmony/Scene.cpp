#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "Logger.h"
#include "Configuration.h"
#include "ComponentsHandler.h"


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

	Scene::~Scene() = default;

	void Scene::initialize()
	{
		registry_.clear();

		Configuration entitiesConfiguration = configuration_.subsection({ "entities" }).value_or(Configuration());
		createEntities(entitiesConfiguration);
	}

	void Scene::createComponent(const std::string& componentName, const Configuration& configuration, EntityID entity) {
		Harmony::Internals::ComponentsHandler::createComponent(componentName, configuration, entity, *this);
	}

	void Scene::deleteComponent(const std::string& componentName, EntityID entityId) {
		Harmony::Internals::ComponentsHandler::deleteComponent(componentName, entityId, *this);
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
		registry_.destroy(static_cast<entt::entity>(entityId));
		HARMONY_DEBUG("Entity {} destroyed", entityId);
	}

	void Scene::render()
	{
	}

	void Scene::update()
	{
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