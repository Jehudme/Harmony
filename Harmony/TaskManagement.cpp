#include "pch.h"
#include "pch.h"
#include "TaskManagement.h"

namespace Harmony {

    // Comparator for the priority queue:
    // - Higher priority value executes first
    // - If priorities are equal, earlier start time executes first
    bool TaskManagement::Compare::operator()(const TaskPtr& leftTask, const TaskPtr& rightTask) const {
        if (leftTask->priority() != rightTask->priority()) {
            return leftTask->priority() < rightTask->priority(); // Higher priority first
        }
        return leftTask->start_time() > rightTask->start_time(); // Earlier time first
    }

    // Add a new task to the queue
    void TaskManagement::push(TaskPtr newTask) {
        std::unique_lock<std::mutex> queueLock(taskQueueMutex_);
        taskQueue_.push(std::move(newTask));
    }

    // Cancel a task by its unique ID before it runs
    bool TaskManagement::cancel(std::size_t taskIdToRemove) {
        std::unique_lock<std::mutex> queueLock(taskQueueMutex_);
        bool wasRemoved = false;
        std::vector<TaskPtr> remainingTasks;

        // Move all tasks out, skipping the one to remove
        while (!taskQueue_.empty()) {
            TaskPtr currentTask = std::move(const_cast<TaskPtr&>(taskQueue_.top()));
            taskQueue_.pop();

            if (currentTask->id() != taskIdToRemove) {
                remainingTasks.push_back(std::move(currentTask));
            }
            else {
                wasRemoved = true;
            }
        }

        // Push remaining tasks back into the queue
        for (auto& task : remainingTasks) {
            taskQueue_.push(std::move(task));
        }

        return wasRemoved;
    }

    // Run only tasks whose scheduled start time has arrived
    void TaskManagement::run_ready() {
        std::unique_lock<std::mutex> queueLock(taskQueueMutex_);
        const auto currentTime = std::chrono::steady_clock::now();

        while (!taskQueue_.empty()) {
            Task& nextTask = *taskQueue_.top();

            // Stop if the next task is scheduled for the future
            if (nextTask.start_time() > currentTime) {
                break;
            }

            // Take ownership of the task
            TaskPtr taskToExecute = std::move(const_cast<TaskPtr&>(taskQueue_.top()));
            taskQueue_.pop();

            // Unlock while executing to allow concurrent push/cancel
            queueLock.unlock();
            taskToExecute->execute();
            queueLock.lock();
        }
    }

    // Run all tasks regardless of their scheduled time
    void TaskManagement::run_all() {
        std::unique_lock<std::mutex> queueLock(taskQueueMutex_);

        while (!taskQueue_.empty()) {
            TaskPtr taskToExecute = std::move(const_cast<TaskPtr&>(taskQueue_.top()));
            taskQueue_.pop();

            queueLock.unlock();
            taskToExecute->execute();
            queueLock.lock();
        }
    }

    // Check if the task queue is empty
    bool TaskManagement::empty() const {
        std::unique_lock<std::mutex> queueLock(taskQueueMutex_);
        return taskQueue_.empty();
    }

} // namespace Harmony