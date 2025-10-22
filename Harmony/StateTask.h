#pragma once
#include "Task.h"

namespace Harmony::Tasks
{
	class PushState : public Tasks::Task
	{
	public:
		PushState(const Utilities::UUID stateId);

	private:
		void run() override;

	private:
		const Utilities::UUID stateId_;

	};

	class PopState : public Tasks::Task {
	public:
		PopState();

	private:
		void run() override;

	};

	// Task to clear all states from the state stack
	class ClearStatesTask : public Tasks::Task
	{
	public:
		ClearStatesTask();

	private:
		void run() override;
	};

	// Task to switch to a different state (pop current and push new)
	class SwitchStateTask : public Tasks::Task
	{
	public:
		SwitchStateTask(const Utilities::UUID newStateId);

	private:
		void run() override;

	private:
		const Utilities::UUID newStateId_;
	};
}

