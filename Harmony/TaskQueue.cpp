#include "pch.h"
#include "TaskQueue.h"

namespace Harmony {

    TaskQueue::TaskQueue(uint64_t uniqueId)
        : Object(uniqueId) {
    }

    void TaskQueue::push(std::shared_ptr<Task> task) {
        if (!task) {
            throw std::invalid_argument(ERROR_NULL_TASK);
        }
        tasks_.push(task);
    }

    void TaskQueue::clear() {
        std::queue<std::shared_ptr<Task>> emptyQueue;
        std::swap(tasks_, emptyQueue);
    }

    bool TaskQueue::isEmpty() const {
        return tasks_.empty();
    }

    void TaskQueue::execute() {
        while (!tasks_.empty()) {
            auto task = tasks_.front();
            if (task) {
                task->execute();
            }
            tasks_.pop();
        }
    }
}
