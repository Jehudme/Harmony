#pragma once
#include "Task.h"
#include <functional>

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

	// Task for smooth transition with optional delay/fade support
	class TransitionToStateTask : public Tasks::Task
	{
	public:
		TransitionToStateTask(const Utilities::UUID newStateId,
			std::chrono::milliseconds transitionDelay = std::chrono::milliseconds(0),
			std::function<void(Engine&)> transitionEffect = nullptr);

	private:
		void run() override;

	private:
		const Utilities::UUID newStateId_;
		const std::chrono::milliseconds transitionDelay_;
		std::function<void(Engine&)> transitionEffect_;
	};

	// Task to replace top state without popping first
	class ReplaceStateTask : public Tasks::Task
	{
	public:
		ReplaceStateTask(const Utilities::UUID newStateId);

	private:
		void run() override;

	private:
		const Utilities::UUID newStateId_;
	};

	// Task to query current state without modifying stack
	class PeekStateTask : public Tasks::Task
	{
	public:
		PeekStateTask(std::function<void(Utilities::UUID)> callback);

	private:
		void run() override;

	private:
		std::function<void(Utilities::UUID)> callback_;
	};
}

