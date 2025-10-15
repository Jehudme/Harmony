#include "pch.h"
#include "StateTask.h"
#include "Engine.h"
#include "StateManagement.h"

namespace Harmony::Tasks
{
	PushState::PushState(const Utilities::UUID stateId) :
		Task(100, true, std::chrono::milliseconds(0)), stateId_(stateId)
	{
	}

	void PushState::run()
	{
		engine->get().stateManagement->push(stateId_);
	}


	PopState::PopState() :
		Task(100, true, std::chrono::milliseconds(100))
	{
	}

	void PopState::run()
	{
		engine->get().stateManagement->pop();
	}

}
