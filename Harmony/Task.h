#pragma once
#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

namespace Harmony::Internals {

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
}