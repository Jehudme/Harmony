#include "pch.h"
#include "TaskQueue.h"

namespace Harmony 
{
    TaskQueue::TaskQueue(const uint64_t& uniqueId)
        : Object(uniqueId) {
    }

    void TaskQueue::push(std::shared_ptr<Task> task) {
        m_tasks.push(task);
    }

    void TaskQueue::clean() {
        std::queue<std::shared_ptr<Task>> empty;
        std::swap(m_tasks, empty);
    }

    bool TaskQueue::isEmpty() const {
        return m_tasks.empty();
    }

    void TaskQueue::execute() {
        while (!m_tasks.empty()) {
            auto task = m_tasks.front();
            task->execute();
            m_tasks.pop();
        }
    }
}
