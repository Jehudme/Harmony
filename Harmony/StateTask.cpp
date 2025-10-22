#include "pch.h"
#include "StateTask.h"
#include "Engine.h"
#include "StateManagement.h"

namespace Harmony::Tasks
{
	PushState::PushState(const Utilities::UUID stateId) :
		Task(100, FastMultiThreaded, std::chrono::milliseconds(0)), stateId_(stateId) {}

	void PushState::run()
	{
		getEngine().stateManagement->push(stateId_);
	}

	PopState::PopState() :
		Task(100, FastMultiThreaded, std::chrono::milliseconds(0)) {}

	void PopState::run()
	{
		getEngine().stateManagement->pop();
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
}
