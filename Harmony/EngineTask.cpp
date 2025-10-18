#include "pch.h"
#include "Engine.h"
#include "EngineTask.h"

namespace Harmony::Tasks
{
	SetFPS::SetFPS(const unsigned int targetFPS) :
		Task(0, FastMultiThreaded), targetFPS_(targetFPS) {}

	void SetFPS::run()
	{
		getEngine().setTargetFPS(targetFPS_);
	}
}
