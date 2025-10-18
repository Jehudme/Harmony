#include "pch.h"
#include "TaskManagement.h"
#include "Engine.h"
#include "Task.h"
#include "Logger.h"

namespace Harmony::Management
{
	////////////////////////////////////////////////////////////////
	// Definition                                                 //
	////////////////////////////////////////////////////////////////

	struct TaskManagemer::WorkerPool {
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

	struct TaskManagemer::WorkerPool::Worker {
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

	/////////////////////////////////////////////////////////////////
	// Implementation                                              //
	/////////////////////////////////////////////////////////////////

    class TaskManagerError : public std::runtime_error {
    public:
        explicit TaskManagerError(const std::string& msg)
            : std::runtime_error("TaskManager error: " + msg) {}
    };

    TaskManagemer::WorkerPool::WorkerPool()
    {
        unsigned int workerCount = std::thread::hardware_concurrency();
        if (workerCount == 0) workerCount = 2; // fallback

        HARMONY_INFO("WorkerPool starting with {} workers", workerCount + 2);

        for (unsigned int workerIndex = 0; workerIndex < workerCount + 2; workerIndex++) {
            auto worker = std::make_unique<Worker>(tasks_, running_, mutex_, condition_);
            workers_.emplace_back(std::move(worker));
        }
    }

    TaskManagemer::WorkerPool::~WorkerPool()
    {
        HARMONY_INFO("WorkerPool shutting down ({} workers)", workers_.size());
        running_ = false;
        condition_.notify_all();
    }

    void TaskManagemer::WorkerPool::submit(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) {
            HARMONY_ERROR("Attempted to submit null task");
            throw TaskManagerError("Null task submitted to WorkerPool");
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(std::move(task));
            HARMONY_DEBUG("Task submitted (queue size = {})", tasks_.size());
        }
        condition_.notify_one();
    }

    TaskManagemer::WorkerPool::Worker::Worker(
        std::queue<std::unique_ptr<Tasks::Task>>& tasks,
        bool& running_,
        std::mutex& mutex,
        std::condition_variable& condition)
        : tasks_(tasks),
        running_(running_),
        mutex_(mutex),
        condition_(condition),
        thread_(run, std::ref(*this))
    {
        HARMONY_DEBUG("Worker thread created");
    }

    TaskManagemer::WorkerPool::Worker::~Worker()
    {
        if (thread_.joinable()) {
            thread_.join();
            HARMONY_DEBUG("Worker thread joined");
        }
    }

    void TaskManagemer::WorkerPool::Worker::run(Worker& worker)
    {
        HARMONY_DEBUG("Worker thread running");

        while (true) {
            std::unique_lock<std::mutex> lock(worker.mutex_);
            worker.condition_.wait(lock, [&worker] { return !worker.running_ || !worker.tasks_.empty(); });

            if (!worker.running_ && worker.tasks_.empty()) {
                HARMONY_INFO("Worker shutting down");
                return;
            }

            worker.currentTask_ = std::move(worker.tasks_.front());
            worker.tasks_.pop();
            lock.unlock();

            if (!worker.currentTask_) {
                HARMONY_ERROR("Worker encountered null task");
                continue;
            }

            HARMONY_DEBUG("Worker executing task (mode = {})", static_cast<int>(worker.currentTask_->mode));

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
                    break;
                }
            }
            catch (const std::exception& e) {
                HARMONY_ERROR("Task execution failed: {}", e.what());
            }
        }
    }

    void TaskManagemer::WorkerPool::Worker::runTask(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) {
            HARMONY_ERROR("runTask received null task");
            throw TaskManagerError("Null task in runTask");
        }

        try {
            task->start();
            HARMONY_DEBUG("Task executed successfully");
        }
        catch (const std::exception& e) {
            HARMONY_ERROR("Task threw exception: {}", e.what());
            throw;
        }
    }

    TaskManagemer::TaskManagemer(Engine& engine)
        : engine(engine), workerPool_(std::make_unique<WorkerPool>())
    {
        HARMONY_INFO("TaskManager created");
    }

    TaskManagemer::~TaskManagemer()
    {
        HARMONY_INFO("TaskManager destroyed, {} pending tasks", tasks_.size());
    }

    void TaskManagemer::submit(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) {
            HARMONY_ERROR("Attempted to submit null task");
            throw TaskManagerError("Null task submitted to TaskManager");
        }

        task->engine = std::make_optional<std::reference_wrapper<Engine>>(engine);

        if (task->priority == 0) {
            HARMONY_DEBUG("Executing priority=0 task immediately");
            handleTask(std::move(task));
            return;
        }

        tasks_.emplace(std::move(task));
        HARMONY_INFO("Task queued (priority > 0). Queue size = {}", tasks_.size());
    }

    void TaskManagemer::handleTasks()
    {
        while (!tasks_.empty()) {
            std::unique_ptr<Tasks::Task> task;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                task = std::move(const_cast<std::unique_ptr<Tasks::Task>&>(tasks_.top()));
                tasks_.pop();
            }

            if (!task) {
                HARMONY_ERROR("Encountered null task in queue");
                continue;
            }

            HARMONY_DEBUG("Handling queued task (mode = {})", static_cast<int>(task->mode));
            handleTask(std::move(task));
        }
    }

    void TaskManagemer::handleTask(std::unique_ptr<Tasks::Task> task)
    {
        if (!task) {
            HARMONY_ERROR("handleTask received null task");
            throw TaskManagerError("Null task in handleTask");
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