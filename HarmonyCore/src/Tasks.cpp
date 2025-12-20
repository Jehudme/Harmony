#include "Harmony/Tasks.h"
#include "internal/PrivateAccess.h"
#include <fstream>
#include <thread>
#include <future>

// Declare the tag for accessing ITask::run() (defined in TaskDispatcher.cpp)
struct TASK_RUN_TAG {
	using __Sig = void();
	using PtrType = __Sig Harmony::ITask::*;
};

namespace Harmony {

	// Helper function to call the private run() method
	static inline void executeTask(ITask& task) {
		std::invoke(Internal::steal<TASK_RUN_TAG, typename TASK_RUN_TAG::PtrType>(TASK_RUN_TAG{}), task);
	}

	// ============================================================================
	// ITask - Base class implementation
	// ============================================================================

	ITask::ITask(ExecutionMode mode, unsigned char priority, EngineContext* context)
		: mode(mode), priority(priority), context(context ? std::optional<EngineContext>(*context) : std::nullopt) {
	}

	// ============================================================================
	// LambdaTask - Functional task implementation
	// ============================================================================

	void LambdaTask::run() {
		if (m_func) {
			m_func();
		}
	}

	// ============================================================================
	// ConditionalTask - Conditional execution
	// ============================================================================

	ConditionalTask::ConditionalTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		Predicate condition, std::unique_ptr<ITask> trueTask, std::unique_ptr<ITask> falseTask)
		: ITask(mode, priority, ctx), m_condition(condition), m_trueTask(std::move(trueTask)), m_falseTask(std::move(falseTask)) {
	}

	void ConditionalTask::run() {
		if (m_condition && m_condition()) {
			if (m_trueTask) {
				executeTask(*m_trueTask);
			}
		}
		else {
			if (m_falseTask) {
				executeTask(*m_falseTask);
			}
		}
	}

	// ============================================================================
	// DelayedTask - Task execution with delay
	// ============================================================================

	DelayedTask::DelayedTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		std::chrono::milliseconds delay, std::unique_ptr<ITask> task)
		: ITask(mode, priority, ctx), m_delay(delay), m_task(std::move(task)) {
	}

	void DelayedTask::run() {
		std::this_thread::sleep_for(m_delay);
		if (m_task) {
			executeTask(*m_task);
		}
	}

	// ============================================================================
	// ScheduledTask - Task execution at specific time
	// ============================================================================

	ScheduledTask::ScheduledTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		TimePoint time, std::unique_ptr<ITask> task)
		: ITask(mode, priority, ctx), m_time(time), m_task(std::move(task)) {
	}

	void ScheduledTask::run() {
		auto now = std::chrono::system_clock::now();
		if (m_time > now) {
			std::this_thread::sleep_until(m_time);
		}
		if (m_task) {
			executeTask(*m_task);
		}
	}

	// ============================================================================
	// PeriodicTask - Repeating task execution
	// ============================================================================

	PeriodicTask::PeriodicTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		std::chrono::milliseconds interval, int repeatCount, BreakCondition earlyExit, std::unique_ptr<ITask> task)
		: ITask(mode, priority, ctx), m_interval(interval), m_maxRepeats(repeatCount), m_earlyExit(earlyExit), m_task(std::move(task)) {
	}

	void PeriodicTask::run() {
		int count = 0;
		while ((m_maxRepeats < 0 || count < m_maxRepeats)) {
			// Check early exit condition
			if (m_earlyExit && m_earlyExit()) {
				break;
			}

			// Execute the task
			if (m_task) {
				executeTask(*m_task);
			}

			count++;

			// Sleep for the interval (unless this was the last iteration)
			if (m_maxRepeats < 0 || count < m_maxRepeats) {
				std::this_thread::sleep_for(m_interval);
			}
		}
	}

	// ============================================================================
	// RawReadTask - File reading task
	// ============================================================================

	RawReadTask::RawReadTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		const std::string& path, SuccessCallback callback)
		: ITask(mode, priority, ctx), m_path(path), m_callback(callback) {
	}

	void RawReadTask::run() {
		std::ifstream file(m_path, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			// Call callback with empty data on failure
			if (m_callback) {
				m_callback(std::vector<uint8_t>());
			}
			return;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(size);
		if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
			if (m_callback) {
				m_callback(std::move(buffer));
			}
		}
		else {
			// Read failed, call callback with empty data
			if (m_callback) {
				m_callback(std::vector<uint8_t>());
			}
		}
	}

	// ============================================================================
	// RawWriteTask - File writing task
	// ============================================================================

	RawWriteTask::RawWriteTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		const std::string& path, std::vector<uint8_t> data)
		: ITask(mode, priority, ctx), m_path(path), m_data(std::move(data)) {
	}

	void RawWriteTask::run() {
		std::ofstream file(m_path, std::ios::binary);
		if (!file.is_open()) {
			return;
		}

		file.write(reinterpret_cast<const char*>(m_data.data()), m_data.size());
	}

	// ============================================================================
	// DownloadTask - Network download task
	// ============================================================================

	DownloadTask::DownloadTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		const std::string& url, DownloadCallback callback)
		: ITask(mode, priority, ctx), m_url(url), m_callback(callback) {
	}

	void DownloadTask::run() {
		// Note: This is a placeholder implementation
		// A real implementation would require a networking library like libcurl
		// For now, we'll just call the callback with empty data and error status
		if (m_callback) {
			m_callback(std::vector<uint8_t>(), 501); // 501 = Not Implemented
		}
	}

	// ============================================================================
	// SequenceTask - Sequential task execution
	// ============================================================================

	SequenceTask::SequenceTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx)
		: ITask(mode, priority, ctx) {
	}

	void SequenceTask::add(std::unique_ptr<ITask> task) {
		m_tasks.push_back(std::move(task));
	}

	void SequenceTask::run() {
		for (auto& task : m_tasks) {
			if (task) {
				executeTask(*task);
			}
		}
	}

	// ============================================================================
	// BarrierTask - Parallel task execution with barrier
	// ============================================================================

	BarrierTask::BarrierTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx)
		: ITask(mode, priority, ctx) {
	}

	void BarrierTask::add(std::unique_ptr<ITask> task) {
		m_tasks.push_back(std::move(task));
	}

	void BarrierTask::run() {
		std::vector<std::future<void>> futures;
		futures.reserve(m_tasks.size());

		// Launch all tasks in parallel
		for (auto& task : m_tasks) {
			if (task) {
				ITask* taskPtr = task.get();
				futures.push_back(std::async(std::launch::async, [taskPtr]() {
					executeTask(*taskPtr);
					}));
			}
		}

		// Wait for all tasks to complete
		for (auto& future : futures) {
			future.wait();
		}
	}

	// ============================================================================
	// WaitSignalTask - Wait for signal before executing task
	// ============================================================================

	WaitSignalTask::WaitSignalTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		std::atomic<bool>& signal, std::unique_ptr<ITask> task)
		: ITask(mode, priority, ctx), m_signal(signal), m_task(std::move(task)) {
	}

	void WaitSignalTask::run() {
		// Wait for signal to become true
		while (!m_signal.load(std::memory_order_acquire)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		// Execute the task
		if (m_task) {
			executeTask(*m_task);
		}
	}

	// ============================================================================
	// SignalTask - Set signal value
	// ============================================================================

	SignalTask::SignalTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx,
		std::atomic<bool>& signal, bool value)
		: ITask(mode, priority, ctx), m_signal(signal), m_value(value) {
	}

	void SignalTask::run() {
		m_signal.store(m_value, std::memory_order_release);
	}

} // namespace Harmony
