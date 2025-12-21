#pragma once
#include "Harmony/EngineContext.h"

#include <memory>
#include <atomic>
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <optional>
#include <any>

namespace Harmony {

	enum class ExecutionMode {
		UndedicatedThreadMode,
		DedicatedThreadMode,
		MainThreadMode,
	};

	enum class ExecutionPriority : unsigned char {
		Immediate = 255,
		High = 170,
		Normal = 85,
		Low = 0
	};

	class ITask {
		friend class TaskDispatcher;
	public:
		ITask(ExecutionMode mode, unsigned char priority, EngineContext* context);
		virtual ~ITask() = default;

	public:
		std::optional<EngineContext> context;
		const ExecutionMode mode;
		const unsigned char priority;

	protected:
		virtual void run() = 0;
	};

	// --- FONCTIONNEL & PIPELINE ---

	class LambdaTask : public ITask {
	public:
		template<typename Function>
		LambdaTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, Function&& func);
	protected:
		void run() override;
	private:
		std::function<void()> m_func;
	};

	// --- LOGIQUE CONDITIONNELLE ---

	class ConditionalTask : public ITask {
	public:
		using Predicate = std::function<bool()>;
		ConditionalTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, Predicate condition, std::unique_ptr<ITask> trueTask, std::unique_ptr<ITask> falseTask = nullptr);
	protected:
		void run() override;
	private:
		Predicate m_condition;
		std::unique_ptr<ITask> m_trueTask;
		std::unique_ptr<ITask> m_falseTask;
	};

	// --- TEMPORISATION & PLANIFICATION ---

	class DelayedTask : public ITask {
	public:
		DelayedTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, std::chrono::milliseconds delay, std::unique_ptr<ITask> task);
	protected:
		void run() override;
	private:
		std::chrono::milliseconds m_delay;
		std::unique_ptr<ITask> m_task;
	};

	class ScheduledTask : public ITask {
	public:
		using TimePoint = std::chrono::system_clock::time_point;
		ScheduledTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, TimePoint time, std::unique_ptr<ITask> task);
	protected:
		void run() override;
	private:
		TimePoint m_time;
		std::unique_ptr<ITask> m_task;
	};

	class PeriodicTask : public ITask {
	public:
		using BreakCondition = std::function<bool()>;
		PeriodicTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, std::chrono::milliseconds interval, int repeatCount, BreakCondition earlyExit, std::unique_ptr<ITask> task);
	protected:
		void run() override;
	private:
		std::chrono::milliseconds m_interval;
		int m_maxRepeats;
		BreakCondition m_earlyExit;
		std::unique_ptr<ITask> m_task;
	};

	// --- ENTR�ES / SORTIES & R�SEAU ---

	class RawReadTask : public ITask {
	public:
		using SuccessCallback = std::function<void(std::vector<uint8_t>&&)>;
		RawReadTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, const std::string& path, SuccessCallback callback);
	protected:
		void run() override;
	private:
		std::string m_path;
		SuccessCallback m_callback;
	};

	class RawWriteTask : public ITask {
	public:
		RawWriteTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, const std::string& path, std::vector<uint8_t> data);
	protected:
		void run() override;
	private:
		std::string m_path;
		std::vector<uint8_t> m_data;
	};

	class DownloadTask : public ITask {
	public:
		using DownloadCallback = std::function<void(std::vector<uint8_t>&&, int statusCode)>;
		DownloadTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, const std::string& url, DownloadCallback callback);
	protected:
		void run() override;
	private:
		std::string m_url;
		DownloadCallback m_callback;
	};

	// --- COMPOSITES ---

	class SequenceTask : public ITask {
	public:
		SequenceTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx);
		void add(std::unique_ptr<ITask> task);
	protected:
		void run() override;
	private:
		std::vector<std::unique_ptr<ITask>> m_tasks;
	};

	class BarrierTask : public ITask {
	public:
		BarrierTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx);
		void add(std::unique_ptr<ITask> task);
	protected:
		void run() override;
	private:
		std::vector<std::unique_ptr<ITask>> m_tasks;
	};

	// --- SIGNALISATION ---

	class WaitSignalTask : public ITask {
	public:
		WaitSignalTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, std::atomic<bool>& signal, std::unique_ptr<ITask> task);
	protected:
		void run() override;
	private:
		std::atomic<bool>& m_signal;
		std::unique_ptr<ITask> m_task;
	};

	class SignalTask : public ITask {
	public:
		SignalTask(ExecutionMode mode, unsigned char priority, EngineContext* ctx, std::atomic<bool>& signal, bool value);
	protected:
		void run() override;
	private:
		std::atomic<bool>& m_signal;
		bool m_value;
	};
}

#include "Tasks.inl"