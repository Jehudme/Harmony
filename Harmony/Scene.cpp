#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Internals {

	Scene::Scene(Engine& engine, const Configuration& configuration, const std::string& name)
		: engine_(engine),
		  configuration_(configuration),
		  name_(name),
		  active_(false),
		  drawOrder_(0) {
		HARMONY_INFO("Initializing Scene: '{}'", name_);

		try {
			HARMONY_ASSERT_NOT_NULL(&engine, "Engine reference cannot be null");
			HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration reference cannot be null");
			HARMONY_ASSERT(!name.empty(), "Scene name cannot be empty");

			// Try to get draw order from configuration
			auto drawOrderOpt = configuration_.get<int>({ "drawOrder" });
			if (drawOrderOpt.has_value()) {
				drawOrder_ = drawOrderOpt.value();
				HARMONY_DEBUG("Scene '{}' draw order set to: {}", name_, drawOrder_);
			}

			HARMONY_DEBUG("Scene '{}' initialized successfully", name_);
		}
		catch (const Exceptions::HarmonyException& e) {
			HARMONY_CRITICAL("Scene '{}' initialization failed with HarmonyException: {}", name_, e.what());
			throw Exceptions::SceneInitializationException(name_, e.what());
		}
		catch (const std::exception& e) {
			HARMONY_CRITICAL("Scene '{}' initialization failed with exception: {}", name_, e.what());
			throw Exceptions::SceneInitializationException(name_, e.what());
		}
	}

	Scene::~Scene() {
		HARMONY_INFO("Destroying Scene: '{}'", name_);
	}

	void Scene::initialize() {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("Scene '{}' initializing", name_);
		active_ = true;
		// Base implementation - override in derived classes
	}

	void Scene::cleanup() {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("Scene '{}' cleaning up", name_);
		active_ = false;
		// Base implementation - override in derived classes
	}

	void Scene::handleResources(float deltaTime) {
		std::lock_guard lock(mutex_);
		HARMONY_TRACE("Scene '{}' handleResources - deltaTime: {}", name_, deltaTime);
		// Base implementation does nothing - override in derived classes
	}

	void Scene::render() {
		std::lock_guard lock(mutex_);
		HARMONY_TRACE("Scene '{}' render", name_);
		// Base implementation does nothing - override in derived classes
	}

	void Scene::handleEvents() {
		std::lock_guard lock(mutex_);
		HARMONY_TRACE("Scene '{}' handling events", name_);
		// Base implementation does nothing - override in derived classes
	}

	const std::string& Scene::getName() const {
		std::lock_guard lock(mutex_);
		return name_;
	}

	bool Scene::isActive() const {
		std::lock_guard lock(mutex_);
		return active_;
	}

	void Scene::setActive(bool active) {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("Scene '{}' active status changed to: {}", name_, active);
		active_ = active;
	}

	int Scene::getDrawOrder() const {
		std::lock_guard lock(mutex_);
		return drawOrder_;
	}

	void Scene::setDrawOrder(int order) {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("Scene '{}' draw order changed from {} to {}", name_, drawOrder_, order);
		drawOrder_ = order;
	}

	Engine& Scene::getEngine() {
		return engine_;
	}

	const Configuration& Scene::getConfiguration() const {
		return configuration_;
	}

} // namespace Harmony::Internals
