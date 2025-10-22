#include "pch.h"
#include "UtilityTask.h"
#include "Engine.h"
#include "Logger.h"

namespace Harmony::Tasks
{
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
}
