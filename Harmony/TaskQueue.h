#pragma once

#include "Object.h"
#include "Task.h"
#include <queue>
#include <memory>
#include <stdexcept>

namespace Harmony {

    // Constants for error messages
    constexpr const char* ERROR_NULL_TASK = "Task cannot be null.";

    class TaskQueue : public Object {
    public:
        explicit TaskQueue(uint64_t uniqueId = 0);

        void push(std::shared_ptr<Task> task);
        void clear();

        bool isEmpty() const;
        void execute();

    private:
        std::queue<std::shared_ptr<Task>> tasks_;
    };
}