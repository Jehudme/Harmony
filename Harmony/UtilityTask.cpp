#include "pch.h"
#include "UtilityTask.h"
#include "Engine.h"
#include "Logger.h"
#include "Scene.h"
#include "SceneManagement.h"
#include <thread>
#include <chrono>
#include <unordered_map>
#include <mutex>

namespace Harmony::Tasks
{
	// Static members for ThrottledTask
	std::unordered_map<size_t, std::chrono::steady_clock::time_point> ThrottledTask::lastExecutionTimes_;
	std::mutex ThrottledTask::throttleMutex_;

	// CallbackTask implementation
	CallbackTask::CallbackTask(std::function<void(Engine&)> callback, int priority, Mode mode) :
		Task(priority, mode), callback_(callback) {}

	void CallbackTask::run()
	{
		if (callback_) {
			try {
				callback_(getEngine());
				HARMONY_DEBUG("Callback task executed successfully");
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Callback task execution failed: {}", e.what());
			}
		}
		else {
			HARMONY_WARN("Callback task has no callback to execute");
		}
	}

	// LambdaTask implementation
	LambdaTask::LambdaTask(std::function<void()> lambda, int priority, Mode mode) :
		Task(priority, mode), lambda_(lambda) {}

	void LambdaTask::run()
	{
		if (lambda_) {
			try {
				lambda_();
				HARMONY_DEBUG("Lambda task executed successfully");
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Lambda task execution failed: {}", e.what());
			}
		}
		else {
			HARMONY_WARN("Lambda task has no lambda to execute");
		}
	}

	// LogMessageTask implementation
	LogMessageTask::LogMessageTask(const std::string& message, LogLevel level) :
		Task(0, SingleThreaded), message_(message), level_(level) {}

	void LogMessageTask::run()
	{
		switch (level_) {
		case Trace:
			HARMONY_TRACE("{}", message_);
			break;
		case Debug:
			HARMONY_DEBUG("{}", message_);
			break;
		case Info:
			HARMONY_INFO("{}", message_);
			break;
		case Warning:
			HARMONY_WARN("{}", message_);
			break;
		case Error:
			HARMONY_ERROR("{}", message_);
			break;
		default:
			HARMONY_INFO("{}", message_);
			break;
		}
	}

	// ConditionalTask implementation
	ConditionalTask::ConditionalTask(std::function<bool(Engine&)> condition, 
		std::function<void(Engine&)> onTrue,
		std::function<void(Engine&)> onFalse,
		int priority) :
		Task(priority, FastMultiThreaded), condition_(condition), onTrue_(onTrue), onFalse_(onFalse) {}

	void ConditionalTask::run()
	{
		if (!condition_) {
			HARMONY_WARN("Conditional task has no condition to evaluate");
			return;
		}

		try {
			bool result = condition_(getEngine());
			
			if (result && onTrue_) {
				onTrue_(getEngine());
				HARMONY_DEBUG("Conditional task: condition true, executed onTrue action");
			}
			else if (!result && onFalse_) {
				onFalse_(getEngine());
				HARMONY_DEBUG("Conditional task: condition false, executed onFalse action");
			}
			else {
				HARMONY_DEBUG("Conditional task: condition {}, no action to execute", result ? "true" : "false");
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Conditional task execution failed: {}", e.what());
		}
	}

	// RepeatedTask implementation
	RepeatedTask::RepeatedTask(std::function<void(Engine&)> action, int repeatCount, 
		std::chrono::milliseconds delayBetween) :
		Task(0, SlowMultiThreaded), action_(action), repeatCount_(repeatCount), delayBetween_(delayBetween) {}

	void RepeatedTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Repeated task has no action to execute");
			return;
		}

		HARMONY_INFO("Repeated task starting: {} iterations with {}ms delay", repeatCount_, delayBetween_.count());

