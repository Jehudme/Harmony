#include "pch.h"
#include "StateTask.h"
#include "Engine.h"
#include "StateManagement.h"
#include <thread>

namespace Harmony::Tasks
{
	PushState::PushState(const Utilities::UUID stateId) :
		Task(100, FastMultiThreaded, std::chrono::milliseconds(0)), stateId_(stateId) {}

	void PushState::run()
	{
		try {
			getEngine().stateManagement->push(stateId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to push state {}: {}", stateId_, e.what());
			throw;
		}
	}

	PopState::PopState() :
		Task(100, FastMultiThreaded, std::chrono::milliseconds(0)) {}

	void PopState::run()
	{
		try {
			getEngine().stateManagement->pop();
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to pop state: {}", e.what());
			throw;
		}
	}

	// ClearStatesTask implementation
	ClearStatesTask::ClearStatesTask() :
		Task(150, FastMultiThreaded) {}

	void ClearStatesTask::run()
	{
		// Pop all states until empty
		try {
			while (true) {
				getEngine().stateManagement->pop();
			}
		}
		catch (const std::exception&) {
			// State stack is now empty, which is expected
			HARMONY_INFO("All states cleared from stack");
		}
	}

	// SwitchStateTask implementation
	SwitchStateTask::SwitchStateTask(const Utilities::UUID newStateId) :
		Task(100, FastMultiThreaded), newStateId_(newStateId) {}

	void SwitchStateTask::run()
	{
		try {
			getEngine().stateManagement->pop();
			getEngine().stateManagement->push(newStateId_);
			HARMONY_INFO("Switched to state {}", newStateId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to switch state: {}", e.what());
		}
	}

	// TransitionToStateTask implementation
	TransitionToStateTask::TransitionToStateTask(const Utilities::UUID newStateId,
		std::chrono::milliseconds transitionDelay,
		std::function<void(Engine&)> transitionEffect) :
		Task(100, SlowMultiThreaded), newStateId_(newStateId), 
		transitionDelay_(transitionDelay), transitionEffect_(transitionEffect) {}

	void TransitionToStateTask::run()
	{
		try {
			HARMONY_INFO("Starting state transition to {}", newStateId_);

			// Execute transition effect if provided
			if (transitionEffect_) {
				HARMONY_DEBUG("Executing transition effect");
				transitionEffect_(getEngine());
			}

			// Wait for transition delay
			if (transitionDelay_.count() > 0) {
				HARMONY_DEBUG("Transition delay: {}ms", transitionDelay_.count());
				std::this_thread::sleep_for(transitionDelay_);
			}

			// Pop current state and push new one
			getEngine().stateManagement->pop();
			getEngine().stateManagement->push(newStateId_);
			
			HARMONY_INFO("State transition to {} completed", newStateId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("State transition to {} failed: {}", newStateId_, e.what());
		}
	}

	// ReplaceStateTask implementation
	ReplaceStateTask::ReplaceStateTask(const Utilities::UUID newStateId) :
		Task(100, FastMultiThreaded), newStateId_(newStateId) {}

	void ReplaceStateTask::run()
	{
		try {
			// Pop the current state
			getEngine().stateManagement->pop();
			
			// Push the new state
			getEngine().stateManagement->push(newStateId_);
			
			HARMONY_INFO("Replaced state with {}", newStateId_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to replace state with {}: {}", newStateId_, e.what());
		}
	}

	// PeekStateTask implementation
	PeekStateTask::PeekStateTask(std::function<void(Utilities::UUID)> callback) :
		Task(0, FastMultiThreaded), callback_(callback) {}

	void PeekStateTask::run()
	{
		if (!callback_) {
			HARMONY_WARN("Peek state task has no callback");
			return;
		}

		try {
			auto currentState = getEngine().stateManagement->getCurrentState();
			
			if (currentState) {
				// Note: State doesn't have a public ID, so we'll return 0 as placeholder
				// In a real implementation, State would need to expose its ID
				Utilities::UUID stateId = 0;
				HARMONY_DEBUG("Peeked at current state");
				callback_(stateId);
			}
			else {
				HARMONY_WARN("No current state to peek at");
				callback_(0);
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to peek at state: {}", e.what());
			callback_(0);
		}
	}
}
