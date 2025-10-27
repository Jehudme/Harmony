#include "pch.h"
#include "StateMachine.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include <unordered_map>

HARMONY_REGISTER_COMPONENT(Harmony::Components::StateMachine, StateMachine)

namespace Harmony::Components
{
	struct StateInfo {
		std::string name;
		StateMachine::StateCallback onEnter;
		StateMachine::StateCallback onExit;
		StateMachine::StateCallback onUpdate;
		std::unordered_map<std::string, std::string> data;
	};

	struct StateMachineImpl {
		std::unordered_map<std::string, StateInfo> states;
		std::string currentState;
		std::string previousState;
	};

	StateMachine::StateMachine(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<StateMachineImpl>())
	{
		try {
			// Load initial states from configuration
			auto statesConfig = configuration.subsection({ "states" });
			if (statesConfig) {
				auto stateKeys = statesConfig->extractKeys({});
				for (const auto& stateName : stateKeys) {
					addState(stateName);
				}
			}

			// Set initial state
			std::string initialState = configuration.get<std::string>({ "initial_state" }).value_or("");
			if (!initialState.empty() && hasState(initialState)) {
				changeState(initialState);
			}

			HARMONY_DEBUG("StateMachine component initialized with {} states", impl_->states.size());
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize StateMachine component: {}", e.what());
			throw;
		}
	}

	StateMachine::~StateMachine() = default;

	void StateMachine::addState(const std::string& stateName) {
		if (impl_->states.find(stateName) == impl_->states.end()) {
			StateInfo info;
			info.name = stateName;
			impl_->states[stateName] = info;
			HARMONY_DEBUG("Added state '{}' to StateMachine", stateName);
		}
		else {
			HARMONY_WARN("State '{}' already exists in StateMachine", stateName);
		}
	}

	void StateMachine::removeState(const std::string& stateName) {
		if (stateName == impl_->currentState) {
			HARMONY_WARN("Cannot remove current state '{}'", stateName);
			return;
		}

		auto it = impl_->states.find(stateName);
		if (it != impl_->states.end()) {
			impl_->states.erase(it);
			HARMONY_DEBUG("Removed state '{}' from StateMachine", stateName);
		}
		else {
			HARMONY_WARN("State '{}' not found in StateMachine", stateName);
		}
	}

	bool StateMachine::hasState(const std::string& stateName) const {
		return impl_->states.find(stateName) != impl_->states.end();
	}

	void StateMachine::changeState(const std::string& stateName) {
		if (!hasState(stateName)) {
			HARMONY_ERROR("Cannot change to non-existent state '{}'", stateName);
			return;
		}

		// Exit current state
		if (!impl_->currentState.empty()) {
			auto& currentStateInfo = impl_->states[impl_->currentState];
			if (currentStateInfo.onExit) {
				try {
					currentStateInfo.onExit();
				}
				catch (const std::exception& e) {
					HARMONY_ERROR("State '{}' exit callback failed: {}", impl_->currentState, e.what());
				}
			}
		}

		impl_->previousState = impl_->currentState;
		impl_->currentState = stateName;

		// Enter new state
		auto& newStateInfo = impl_->states[stateName];
		if (newStateInfo.onEnter) {
			try {
				newStateInfo.onEnter();
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("State '{}' enter callback failed: {}", stateName, e.what());
			}
		}

		HARMONY_DEBUG("StateMachine changed state from '{}' to '{}'", impl_->previousState, stateName);
	}

	std::string StateMachine::getCurrentState() const {
		return impl_->currentState;
	}

	std::string StateMachine::getPreviousState() const {
		return impl_->previousState;
	}

	void StateMachine::setOnStateEnter(const std::string& stateName, StateCallback callback) {
		if (hasState(stateName)) {
			impl_->states[stateName].onEnter = callback;
		}
		else {
			HARMONY_WARN("Cannot set enter callback for non-existent state '{}'", stateName);
		}
	}

	void StateMachine::setOnStateExit(const std::string& stateName, StateCallback callback) {
		if (hasState(stateName)) {
			impl_->states[stateName].onExit = callback;
		}
		else {
			HARMONY_WARN("Cannot set exit callback for non-existent state '{}'", stateName);
		}
	}

	void StateMachine::setOnStateUpdate(const std::string& stateName, StateCallback callback) {
		if (hasState(stateName)) {
			impl_->states[stateName].onUpdate = callback;
		}
		else {
			HARMONY_WARN("Cannot set update callback for non-existent state '{}'", stateName);
		}
	}

	void StateMachine::update(float deltaTime) {
		if (impl_->currentState.empty()) return;

		auto& currentStateInfo = impl_->states[impl_->currentState];
		if (currentStateInfo.onUpdate) {
			try {
				currentStateInfo.onUpdate();
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("State '{}' update callback failed: {}", impl_->currentState, e.what());
			}
		}
	}

	void StateMachine::setStateData(const std::string& stateName, const std::string& key, const std::string& value) {
		if (hasState(stateName)) {
			impl_->states[stateName].data[key] = value;
		}
		else {
			HARMONY_WARN("Cannot set data for non-existent state '{}'", stateName);
		}
	}

	std::string StateMachine::getStateData(const std::string& stateName, const std::string& key) const {
		if (hasState(stateName)) {
			auto& stateData = impl_->states.at(stateName).data;
			auto it = stateData.find(key);
			if (it != stateData.end()) {
				return it->second;
			}
		}
		return "";
	}
}
