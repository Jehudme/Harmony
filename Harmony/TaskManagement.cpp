#include "pch.h"
#include "TaskManagement.h"
#include "Engine.h"
#include "Task.h"
#include "Logger.h"

namespace Harmony::Management
{
	struct TaskManager::WorkerPool {
	public:
		WorkerPool();
		~WorkerPool();

		void submit(std::unique_ptr<Tasks::Task> task);

	private:
		struct Worker;

		std::mutex mutex_;
		std::condition_variable condition_;

		bool running_ = true;

		std::queue<std::unique_ptr<Tasks::Task>> tasks_;
		std::vector<std::unique_ptr<Worker>> workers_;
	};

	struct TaskManager::WorkerPool::Worker {
	public:
		friend struct WorkerPool;

		Worker(std::queue<std::unique_ptr<Tasks::Task>>& tasks, bool& running_, std::mutex& mutex, std::condition_variable& condition);
		~Worker();

		// Prevent copying
		Worker(const Worker&) = delete;
		Worker& operator=(const Worker&) = delete;

		// Allow moving
		Worker(Worker&&) noexcept = default;
		Worker& operator=(Worker&&) noexcept = default;


	private:
		static void run(Worker& worker);
		static void runTask(std::unique_ptr<Tasks::Task> task);

	private:
		std::queue<std::unique_ptr<Tasks::Task>>& tasks_;
		std::unique_ptr<Tasks::Task> currentTask_;

		std::mutex& mutex_;
		std::condition_variable& condition_;

		bool& running_;
		std::thread thread_;
	};

    TaskManager::WorkerPool::WorkerPool()
    {
        unsigned int workerCount = std::thread::hardware_concurrency();
        if (workerCount == 0) workerCount = 4; // fallback

        HARMONY_INFO("WorkerPool starting with {} workers", workerCount);

        for (unsigned int workerIndex = 0; workerIndex < workerCount; workerIndex++) {
            auto worker = std::make_unique<Worker>(tasks_, running_, mutex_, condition_);
            workers_.emplace_back(std::move(worker));
        }
    }

    TaskManager::WorkerPool::~WorkerPool()
    {
        HARMONY_INFO("WorkerPool shutting down ({} workers)", workers_.size());
        running_ = false;
        condition_.notify_all();
    }

    void TaskManager::WorkerPool::submit(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) throw Exceptions::NullTaskException("Null task submitted to WorkerPool");
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(std::move(task));
            HARMONY_DEBUG("Task submitted (queue size = {})", tasks_.size());
        }
        condition_.notify_one();
    }

    TaskManager::WorkerPool::Worker::Worker(
        std::queue<std::unique_ptr<Tasks::Task>>& tasks,
        bool& running_,
        std::mutex& mutex,
        std::condition_variable& condition)
        : tasks_(tasks),
        running_(running_),
        mutex_(mutex),
        condition_(condition),
        thread_(run, std::ref(*this)) {}

    TaskManager::WorkerPool::Worker::~Worker()
    {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void TaskManager::WorkerPool::Worker::run(Worker& worker)
    {
        while (true) 
        {
            std::unique_lock<std::mutex> lock(worker.mutex_);
            worker.condition_.wait(lock, [&worker] { return !worker.running_ || !worker.tasks_.empty(); });

			// Shutdown condition
            if (!worker.running_ && worker.tasks_.empty()) { return; }

            worker.currentTask_ = std::move(worker.tasks_.front());
            worker.tasks_.pop();

            lock.unlock();

            if (!worker.currentTask_) HARMONY_ERROR("Worker encountered null task"); continue;

            try {
                switch (worker.currentTask_->mode) {
                case Tasks::Task::FastMultiThreaded:
                    runTask(std::move(worker.currentTask_));
                    break;

                case Tasks::Task::SlowMultiThreaded:
                    std::thread(Worker::runTask, std::move(worker.currentTask_)).detach();
                    break;

                default:
                    HARMONY_WARN("Worker encountered task with unknown mode");
                    runTask(std::move(worker.currentTask_));
                    break;
                }
            }
            catch (const std::exception& e) {
                HARMONY_ERROR("Task execution failed: {}", e.what());
            }
        }
    }

    void TaskManager::WorkerPool::Worker::runTask(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) throw Exceptions::NullTaskException("Null task in runTask");
        
        try { task->start(); }
        catch (const std::exception& e) { HARMONY_ERROR("Task execution failed: {}", e.what()); }
    }

    TaskManager::TaskManager(Engine& engine)
        : engine(engine), workerPool_(std::make_unique<WorkerPool>()) {
        HARMONY_INFO("TaskManager created");
    }

    TaskManager::~TaskManager() {
        HARMONY_INFO("TaskManager destroyed, {} pending tasks", tasks_.size());
    }

    void TaskManager::submit(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) throw Exceptions::NullTaskException("Attempted to submit null task");

        task->engine = std::make_optional<std::reference_wrapper<Engine>>(engine);

		// Immediate execution
        if (task->priority == 0) {
            handleTask(std::move(task));
            return;
        }

        tasks_.emplace(std::move(task));
    }

    void TaskManager::handleTasks()
    {
        while (!tasks_.empty()) {
            std::unique_ptr<Tasks::Task> task;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                task = std::move(const_cast<std::unique_ptr<Tasks::Task>&>(tasks_.top()));
                tasks_.pop();
            }

            if (!task) {
                HARMONY_WARN("Encountered null task in queue");
                continue;
            }

            HARMONY_DEBUG("Handling queued task (mode = {})", static_cast<int>(task->mode));
            handleTask(std::move(task));
        }
    }

    void TaskManager::handleTask(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) {
            HARMONY_ERROR("handleTask received null task");
            throw Exceptions::NullTaskException("Null task in handleTask");
        }

        switch (task->mode) {
        case Tasks::Task::SingleThreaded:
            HARMONY_DEBUG("Executing SingleThreaded task");
            task->start();
            break;

        case Tasks::Task::FastMultiThreaded:
        case Tasks::Task::SlowMultiThreaded:
            HARMONY_DEBUG("Submitting MultiThreaded task to WorkerPool");
            workerPool_->submit(std::move(task));
            break;

        default:
            HARMONY_WARN("Task has unknown execution mode");
            break;
        }
    }
}

namespace Harmony::Exceptions
{
    NullTaskException::NullTaskException(const std::string& message) : std::runtime_error(message) { HARMONY_ERROR(what()); }
	FailedTaskExecutionException::FailedTaskExecutionException(const std::string& message) : std::runtime_error(message) { HARMONY_ERROR(what()); }
}