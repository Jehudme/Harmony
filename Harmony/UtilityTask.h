#pragma once
#include "Task.h"
#include <functional>

namespace Harmony::Tasks
{
	// Task to execute a callback function
	class CallbackTask : public Tasks::Task
	{
	public:
		CallbackTask(std::function<void(Engine&)> callback, int priority = 0, Mode mode = SingleThreaded);

	private:
		void run() override;

	private:
		std::function<void(Engine&)> callback_;
	};

	// Task to execute a simple lambda with no parameters
	class LambdaTask : public Tasks::Task
	{
	public:
		LambdaTask(std::function<void()> lambda, int priority = 0, Mode mode = SingleThreaded);

	private:
		void run() override;

	private:
		std::function<void()> lambda_;
	};

	// Task to log a message at a specific time
	class LogMessageTask : public Tasks::Task
	{
	public:
		enum LogLevel {
			Trace,
			Debug,
			Info,
			Warning,
			Error
		};

		LogMessageTask(const std::string& message, LogLevel level = Info);

	private:
		void run() override;

	private:
		const std::string message_;
		const LogLevel level_;
	};

	// Task for conditional execution based on a predicate
	class ConditionalTask : public Tasks::Task
	{
	public:
		ConditionalTask(std::function<bool(Engine&)> condition, 
			std::function<void(Engine&)> onTrue,
			std::function<void(Engine&)> onFalse = nullptr,
			int priority = 0);

	private:
		void run() override;

	private:
		std::function<bool(Engine&)> condition_;
		std::function<void(Engine&)> onTrue_;
		std::function<void(Engine&)> onFalse_;
	};

	// Task for repeated execution
	class RepeatedTask : public Tasks::Task
	{
	public:
		RepeatedTask(std::function<void(Engine&)> action, int repeatCount, 
			std::chrono::milliseconds delayBetween = std::chrono::milliseconds(0));

	private:
		void run() override;

	private:
		std::function<void(Engine&)> action_;
		const int repeatCount_;
		const std::chrono::milliseconds delayBetween_;
	};
}
