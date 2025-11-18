#include "pch.h"
#include "TasksHandler.h"

#include "Engine.h"
#include "Task.h"
#include "Logger.h"

namespace Harmony::Internals
{
    static void runTask(Engine& engine, Tasks::Task_t* task);

    struct TasksHandler::WorkerPool 
    {
    public:
        WorkerPool(Engine& engine);
        ~WorkerPool();

        void submit(Tasks::Task_t* task);

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
        unsigned int workerCount = std::thread::hardware_concurrency();
		if (workerCount == 0) workerCount = 4; // Fallback to 4 workers if unable to detect

        for (unsigned int workerIndex = 0; workerIndex < workerCount; workerIndex++) {
            workers_.emplace_back(engine, tasks_, running_, mutex_, condition_);
        }
    }

    TasksHandler::WorkerPool::~WorkerPool()
    {
        running_ = false;
        condition_.notify_all();
		workers_.clear();
		std::unique_lock<std::mutex> lock(slowTasksDeleteMutex_);
		slowTasksDeleteCondition_.wait(lock, [this]() { return activeWorkerCount_ == 0; });
    }

    void TasksHandler::WorkerPool::submit(Tasks::Task_t* task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(std::move(task));
        }
        condition_.notify_one();
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
    }

    TasksHandler::WorkerPool::Worker::~Worker()
    {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void TasksHandler::WorkerPool::Worker::run(Engine& engine, Worker& worker)
    {
        auto& activeWorkerCountRef = engine.tasksHandler->workerPool_->activeWorkerCount_;
		auto& slowTasksDeleteCondition = engine.tasksHandler->workerPool_->slowTasksDeleteCondition_;

		activeWorkerCountRef++;
        while (true)
        {
            std::unique_lock<std::mutex> lock(worker.mutex_);
            worker.condition_.wait(lock, [&worker] { return !worker.running_ || !worker.tasks_.empty(); });

            if (!worker.running_ && worker.tasks_.empty()) { return; }

            worker.currentTask_ = worker.tasks_.top(); // move the pointer
            worker.tasks_.pop();                       // remove it from the queue

            lock.unlock();

            if (!worker.currentTask_) {
                continue;
            }

            switch (worker.currentTask_->mode) {
			case Tasks::Task_t::Mode::FastMultiThreaded:
                runTask(engine, worker.currentTask_);
                break;

            case Tasks::Task_t::Mode::SlowMultiThreaded:
                std::thread([&engine, &worker, &activeWorkerCountRef, &slowTasksDeleteCondition]() {
					activeWorkerCountRef++;
                    try { runTask(engine, worker.currentTask_); }
                    catch (const std::exception&) {} // add good exception handling here and logging
                    activeWorkerCountRef--;
					slowTasksDeleteCondition.notify_all();
                }).detach();
                break;

            default:
                break;
            }
        }
		activeWorkerCountRef--;
		slowTasksDeleteCondition.notify_all();
    }

    void runTask(Engine& engine, Tasks::Task_t* task) {
        task->start(engine);
    }

    TasksHandler::TasksHandler(Engine& engine)
        : engine_(engine), workerPool_(std::make_unique<WorkerPool>(engine)) {
    }

    TasksHandler::~TasksHandler() {
    }

    void TasksHandler::submit(std::unique_ptr<Tasks::Task_t> task)
    {
        // Immediate execution
        if (task->priority == 0) {
            handleTask(task.release());
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.emplace(task.release());
    }

    void TasksHandler::handleTasks()
    {
        while (true) {
            Tasks::Task_t* task;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (tasks_.empty())  break;

                task = tasks_.top();
                tasks_.pop();
            }

            if (!task) continue;

            handleTask(task);
        }
    }

    void TasksHandler::handleTask(Tasks::Task_t* task)
    {
        switch (task->mode) {
        case Tasks::Task_t::Mode::SingleThreaded:
			runTask(engine_, task);
            break;

        case Tasks::Task_t::Mode::FastMultiThreaded:
        case Tasks::Task_t::Mode::SlowMultiThreaded:
            workerPool_->submit(task);
            break;

        default:
            break;
        }
    }
}