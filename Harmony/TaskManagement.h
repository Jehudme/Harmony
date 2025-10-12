#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

#include "Task.h"

namespace Harmony::Internals {
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

