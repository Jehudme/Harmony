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

	// Task to execute multiple tasks in sequence
	class SequentialTasksTask : public Tasks::Task
	{
	public:
		SequentialTasksTask(std::vector<std::function<void(Engine&)>> actions, 
			int priority = 50);

	private:
		void run() override;

	private:
		std::vector<std::function<void(Engine&)>> actions_;
	};

	// Task to measure and log execution time
	class TimedActionTask : public Tasks::Task
	{
	public:
		TimedActionTask(std::function<void(Engine&)> action, 
			const std::string& actionName,
			int priority = 50);

	private:
		void run() override;

	private:
		std::function<void(Engine&)> action_;
		const std::string actionName_;
	};

	// Task to retry an action on failure
	class RetryTask : public Tasks::Task
	{
	public:
		RetryTask(std::function<bool(Engine&)> action, 
			int maxRetries = 3,
			std::chrono::milliseconds delayBetween = std::chrono::milliseconds(1000),
			int priority = 50);

	private:
		void run() override;

	private:
		std::function<bool(Engine&)> action_;
		const int maxRetries_;
		const std::chrono::milliseconds delayBetween_;
	};

	// Task to throttle execution (execute at most once per time period)
	class ThrottledTask : public Tasks::Task
	{
	public:
		ThrottledTask(std::function<void(Engine&)> action,
			std::chrono::milliseconds minDelay,
			int priority = 50);

	private:
		void run() override;

	private:
		std::function<void(Engine&)> action_;
		const std::chrono::milliseconds minDelay_;
		static std::unordered_map<size_t, std::chrono::steady_clock::time_point> lastExecutionTimes_;
		static std::mutex throttleMutex_;
	};

	// Task to debounce execution (execute only after period of inactivity)
	class DebouncedTask : public Tasks::Task
	{
	public:
		DebouncedTask(std::function<void(Engine&)> action,
			std::chrono::milliseconds delay,
			int priority = 50);

	private:
		void run() override;

	private:
		std::function<void(Engine&)> action_;
		const std::chrono::milliseconds delay_;
	};

	// Task to collect memory snapshot information
	class MemorySnapshotTask : public Tasks::Task
	{
	public:
		MemorySnapshotTask(std::function<void(std::string)> callback = nullptr);

	private:
		void run() override;

	private:
		std::function<void(std::string)> callback_;
	};

	// Task to dump scene information for debugging
	class DumpSceneInfoTask : public Tasks::Task
	{
	public:
		DumpSceneInfoTask(const Utilities::UUID sceneId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
	};

	// Task to validate scene integrity
	class ValidateSceneTask : public Tasks::Task
	{
	public:
		ValidateSceneTask(const Utilities::UUID sceneId, 
			std::function<void(bool, std::string)> callback = nullptr);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		std::function<void(bool, std::string)> callback_;
	};

	// Task to chain multiple tasks with error handling
	class ChainedTasksTask : public Tasks::Task
	{
	public:
		ChainedTasksTask(std::vector<std::function<bool(Engine&)>> tasks,
			std::function<void(Engine&, int)> onError = nullptr,
			int priority = 50);

	private:
		void run() override;

	private:
		std::vector<std::function<bool(Engine&)>> tasks_;
		std::function<void(Engine&, int)> onError_;
	};

	// Task to schedule an action at a specific time
	class ScheduledTask : public Tasks::Task
	{
	public:
		ScheduledTask(std::function<void(Engine&)> action,
			std::chrono::steady_clock::time_point executeAt,
			int priority = 50);

	private:
		void run() override;

	private:
		std::function<void(Engine&)> action_;
		const std::chrono::steady_clock::time_point executeAt_;
	};

	// Task for interval-based execution (like a timer)
	class IntervalTask : public Tasks::Task
	{
	public:
		IntervalTask(std::function<bool(Engine&)> action,
			std::chrono::milliseconds interval,
			int maxExecutions = -1);  // -1 for infinite

	private:
		void run() override;

	private:
		std::function<bool(Engine&)> action_;
		const std::chrono::milliseconds interval_;
		const int maxExecutions_;
	};
}
