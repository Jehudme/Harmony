#include "pch.h"
#include "TasksHandler.h"

#include "Engine.h"
#include "Task.h"
#include "Logger.h"
#include "Assert.h"

namespace Harmony::Internals
{
    static void runTask(Engine& engine, Tasks::Task_t* task);

    struct TasksHandler::WorkerPool 
    {
    public:
        WorkerPool(Engine& engine);
        ~WorkerPool();

        void submit(Tasks::Task_t* task);
        std::size_t getActiveWorkerCount() const;

    private:
        struct Worker;

        priorityQueue tasks_;
        std::vector<Worker> workers_;

        std::mutex mutex_;
        std::condition_variable condition_;

        std::mutex slowTasksDeleteMutex_;
        std::condition_variable slowTasksDeleteCondition_;

        std::atomic<std::size_t> activeWorkerCount_;

        bool running_ = true;
    };

    struct TasksHandler::WorkerPool::Worker {
    public:
        friend struct WorkerPool;

        Worker(Engine& engine, priorityQueue& tasks, bool& running_, std::mutex& mutex, std::condition_variable& condition);
        ~Worker();

        // Prevent copying
        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;

        // Allow moving
        Worker(Worker&&) noexcept = default;
        Worker& operator=(Worker&&) noexcept = default;


    private:
        static void run(Engine& engine, Worker& worker);

    private:
        priorityQueue& tasks_;
        Tasks::Task_t* currentTask_;

        std::mutex& mutex_;
        std::condition_variable& condition_;

        bool& running_;
        std::thread thread_;
    };

    TasksHandler::WorkerPool::WorkerPool(Engine& engine)
    {
        HARMONY_INFO("Initializing TasksHandler WorkerPool");
        
        unsigned int workerCount = std::thread::hardware_concurrency();
		if (workerCount == 0) {
            HARMONY_WARN("Unable to detect hardware concurrency, falling back to 4 workers");
            workerCount = 4; // Fallback to 4 workers if unable to detect
        }

        HARMONY_INFO("Creating {} worker threads", workerCount);
        HARMONY_ASSERT(workerCount > 0 && workerCount <= 256, "Worker count must be between 1 and 256");

        try {
            for (unsigned int workerIndex = 0; workerIndex < workerCount; workerIndex++) {
                HARMONY_DEBUG("Creating worker thread {}/{}", workerIndex + 1, workerCount);
                workers_.emplace_back(engine, tasks_, running_, mutex_, condition_);
            }
            HARMONY_INFO("WorkerPool initialized successfully with {} workers", workerCount);
        }
        catch (const std::exception& e) {
            HARMONY_CRITICAL("Failed to initialize WorkerPool: {}", e.what());
            throw Exceptions::WorkerPoolException("initialization", e.what());
        }
    }

    TasksHandler::WorkerPool::~WorkerPool()
    {
        HARMONY_INFO("Shutting down TasksHandler WorkerPool");
        
        running_ = false;
        condition_.notify_all();
        
        HARMONY_DEBUG("Clearing {} workers", workers_.size());
		workers_.clear();
        
		std::unique_lock<std::mutex> lock(slowTasksDeleteMutex_);
        HARMONY_DEBUG("Waiting for active workers to complete (current count: {})", activeWorkerCount_.load());
		slowTasksDeleteCondition_.wait(lock, [this]() { return activeWorkerCount_ == 0; });
        
        HARMONY_INFO("WorkerPool shutdown complete");
    }

