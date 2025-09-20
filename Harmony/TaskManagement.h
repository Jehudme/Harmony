#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

namespace Harmony {

    /**
     * @brief Abstract base class representing a unit of work to be executed.
     *
     * Tasks can define their own execution logic, priority, and scheduled start time.
     * They are intended to be managed by TaskManagement.
     */
    struct Task {
        virtual ~Task() = default;

        /**
         * @brief Execute the task's work.
         */
        virtual void execute() = 0;

        /**
         * @brief Task priority value. Higher values run first.
         * @return int Priority level (default: 0).
         */
        virtual int priority() const noexcept { return 0; }

        /**
         * @brief Scheduled start time for the task.
         * @return std::chrono::steady_clock::time_point (default: now).
         */
        virtual std::chrono::steady_clock::time_point start_time() const noexcept {
            return std::chrono::steady_clock::now();
        }

        /**
         * @brief Unique identifier for the task, used for cancellation.
         * @return std::size_t Unique ID (default: pointer value).
         */
        virtual std::size_t id() const noexcept { return reinterpret_cast<std::size_t>(this); }
    };

    /**
     * @brief Manages a collection of tasks, supporting priority scheduling,
     *        delayed execution, cancellation, and thread safety.
     */
    class TaskManagement {
    public:
        using TaskPtr = std::unique_ptr<Task>;

        /**
         * @brief Add a new task to the queue.
         * @param newTask Unique pointer to the task.
         */
        void push(TaskPtr newTask);

        /**
         * @brief Cancel a task by its unique ID before it executes.
         * @param taskIdToRemove ID of the task to cancel.
         * @return true if a task was removed, false otherwise.
         */
        bool cancel(std::size_t taskIdToRemove);

        /**
         * @brief Run all tasks whose scheduled start time has arrived.
         */
        void run_ready();

        /**
         * @brief Run all tasks regardless of their scheduled time.
         */
        void run_all();

        /**
         * @brief Check if the task queue is empty.
         * @return true if no tasks are queued, false otherwise.
         */
        bool empty() const;

    private:
        /**
         * @brief Comparator for ordering tasks in the priority queue.
         *        Higher priority first, then earlier start time.
         */
        struct Compare {
            bool operator()(const TaskPtr& leftTask, const TaskPtr& rightTask) const;
        };

        mutable std::mutex taskQueueMutex_; ///< Mutex to protect task queue access.
        std::priority_queue<TaskPtr, std::vector<TaskPtr>, Compare> taskQueue_; ///< Priority-based task queue.
    };

} // namespace Harmony