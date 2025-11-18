#include "pch.h"
#include "StateStack.h"
#include "Engine.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Internals {

	StateStack::StateStack(Engine& engine, const Configuration& configuration)
		: engine_(engine), configuration_(configuration) {
		HARMONY_INFO("Initializing StateStack");

		try {
			HARMONY_ASSERT_NOT_NULL(&engine, "Engine reference cannot be null");
			HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration reference cannot be null");

			HARMONY_DEBUG("StateStack initialized successfully");
		}
		catch (const Exceptions::HarmonyException& e) {
			HARMONY_CRITICAL("StateStack initialization failed with HarmonyException: {}", e.what());
			throw Exceptions::StateStackOperationException("initialization", e.what());
		}
		catch (const std::exception& e) {
			HARMONY_CRITICAL("StateStack initialization failed with exception: {}", e.what());
			throw Exceptions::StateStackOperationException("initialization", e.what());
		}

		HARMONY_INFO("StateStack initialized successfully");
	}

	StateStack::~StateStack() {
		HARMONY_INFO("Destroying StateStack");
		std::lock_guard lock(mutex_);
		
		// Clear all states
		HARMONY_DEBUG("Clearing {} states from stack", states_.size());
		while (!states_.empty()) {
			auto& state = states_.top();
			if (state) {
				state->onExit();
			}
			states_.pop();
		}
	}

	void StateStack::push(std::unique_ptr<State> state) {
		std::lock_guard lock(mutex_);

		HARMONY_ASSERT_NOT_NULL(state.get(), "Cannot push null state onto stack");
		
		if (!state) {
			HARMONY_ERROR("Attempted to push null state onto StateStack");
			throw Exceptions::NullStateException("push");
		}

		try {
			// Pause the current top state if it exists
			if (!states_.empty() && states_.top()) {
				HARMONY_DEBUG("Pausing current state: '{}'", states_.top()->getName());
				states_.top()->onPause();
			}

			// Push the new state
			std::string stateName = state->getName();
			states_.push(std::move(state));
			
			// Enter the new state
			HARMONY_INFO("Pushing state '{}' onto stack (stack size: {})", stateName, states_.size());
			states_.top()->onEnter();
		}
		catch (const Exceptions::StateException& e) {
			HARMONY_ERROR("Failed to push state onto stack: {}", e.what());
			throw Exceptions::StateStackOperationException("push", e.what());
		}
	}

	std::unique_ptr<State> StateStack::pop() {
		std::lock_guard lock(mutex_);

		if (states_.empty()) {
			HARMONY_WARN("Attempted to pop from empty StateStack");
			throw Exceptions::EmptyStateStackException();
		}

		try {
			// Get and exit the current state
			auto state = std::move(states_.top());
			states_.pop();

			std::string stateName = state ? state->getName() : "null";
			HARMONY_INFO("Popping state '{}' from stack (stack size: {})", stateName, states_.size());

			if (state) {
				state->onExit();
			}

			// Resume the new top state if it exists
			if (!states_.empty() && states_.top()) {
				HARMONY_DEBUG("Resuming state: '{}'", states_.top()->getName());
				states_.top()->onResume();
			}

			return state;
		}
		catch (const Exceptions::StateException& e) {
			HARMONY_ERROR("Failed to pop state from stack: {}", e.what());
			throw Exceptions::StateStackOperationException("pop", e.what());
		}
	}

	State* StateStack::peek() const {
		std::lock_guard lock(mutex_);

		if (states_.empty()) {
			HARMONY_TRACE("StateStack is empty, peek returns nullptr");
			return nullptr;
		}

		HARMONY_TRACE("Peeking at top state: '{}'", states_.top()->getName());
		return states_.top().get();
	}

	void StateStack::clear() {
		std::lock_guard lock(mutex_);

		HARMONY_INFO("Clearing StateStack ({} states)", states_.size());

		try {
			while (!states_.empty()) {
				auto& state = states_.top();
				if (state) {
					HARMONY_DEBUG("Exiting and removing state: '{}'", state->getName());
					state->onExit();
				}
				states_.pop();
			}

			HARMONY_INFO("StateStack cleared successfully");
		}
		catch (const Exceptions::StateException& e) {
			HARMONY_ERROR("Error during StateStack clear: {}", e.what());
			throw Exceptions::StateStackOperationException("clear", e.what());
		}
	}

	std::size_t StateStack::size() const {
		std::lock_guard lock(mutex_);
		return states_.size();
	}

	bool StateStack::isEmpty() const {
		std::lock_guard lock(mutex_);
		return states_.empty();
	}

	void StateStack::update(float deltaTime) {
		std::lock_guard lock(mutex_);

		if (!states_.empty() && states_.top()) {
			HARMONY_TRACE("Updating current state: '{}'", states_.top()->getName());
			states_.top()->update(deltaTime);
		}
	}

	void StateStack::render() {
		std::lock_guard lock(mutex_);

		if (!states_.empty() && states_.top()) {
			HARMONY_TRACE("Rendering current state: '{}'", states_.top()->getName());
			states_.top()->render();
		}
	}

	void StateStack::handleEvents() {
		std::lock_guard lock(mutex_);

		if (!states_.empty() && states_.top()) {
			HARMONY_TRACE("Handling events for current state: '{}'", states_.top()->getName());
			states_.top()->handleEvents();
		}
	}

} // namespace Harmony::Internals