		for (int i = 0; i < repeatCount_; ++i) {
			try {
				action_(getEngine());
				HARMONY_DEBUG("Repeated task iteration {}/{} completed", i + 1, repeatCount_);

				if (i < repeatCount_ - 1 && delayBetween_.count() > 0) {
					std::this_thread::sleep_for(delayBetween_);
				}
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Repeated task iteration {}/{} failed: {}", i + 1, repeatCount_, e.what());
			}
		}

		HARMONY_INFO("Repeated task completed all {} iterations", repeatCount_);
	}

	// SequentialTasksTask implementation
	SequentialTasksTask::SequentialTasksTask(std::vector<std::function<void(Engine&)>> actions, 
		int priority) :
		Task(priority, FastMultiThreaded), actions_(actions) {}

	void SequentialTasksTask::run()
	{
		if (actions_.empty()) {
			HARMONY_WARN("Sequential tasks task has no actions to execute");
			return;
		}

		HARMONY_INFO("Sequential tasks starting: {} actions", actions_.size());
		int successCount = 0;
		int failCount = 0;

		for (size_t i = 0; i < actions_.size(); ++i) {
			try {
				if (actions_[i]) {
					actions_[i](getEngine());
					successCount++;
					HARMONY_DEBUG("Sequential task action {}/{} completed", i + 1, actions_.size());
				}
			}
			catch (const std::exception& e) {
				failCount++;
				HARMONY_ERROR("Sequential task action {}/{} failed: {}", i + 1, actions_.size(), e.what());
			}
		}

		HARMONY_INFO("Sequential tasks completed: {} succeeded, {} failed", successCount, failCount);
	}

	// TimedActionTask implementation
	TimedActionTask::TimedActionTask(std::function<void(Engine&)> action, 
		const std::string& actionName, int priority) :
		Task(priority, FastMultiThreaded), action_(action), actionName_(actionName) {}

	void TimedActionTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Timed action task has no action to execute");
			return;
		}

		auto startTime = std::chrono::high_resolution_clock::now();
		
		try {
			action_(getEngine());
			
			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
			
			HARMONY_INFO("Timed action '{}' completed in {:.3f}ms", 
				actionName_, duration.count() / 1000.0);
		}
		catch (const std::exception& e) {
			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
			
			HARMONY_ERROR("Timed action '{}' failed after {:.3f}ms: {}", 
				actionName_, duration.count() / 1000.0, e.what());
		}
	}

	// RetryTask implementation
	RetryTask::RetryTask(std::function<bool(Engine&)> action, int maxRetries,
		std::chrono::milliseconds delayBetween, int priority) :
		Task(priority, SlowMultiThreaded), action_(action), maxRetries_(maxRetries), 
		delayBetween_(delayBetween) {}

	void RetryTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Retry task has no action to execute");
			return;
		}

		HARMONY_INFO("Retry task starting: max {} retries with {}ms delay", maxRetries_, delayBetween_.count());

		for (int attempt = 0; attempt <= maxRetries_; ++attempt) {
			try {
				bool success = action_(getEngine());
				
				if (success) {
					HARMONY_INFO("Retry task succeeded on attempt {}/{}", attempt + 1, maxRetries_ + 1);
					return;
				}
				else {
					HARMONY_WARN("Retry task attempt {}/{} returned false", attempt + 1, maxRetries_ + 1);
				}
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Retry task attempt {}/{} failed: {}", attempt + 1, maxRetries_ + 1, e.what());
			}

			if (attempt < maxRetries_) {
				std::this_thread::sleep_for(delayBetween_);
			}
		}

		HARMONY_ERROR("Retry task failed after {} attempts", maxRetries_ + 1);
	}

	// ThrottledTask implementation
	ThrottledTask::ThrottledTask(std::function<void(Engine&)> action,
		std::chrono::milliseconds minDelay, int priority) :
		Task(priority, FastMultiThreaded), action_(action), minDelay_(minDelay) {}

	void ThrottledTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Throttled task has no action to execute");
			return;
		}

		size_t taskHash = reinterpret_cast<size_t>(&action_);
		auto now = std::chrono::steady_clock::now();

		{
			std::lock_guard<std::mutex> lock(throttleMutex_);
			
			auto it = lastExecutionTimes_.find(taskHash);
			if (it != lastExecutionTimes_.end()) {
				auto timeSinceLastExecution = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);
				
				if (timeSinceLastExecution < minDelay_) {
					HARMONY_DEBUG("Throttled task skipped: only {}ms since last execution (min {}ms)", 
						timeSinceLastExecution.count(), minDelay_.count());
					return;
				}
			}

			lastExecutionTimes_[taskHash] = now;
		}

		try {
			action_(getEngine());
			HARMONY_DEBUG("Throttled task executed successfully");
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Throttled task execution failed: {}", e.what());
		}
	}

	// DebouncedTask implementation
	DebouncedTask::DebouncedTask(std::function<void(Engine&)> action,
		std::chrono::milliseconds delay, int priority) :
		Task(priority, SlowMultiThreaded), action_(action), delay_(delay) {}

	void DebouncedTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Debounced task has no action to execute");
			return;
		}

		std::this_thread::sleep_for(delay_);

		try {
			action_(getEngine());
			HARMONY_DEBUG("Debounced task executed after {}ms delay", delay_.count());
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Debounced task execution failed: {}", e.what());
		}
	}

	// MemorySnapshotTask implementation
	MemorySnapshotTask::MemorySnapshotTask(std::function<void(std::string)> callback) :
		Task(0, FastMultiThreaded), callback_(callback) {}

	void MemorySnapshotTask::run()
	{
		try {
			std::string snapshot = "Memory Snapshot:\n";
			snapshot += "  Task system active\n";
			// Note: Full memory profiling would require platform-specific code
			
			HARMONY_INFO("{}", snapshot);
			
			if (callback_) {
				callback_(snapshot);
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Memory snapshot task failed: {}", e.what());
		}
	}

	// DumpSceneInfoTask implementation
	DumpSceneInfoTask::DumpSceneInfoTask(const Utilities::UUID sceneId) :
		Task(0, FastMultiThreaded), sceneId_(sceneId) {}

	void DumpSceneInfoTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("Failed to dump scene info: Scene {} not found", sceneId_);
				return;
			}

			auto& registry = Scenes::getRegistryFromScene(*scene);
			size_t entityCount = registry.alive();

			HARMONY_INFO("Scene {} Information:", sceneId_);
			HARMONY_INFO("  Scene ID: {}", sceneId_);
			HARMONY_INFO("  Entity Count: {}", entityCount);
			HARMONY_INFO("  Drawing Enabled: {}", scene->isDrawingEnabled());
			HARMONY_INFO("  Updating Enabled: {}", scene->isUpdatingEnabled());
			HARMONY_INFO("  Draw Order: {}", scene->drawOrder);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to dump scene info: {}", e.what());
		}
	}

	// ValidateSceneTask implementation
	ValidateSceneTask::ValidateSceneTask(const Utilities::UUID sceneId,
		std::function<void(bool, std::string)> callback) :
		Task(0, FastMultiThreaded), sceneId_(sceneId), callback_(callback) {}

	void ValidateSceneTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				std::string error = "Scene not found";
				HARMONY_ERROR("Scene validation failed: {}", error);
				if (callback_) callback_(false, error);
				return;
			}

			auto& registry = Scenes::getRegistryFromScene(*scene);
			size_t entityCount = registry.alive();

			// Basic validation checks
			bool isValid = true;
			std::string message = "Scene validation passed";

			if (entityCount == 0) {
				HARMONY_WARN("Scene {} has no entities", sceneId_);
			}

			HARMONY_INFO("Scene {} validation: {}", sceneId_, message);
			
			if (callback_) {
				callback_(isValid, message);
			}
		}
		catch (const std::exception& e) {
			std::string error = std::string("Validation error: ") + e.what();
			HARMONY_ERROR("{}", error);
			if (callback_) callback_(false, error);
		}
	}

	// ChainedTasksTask implementation
	ChainedTasksTask::ChainedTasksTask(std::vector<std::function<bool(Engine&)>> tasks,
		std::function<void(Engine&, int)> onError, int priority) :
		Task(priority, FastMultiThreaded), tasks_(tasks), onError_(onError) {}

	void ChainedTasksTask::run()
	{
		if (tasks_.empty()) {
			HARMONY_WARN("Chained tasks task has no tasks to execute");
			return;
		}

		HARMONY_INFO("Chained tasks starting: {} tasks", tasks_.size());

		for (size_t i = 0; i < tasks_.size(); ++i) {
			try {
				if (!tasks_[i]) {
					HARMONY_ERROR("Chained task {} is null, aborting chain", i);
					if (onError_) onError_(getEngine(), static_cast<int>(i));
					return;
				}

				bool success = tasks_[i](getEngine());
				
				if (!success) {
					HARMONY_ERROR("Chained task {} failed, aborting chain", i);
					if (onError_) onError_(getEngine(), static_cast<int>(i));
					return;
				}

				HARMONY_DEBUG("Chained task {}/{} completed successfully", i + 1, tasks_.size());
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Chained task {} threw exception: {}", i, e.what());
				if (onError_) onError_(getEngine(), static_cast<int>(i));
				return;
			}
		}

		HARMONY_INFO("All {} chained tasks completed successfully", tasks_.size());
	}

	// ScheduledTask implementation
	ScheduledTask::ScheduledTask(std::function<void(Engine&)> action,
		std::chrono::steady_clock::time_point executeAt, int priority) :
		Task(priority, SlowMultiThreaded), action_(action), executeAt_(executeAt) {}

	void ScheduledTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Scheduled task has no action to execute");
			return;
		}

		auto now = std::chrono::steady_clock::now();
		
		if (executeAt_ > now) {
			auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(executeAt_ - now);
			HARMONY_DEBUG("Scheduled task waiting {}ms until execution", delay.count());
			std::this_thread::sleep_until(executeAt_);
		}

		try {
			action_(getEngine());
			HARMONY_INFO("Scheduled task executed successfully");
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Scheduled task execution failed: {}", e.what());
		}
	}

	// IntervalTask implementation
	IntervalTask::IntervalTask(std::function<bool(Engine&)> action,
		std::chrono::milliseconds interval, int maxExecutions) :
		Task(0, SlowMultiThreaded), action_(action), interval_(interval), 
		maxExecutions_(maxExecutions) {}

	void IntervalTask::run()
	{
		if (!action_) {
			HARMONY_WARN("Interval task has no action to execute");
			return;
		}

		HARMONY_INFO("Interval task starting: {}ms interval, {} max executions", 
			interval_.count(), maxExecutions_ == -1 ? "infinite" : std::to_string(maxExecutions_));

		int executionCount = 0;
		
		while (maxExecutions_ == -1 || executionCount < maxExecutions_) {
			try {
				bool shouldContinue = action_(getEngine());
				executionCount++;
				
				if (!shouldContinue) {
					HARMONY_INFO("Interval task stopped by action after {} executions", executionCount);
					break;
				}

				HARMONY_DEBUG("Interval task execution {}/{}", executionCount, 
					maxExecutions_ == -1 ? "∞" : std::to_string(maxExecutions_));

				if (maxExecutions_ == -1 || executionCount < maxExecutions_) {
					std::this_thread::sleep_for(interval_);
				}
			}
			catch (const std::exception& e) {
				HARMONY_ERROR("Interval task execution {} failed: {}", executionCount, e.what());
			}
		}

		HARMONY_INFO("Interval task completed after {} executions", executionCount);
	}
}
