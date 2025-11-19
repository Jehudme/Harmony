#include "pch.h"
#include "State.h"
#include "Engine.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Internals {

	State::State(Engine& engine, const Configuration& configuration, const std::string& name)
		: engine_(engine),
		  configuration_(configuration),
		  name_(name),
		  active_(false) {
		HARMONY_INFO("Initializing State: '{}'", name_);

		try {
			HARMONY_ASSERT_NOT_NULL(&engine, "Engine reference cannot be null");
			HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration reference cannot be null");
			HARMONY_ASSERT(!name.empty(), "State name cannot be empty");

			HARMONY_DEBUG("State '{}' initialized successfully", name_);
		}
		catch (const Exceptions::HarmonyException& e) {
			HARMONY_CRITICAL("State '{}' initialization failed with HarmonyException: {}", name_, e.what());
			throw Exceptions::StateInitializationException(name_, e.what());
		}
		catch (const std::exception& e) {
			HARMONY_CRITICAL("State '{}' initialization failed with exception: {}", name_, e.what());
			throw Exceptions::StateInitializationException(name_, e.what());
		}
	}

	State::~State() {
		HARMONY_INFO("Destroying State: '{}'", name_);
	}

	void State::onEnter() {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("State '{}' entering", name_);
		active_ = true;
	}

	void State::onExit() {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("State '{}' exiting", name_);
		active_ = false;
	}

	void State::onPause() {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("State '{}' paused", name_);
		active_ = false;
	}

	void State::onResume() {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("State '{}' resumed", name_);
		active_ = true;
	}

	void State::update(float deltaTime) {
		std::lock_guard lock(mutex_);
		HARMONY_TRACE("State '{}' update - deltaTime: {}", name_, deltaTime);
		// Base implementation does nothing - override in derived classes
	}

	void State::render() {
		std::lock_guard lock(mutex_);
		HARMONY_TRACE("State '{}' render", name_);
		// Base implementation does nothing - override in derived classes
	}

	void State::handleEvents() {
		std::lock_guard lock(mutex_);
		HARMONY_TRACE("State '{}' handling events", name_);
		// Base implementation does nothing - override in derived classes
	}

	const std::string& State::getName() const {
		std::lock_guard lock(mutex_);
		return name_;
	}

	bool State::isActive() const {
		std::lock_guard lock(mutex_);
		return active_;
	}

	void State::setActive(bool active) {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("State '{}' active status changed to: {}", name_, active);
		active_ = active;
	}

	Engine& State::getEngine() {
		return engine_;
	}

	const Configuration& State::getConfiguration() const {
		return configuration_;
	}

} // namespace Harmony::Internals
