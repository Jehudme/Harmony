#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	// Task to set the target FPS
	class SetFPS : public Tasks::Task
	{
	public:
		SetFPS(const unsigned int targetFPS);
		void run() override;

	private:
		const unsigned int targetFPS_;
	};

	// Task to stop the engine
	class StopEngineTask : public Tasks::Task
	{
	public:
		StopEngineTask();

	private:
		void run() override;
	};

	// Task to perform a delayed action
	class DelayedActionTask : public Tasks::Task
	{
	public:
		DelayedActionTask(std::function<void()> action, std::chrono::milliseconds delay);

	private:
		void run() override;

	private:
		std::function<void()> action_;
	};
}

