#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include "Transform.h"
#include "Script.h"
#include "View.h"
#include "Node.h"
#include "Configuration.h"
#include "RenderManager.h"
#include "PhysicsWorld.h"
#include "PhysicsBody.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

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
		onConstructConnection_(impl_->registry.on_construct<entt::entity>().connect<&EntityCounter::increment>(entityCounter.get())),
		onDestroyConnection_(impl_->registry.on_destroy<entt::entity>().connect<&EntityCounter::decrement>(entityCounter.get())),
		entityCounter(std::make_unique<EntityCounter>()),
		impl_(std::make_unique<SceneImpl>()),
		configuration_(configuration),
		sceneId(sceneId),
		engine(engine) {
		initialize();
	}

	void Scene::initialize()
	{
		// Clear existing entities if any
		for (auto entity : impl_->registry.view<EntityID>()) {
			impl_->registry.destroy(static_cast<EntityID>(entity));
		}

		// Initialize global components if specified
		if (std::optional<Utilities::Configuration> globalComponentsConfig = configuration_.subsection({ "globalComponents" })) {
			std::vector<std::string> globalComponentNames = globalComponentsConfig->extractKeys({});
			for (const std::string& componentName : globalComponentNames) {
				Utilities::Configuration componentConfig = globalComponentsConfig->subsection({ componentName }).value();
				createGlobalComponent(componentName, componentConfig);
				HARMONY_DEBUG("Global component '{}' created for scene {}", componentName, sceneId);
			}
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
		for (auto entity : impl_->registry.view<EntityID>()) {
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

		for (const EntityID entity : entitiesView) {
			auto& drawable = entitiesView.get<std::unique_ptr<sf::Drawable>>(entity);
			sf::RenderStates entityStates;

			if (const auto& transform = impl_->registry.try_get<std::unique_ptr<Components::Transform>>(entity)) {
				const sf::Transformable* sfTransform = static_cast<const sf::Transformable*>(transform->get());
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

		for (const EntityID entity : entitiesView) {
			Components::Script& scriptComponent = getComponent<Components::Script>(static_cast<EntityID>(entity));
			scriptComponent.onPreUpdate();
		}

		// Physics integration: Sync Transform -> PhysicsBody before physics step
		if (auto* physicsWorldPtr = impl_->registry.ctx().find<std::unique_ptr<Components::PhysicsWorld>>()) {
			Components::PhysicsWorld& physicsWorld = **physicsWorldPtr;
			auto physicsView = impl_->registry.view<std::unique_ptr<Components::Transform>, std::unique_ptr<Components::PhysicsBody>>();
			
			// Before physics step: copy Transform data to PhysicsBody
			for (const EntityID entity : physicsView) {
				auto& transform = *physicsView.get<std::unique_ptr<Components::Transform>>(entity);
				auto& physicsBody = *physicsView.get<std::unique_ptr<Components::PhysicsBody>>(entity);
				
				// Get position and rotation from Transform (in pixels)
				// The transform position is where the origin is located in world space
				// Since fixtures are centered at the body origin and visual origins are typically
				// set to the shape center, the body position should match the transform position
				sf::Vector2f positionPixels = transform.getPosition();
				float rotation = transform.getRotation();

				// Convert position from pixels to meters for Box2D
				float positionMetersX = physicsWorld.pixelsToMeters(positionPixels.x);
				float positionMetersY = physicsWorld.pixelsToMeters(positionPixels.y);

				// Convert rotation from degrees to radians
				float angleRadians = rotation * 3.14159265359f / 180.0f;
				
				// Set the physics body transform (in meters and radians)
				b2Vec2 currentPos = physicsBody.getPosition();
				if (positionMetersX != currentPos.x || positionMetersY != currentPos.y || angleRadians != physicsBody.getAngle())
					physicsBody.setTransform(b2Vec2(positionMetersX, positionMetersY), angleRadians);
			}
			
			// Step the physics world
			physicsWorld.step(deltaTime, 60, 20);
			
			// After physics step: copy PhysicsBody data back to Transform
			for (const EntityID entity : physicsView) {
				auto& transform = *physicsView.get<std::unique_ptr<Components::Transform>>(entity);
				auto& physicsBody = *physicsView.get<std::unique_ptr<Components::PhysicsBody>>(entity);
				
				// Get position and angle from physics body (in meters and radians)
				b2Vec2 positionMeters = physicsBody.getPosition();
				float angleRadians = physicsBody.getAngle();
				
				// Convert position from meters to pixels for rendering
				float positionPixelsX = physicsWorld.metersToPixels(positionMeters.x);
				float positionPixelsY = physicsWorld.metersToPixels(positionMeters.y);
				
				// Convert angle from radians to degrees
				float angleDegrees = angleRadians * 180.0f / 3.14159265359f;
				
				// Update Transform with physics data
				// The body position is at the fixture center, which should match the transform origin location
				transform.setRotation(angleDegrees);
				transform.setPosition(positionPixelsX, positionPixelsY);
			}
		}

		for (const EntityID entity : entitiesView) {
			Components::Script& scriptComponent = getComponent<Components::Script>(static_cast<EntityID>(entity));
			scriptComponent.onPostUpdate();
		}
	}

	void Scene::createComponent(const std::string& componentName, const Utilities::Configuration& configuration, EntityID entity){
		Harmony::Management::ComponentManager::createComponent(componentName, configuration, entity, *this);
	}

	void Scene::deleteComponent(const std::string& componentName, EntityID entityId) {
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
		std::unique_lock<std::mutex> lock(entityMutex_);		
		const EntityID entity = impl_->registry.create();

		for (const std::string& componentName : configuration.extractKeys({ "components" }))
			Management::ComponentManager::createComponent(componentName, configuration.subsection({ "components", componentName}).value(), entity, *this);

		if (std::optional<std::string> scriptName = configuration.get<std::string>({ "script" })) {
			createComponent(scriptName.value(), configuration, entity);

			Components::Script& script = getComponent<Components::Script>(entity);
			script.entityId_ = entity; 
			script.scene_ = *this;
			script.onCreate();
		}

		Components::Node& node = createComponent<Components::Node>(entity, entity, *this);
		if (std::optional<Utilities::Configuration> childrenConfigurations = configuration.subsection({ "children" })) {
			lock.unlock(); // Unlock to allow recursive entity creation
			for (const std::string& childKey : childrenConfigurations->extractKeys({})) {
				EntityID entityId = createEntity(childrenConfigurations->subsection({ childKey }).value(), std::stoul(childKey));
				node.attach(entityId);
			}
		}
		else HARMONY_WARN("Entity {} has no children configuration", static_cast<std::uint32_t>(entity));


		HARMONY_DEBUG("Entity {} created", static_cast<std::uint32_t>(entity));
		return static_cast<EntityID>(entity);
	}

	EntityID Scene::createEntity(const Utilities::Configuration& configuration, Utilities::UUID premadeId)
	{
		Utilities::Configuration mergedConfiguration;
		engine.configuration.debugPrint();
		if (std::optional<Utilities::Configuration> premadeConfiguration = engine.configuration.subsection({ "entities", std::to_string(premadeId) }))
			mergedConfiguration.merge(premadeConfiguration.value());

		else HARMONY_WARN("Premade entity {} has no configuration defined in engine configuration", premadeId);

		mergedConfiguration.merge(configuration);
		return createEntity(mergedConfiguration);
	}

	void Scene::destroyEntity(EntityID entityId)
	{
		std::lock_guard<std::mutex> lock(entityMutex_);
		
		EntityID entity = static_cast<EntityID>(entityId);
		
		if (impl_->registry.try_get<Components::Script>(entity))
			getComponent<Components::Script>(entityId).onDestroy();

		impl_->registry.destroy(entity);

		HARMONY_DEBUG("Entity {} destroyed", static_cast<unsigned int>(entityId));
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
			throw Exceptions::ComponentNotFoundException(static_cast<EntityID>(NULL));  // View is not tied to a specific entity
		}
		return *view;
	}

	const Components::View& Scene::getView() const
	{
		const auto* view = impl_->registry.ctx().find<Components::View>();
		if (!view) {
			throw Exceptions::ComponentNotFoundException(static_cast<EntityID>(NULL));  // View is not tied to a specific entity
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

	void Scene::EntityCounter::increment() {
		count++;
	}
	void Scene::EntityCounter::decrement() {
		count--;
	}
	std::size_t Scene::EntityCounter::getCount() const {
		return count;
	}
}
