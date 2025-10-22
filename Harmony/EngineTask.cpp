#include "pch.h"
#include "Engine.h"
#include "EngineTask.h"
#include "Logger.h"

namespace Harmony::Tasks
{
	SetFPS::SetFPS(const unsigned int targetFPS) :
		Task(0, FastMultiThreaded), targetFPS_(targetFPS) {}

	void SetFPS::run()
	{
		getEngine().setTargetFPS(targetFPS_);
	}

	// StopEngineTask implementation
	StopEngineTask::StopEngineTask() :
		Task(200, FastMultiThreaded) {}

	void StopEngineTask::run()
	{
		HARMONY_INFO("Stopping engine via task");
		getEngine().stop();
	}

	// DelayedActionTask implementation
	DelayedActionTask::DelayedActionTask(std::function<void()> action, std::chrono::milliseconds delay) :
		Task(0, SlowMultiThreaded, delay), action_(action) {}

	void DelayedActionTask::run()
	{
		if (action_) {
			try {
				action_();
				HARMONY_DEBUG("Delayed action executed successfully");
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Delayed action execution failed: {}", e.what());
			}
		}
		else {
			HARMONY_WARN("Delayed action task has no action to execute");
		}
	}
}
