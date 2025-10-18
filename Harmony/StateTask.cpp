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

}
