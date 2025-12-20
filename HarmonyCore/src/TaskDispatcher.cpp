#include "Harmony/TaskDispatcher.h"
#include "internal/PrivateAccess.h"
#include "Harmony/EngineContext.h"
#include "Harmony/Engine.h"
#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <shared_mutex>
#include <condition_variable>

// Macro to access the private 'run' method of ITask
PREPARE_STEAL(TASK_RUN_TAG, Harmony::ITask, run, void());

namespace Harmony
{
	struct TaskCompare {
		bool operator()(const std::unique_ptr<ITask>& lhs, const std::unique_ptr<ITask>& rhs) const {
			if (lhs->mode != rhs->mode)
				return lhs->mode > rhs->mode;
			return lhs->priority < rhs->priority;
		}
	};

	class SafePriorityQueue : public std::priority_queue<std::unique_ptr<ITask>, std::vector<std::unique_ptr<ITask>>, TaskCompare>
	{
	public:
		std::unique_ptr<ITask> pop_move();
	};

	class WorkerPool {
		class Worker;
		friend Worker;

	public:
		WorkerPool();
		~WorkerPool();

		void submit(std::unique_ptr<ITask> task);
		std::unique_ptr<ITask> pop();

		void start();
		void stop();

		std::condition_variable workerCondition;
		std::mutex workerMutex;

	private:
		bool m_active;

		std::vector<std::unique_ptr<Worker>> m_workers;

		std::mutex activationMutex;
		std::shared_mutex m_activeMutex;
		std::shared_mutex queueMutex;
		SafePriorityQueue m_queue;
	};

	class WorkerPool::Worker {
	public:
		Worker(WorkerPool& pool);
		~Worker();

		void start();
		void stop();

	private:
		static void run(Worker* worker);

		static inline void execute(std::unique_ptr<ITask> task);
		static inline void executeDedicatedThread(std::unique_ptr<ITask> task);
		static inline void executeUndedicatedThread(std::unique_ptr<ITask> task);

	private:
		WorkerPool& m_pool;
		std::thread m_thread;
	};

	struct TaskDispatcher::Internal {
		WorkerPool workerPool;
		Engine* engine;

		std::mutex queueMutex;
		SafePriorityQueue queue;
	};

	// --- Worker Implementation ---

	WorkerPool::Worker::Worker(WorkerPool& pool) : m_pool(pool)
	{
	}

	WorkerPool::Worker::~Worker()
	{
		stop();
	}

	void WorkerPool::Worker::start()
	{
		m_thread = std::thread(&Worker::run, this);
	}