    void TasksHandler::WorkerPool::submit(Tasks::Task_t* task)
    {
        HARMONY_ASSERT_NOT_NULL(task, "Cannot submit null task to WorkerPool");
        
        if (!task) {
            HARMONY_ERROR("Attempted to submit null task to WorkerPool");
            throw Exceptions::InvalidTaskException("Task pointer is null");
        }

        HARMONY_DEBUG("Submitting task with priority {} and mode {} to WorkerPool", 
                     task->priority, static_cast<int>(task->mode));
        
        try {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                tasks_.emplace(std::move(task));
            }
            condition_.notify_one();
            HARMONY_TRACE("Task submitted successfully to WorkerPool");
        }
        catch (const std::exception& e) {
            HARMONY_ERROR("Failed to submit task to WorkerPool: {}", e.what());
            throw Exceptions::TaskSubmissionException(e.what());
        }
    }

    std::size_t TasksHandler::WorkerPool::getActiveWorkerCount() const
    {
        return activeWorkerCount_.load();
    }

    TasksHandler::WorkerPool::Worker::Worker(
        Engine& engine,
        priorityQueue& tasks,
        bool& running_,
        std::mutex& mutex,
        std::condition_variable& condition)
        : 
		currentTask_(nullptr),
        tasks_(tasks),
        running_(running_),
        mutex_(mutex),
        condition_(condition),
        thread_([&engine, this]() { Worker::run(engine, *this); })
    {
        HARMONY_TRACE("Worker thread created");
        HARMONY_ASSERT(thread_.joinable(), "Worker thread must be joinable");
    }

    TasksHandler::WorkerPool::Worker::~Worker()
    {
        HARMONY_TRACE("Worker thread destroying");
        if (thread_.joinable()) {
            thread_.join();
            HARMONY_TRACE("Worker thread joined successfully");
        }
    }

    void TasksHandler::WorkerPool::Worker::run(Engine& engine, Worker& worker)
    {
        HARMONY_ASSERT_NOT_NULL(engine.tasksHandler, "Engine tasksHandler is null");
        HARMONY_ASSERT_NOT_NULL(engine.tasksHandler->workerPool_.get(), "WorkerPool is null");
        
        auto& activeWorkerCountRef = engine.tasksHandler->workerPool_->activeWorkerCount_;
		auto& slowTasksDeleteCondition = engine.tasksHandler->workerPool_->slowTasksDeleteCondition_;

        HARMONY_DEBUG("Worker thread starting");
		activeWorkerCountRef++;
        
        try {
            while (true)
            {
                std::unique_lock<std::mutex> lock(worker.mutex_);
                worker.condition_.wait(lock, [&worker] { return !worker.running_ || !worker.tasks_.empty(); });

                if (!worker.running_ && worker.tasks_.empty()) { 
                    HARMONY_DEBUG("Worker thread exiting gracefully");
                    break; 
                }

                HARMONY_ASSERT(!worker.tasks_.empty(), "Tasks queue should not be empty at this point");

                worker.currentTask_ = worker.tasks_.top(); // move the pointer
                worker.tasks_.pop();                       // remove it from the queue

                lock.unlock();

                if (!worker.currentTask_) {
                    HARMONY_WARN("Worker encountered null task in queue, skipping");
                    continue;
                }

                HARMONY_TRACE("Worker processing task with priority {} and mode {}", 
                             worker.currentTask_->priority, static_cast<int>(worker.currentTask_->mode));

                switch (worker.currentTask_->mode) {
			    case Tasks::Task_t::Mode::FastMultiThreaded:
                    try {
                        runTask(engine, worker.currentTask_);
                        HARMONY_TRACE("FastMultiThreaded task completed successfully");
                    }
                    catch (const Exceptions::HarmonyException& e) {
                        HARMONY_ERROR("FastMultiThreaded task failed with HarmonyException: {}", e.what());
                    }
                    catch (const std::exception& e) {
                        HARMONY_ERROR("FastMultiThreaded task failed with exception: {}", e.what());
                    }
                    catch (...) {
                        HARMONY_CRITICAL("FastMultiThreaded task failed with unknown exception");
                    }
                    break;

                case Tasks::Task_t::Mode::SlowMultiThreaded:
                    HARMONY_DEBUG("Spawning detached thread for SlowMultiThreaded task");
                    std::thread([&engine, &worker, &activeWorkerCountRef, &slowTasksDeleteCondition]() {
					    activeWorkerCountRef++;
                        HARMONY_TRACE("SlowMultiThreaded task thread started");
                        
                        try { 
                            runTask(engine, worker.currentTask_);
                            HARMONY_TRACE("SlowMultiThreaded task completed successfully");
                        }
                        catch (const Exceptions::HarmonyException& e) {
                            HARMONY_ERROR("SlowMultiThreaded task failed with HarmonyException: {}", e.what());
                        }
                        catch (const std::exception& e) {
                            HARMONY_ERROR("SlowMultiThreaded task failed with exception: {}", e.what());
                        }
                        catch (...) {
                            HARMONY_CRITICAL("SlowMultiThreaded task failed with unknown exception");
                        }
                        
                        activeWorkerCountRef--;
					    slowTasksDeleteCondition.notify_all();
                        HARMONY_TRACE("SlowMultiThreaded task thread exiting");
                    }).detach();
                    break;

                default:
                    HARMONY_ERROR("Invalid task mode: {}", static_cast<int>(worker.currentTask_->mode));
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            HARMONY_CRITICAL("Worker thread crashed with exception: {}", e.what());
        }
        catch (...) {
            HARMONY_CRITICAL("Worker thread crashed with unknown exception");
        }
        
		activeWorkerCountRef--;
		slowTasksDeleteCondition.notify_all();
        HARMONY_DEBUG("Worker thread exiting");
    }

    void runTask(Engine& engine, Tasks::Task_t* task) {
        HARMONY_ASSERT_NOT_NULL(task, "Cannot run null task");
        
        if (!task) {
            HARMONY_ERROR("Attempted to run null task");
            throw Exceptions::InvalidTaskException("Task pointer is null");
        }
        
        HARMONY_TRACE("Running task with priority {}", task->priority);
        
        try {
            task->start(engine);
        }
        catch (const Exceptions::HarmonyException& e) {
            HARMONY_ERROR("Task execution failed with HarmonyException: {}", e.what());
            throw Exceptions::TaskExecutionException(
                std::format("priority={}", task->priority), e.what());
        }
        catch (const std::exception& e) {
            HARMONY_ERROR("Task execution failed with exception: {}", e.what());
            throw Exceptions::TaskExecutionException(
                std::format("priority={}", task->priority), e.what());
        }
    }

    TasksHandler::TasksHandler(Engine& engine)
        : engine_(engine), workerPool_(nullptr) {
        HARMONY_INFO("Initializing TasksHandler");
        
        HARMONY_ASSERT_NOT_NULL(&engine, "Engine reference is null");
        
        try {
            workerPool_ = std::make_unique<WorkerPool>(engine);
            HARMONY_INFO("TasksHandler initialized successfully");
        }
        catch (const std::exception& e) {
            HARMONY_CRITICAL("TasksHandler initialization failed: {}", e.what());
            throw;
        }
    }

    TasksHandler::~TasksHandler() {
        HARMONY_INFO("Destroying TasksHandler");
    }

    void TasksHandler::submit(std::unique_ptr<Tasks::Task_t> task)
    {
        HARMONY_ASSERT_NOT_NULL(task.get(), "Cannot submit null task");
        
        if (!task) {
            HARMONY_ERROR("Attempted to submit null task");
            throw Exceptions::InvalidTaskException("Task unique_ptr is null");
        }

        HARMONY_DEBUG("Submitting task with priority {}", task->priority);

        // Immediate execution
        if (task->priority == 0) {
            HARMONY_DEBUG("Task has priority 0, executing immediately");
            handleTask(task.release());
            return;
        }

        try {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(task.release());
            HARMONY_TRACE("Task added to queue");
        }
        catch (const std::exception& e) {
            HARMONY_ERROR("Failed to add task to queue: {}", e.what());
            throw Exceptions::TaskSubmissionException(e.what());
        }
    }

    void TasksHandler::handleTasks()
    {
        HARMONY_TRACE("Handling queued tasks");
        
        while (true) {
            Tasks::Task_t* task;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (tasks_.empty()) {
                    HARMONY_TRACE("No more tasks to handle");
                    break;
                }

                task = tasks_.top();
                tasks_.pop();
            }

            if (!task) {
                HARMONY_WARN("Encountered null task in queue, skipping");
                continue;
            }

            HARMONY_DEBUG("Handling task with priority {}", task->priority);
            handleTask(task);
        }
    }

    void TasksHandler::handleTask(Tasks::Task_t* task)
    {
        HARMONY_ASSERT_NOT_NULL(task, "Cannot handle null task");
        
        if (!task) {
            HARMONY_ERROR("Attempted to handle null task");
            throw Exceptions::InvalidTaskException("Task pointer is null");
        }

        HARMONY_DEBUG("Processing task with mode {}", static_cast<int>(task->mode));

        switch (task->mode) {
        case Tasks::Task_t::Mode::SingleThreaded:
            HARMONY_TRACE("Executing SingleThreaded task");
            try {
			    runTask(engine_, task);
            }
            catch (const std::exception& e) {
                HARMONY_ERROR("SingleThreaded task execution failed: {}", e.what());
                throw;
            }
            break;

        case Tasks::Task_t::Mode::FastMultiThreaded:
        case Tasks::Task_t::Mode::SlowMultiThreaded:
            HARMONY_TRACE("Submitting task to WorkerPool");
            HARMONY_ASSERT_NOT_NULL(workerPool_.get(), "WorkerPool is null");
            workerPool_->submit(task);
            break;

        default:
            HARMONY_ERROR("Invalid task mode: {}", static_cast<int>(task->mode));
            throw Exceptions::InvalidTaskModeException(
                std::format("mode={}", static_cast<int>(task->mode)));
        }
    }

    std::size_t TasksHandler::getActiveWorkerCount() const
    {
        HARMONY_ASSERT_NOT_NULL(workerPool_.get(), "WorkerPool is null");
        
        if (!workerPool_) {
            HARMONY_ERROR("Cannot get active worker count: WorkerPool is null");
            throw Exceptions::WorkerPoolException("getActiveWorkerCount", "WorkerPool is null");
        }
        
        return workerPool_->getActiveWorkerCount();
    }
}