	void WorkerPool::Worker::stop()
	{
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	void WorkerPool::Worker::run(Worker* worker)
	{
		std::unique_lock<std::mutex> lock(worker->m_pool.workerMutex);

		while (true) {
			worker->m_pool.workerCondition.wait(lock, [&worker]() {
				std::shared_lock<std::shared_mutex> activeLock(worker->m_pool.m_activeMutex);
				std::shared_lock<std::shared_mutex> queueLock(worker->m_pool.queueMutex);
				return !worker->m_pool.m_active || !worker->m_pool.m_queue.empty();
				});

			{
				std::shared_lock<std::shared_mutex> activeLock(worker->m_pool.m_activeMutex);
				std::shared_lock<std::shared_mutex> queueLock(worker->m_pool.queueMutex);
				if (!worker->m_pool.m_active && worker->m_pool.m_queue.empty()) {
					return;
				}
			}

			std::unique_ptr<ITask> task = worker->m_pool.pop();

			if (lock.owns_lock()) {
				lock.unlock();
			}

			try {
				execute(std::move(task));
			}
			catch (...) {}

			if (task) {
				switch (task->mode) {
				case ExecutionMode::UndedicatedThreadMode:
					executeUndedicatedThread(std::move(task));
					break;

				case ExecutionMode::DedicatedThreadMode:
					executeDedicatedThread(std::move(task));
					break;
				default:
					break;
				}
			}

			if (!lock.owns_lock()) {
				lock.lock();
			}
		}
	}

	void WorkerPool::Worker::execute(std::unique_ptr<ITask> task)
	{
		if (task) {
			switch (task->mode) {
			case ExecutionMode::UndedicatedThreadMode:
				executeUndedicatedThread(std::move(task));
				break;

			case ExecutionMode::DedicatedThreadMode:
				executeDedicatedThread(std::move(task));
				break;
			default:
				break;
			}
		}
	}

	void WorkerPool::Worker::executeDedicatedThread(std::unique_ptr<ITask> task)
	{
		std::thread([task = std::move(task)]() mutable {
			CALL_PRIVATE(*task, TASK_RUN_TAG);
			}).detach();
	}

	void WorkerPool::Worker::executeUndedicatedThread(std::unique_ptr<ITask> task)
	{
		CALL_PRIVATE(*task, TASK_RUN_TAG);
	}

	// --- WorkerPool Implementation ---

	WorkerPool::WorkerPool() : m_active(false)
	{
		unsigned int workerCount = std::thread::hardware_concurrency();
		if (workerCount == 0) workerCount = 4;

		m_workers.reserve(workerCount);
		for (unsigned int i = 0; i < workerCount; ++i) {
			m_workers.push_back(std::make_unique<Worker>(*this));
		}
	}

	WorkerPool::~WorkerPool()
	{
		if (m_active) stop();
	}

	void WorkerPool::submit(std::unique_ptr<ITask> task)
	{
		{
			std::lock_guard<std::shared_mutex> lock(queueMutex);
			m_queue.push(std::move(task));
		}
		workerCondition.notify_one();
	}

	std::unique_ptr<ITask> WorkerPool::pop()
	{
		std::lock_guard <std::shared_mutex> lock(queueMutex);

		if (m_queue.empty()) return nullptr;
		return m_queue.pop_move();
	}

	void WorkerPool::start()
	{
		std::lock_guard<std::mutex> activationLock(activationMutex);
		{
			std::lock_guard <std::shared_mutex> lock(m_activeMutex);
			m_active = true;
		}

		for (auto& worker : m_workers) {
			worker->start();
		}
	}

	void WorkerPool::stop()
	{
		std::lock_guard<std::mutex> activationLock(activationMutex);
		{
			std::lock_guard <std::shared_mutex> lock(m_activeMutex);
			m_active = false;
		}
		workerCondition.notify_all();

		for (auto& worker : m_workers) {
			worker->stop();
		}
	}

	// --- TaskDispatcher Implementation ---

	TaskDispatcher::TaskDispatcher(Engine& engine) :
		m_internal(std::make_unique<Internal>())
	{
		m_internal->engine = &engine;
	}

	TaskDispatcher::~TaskDispatcher()
	{
		stopPool();
	}

	void TaskDispatcher::submit(std::unique_ptr<ITask> task)
	{
		if (!m_internal) return;
		task->context = m_internal->engine->context();

		if (task->priority == static_cast<unsigned char>(ExecutionPriority::Imediate)) {
			return execute(std::move(task));
		}

		std::lock_guard<std::mutex> lock(m_internal->queueMutex);
		m_internal->workerPool.submit(std::move(task));
	}

	void TaskDispatcher::startPool()
	{
		m_internal->workerPool.start();
	}

	void TaskDispatcher::stopPool()
	{
		m_internal->workerPool.stop();
	}

	void TaskDispatcher::executeAll()
	{
		while (true) {
			std::unique_ptr<ITask> task;
			{
				std::lock_guard<std::mutex> lock(m_internal->queueMutex);
				if (m_internal->queue.empty()) break;

				task = m_internal->queue.pop_move();
			}
			execute(std::move(task));
		}
	}

	void TaskDispatcher::execute(std::unique_ptr<ITask> task)
	{
		switch (task->mode) {
		case ExecutionMode::MainThreadMode:
			CALL_PRIVATE(*task, TASK_RUN_TAG);
			break;

		case ExecutionMode::DedicatedThreadMode:
		case ExecutionMode::UndedicatedThreadMode:
			m_internal->workerPool.submit(std::move(task));
			break;

		default:
			break;
		}
	}

	std::unique_ptr<ITask> SafePriorityQueue::pop_move()
	{
		// Standard heap removal: move top to back, then pop back
		std::pop_heap(c.begin(), c.end(), comp);
		std::unique_ptr<ITask> item = std::move(c.back());
		c.pop_back();
		return item;
	}
}