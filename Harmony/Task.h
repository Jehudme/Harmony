#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>
#include <future>
#include <thread>

namespace Harmony::Internals
{
    struct Task
    {
        friend class TaskManagement;

    public:
        Task(int priority = 0, bool multithreaded = true, std::chrono::milliseconds delay = std::chrono::milliseconds(0));
        virtual ~Task();

    private:
        void start();
        virtual void run() = 0;

    public:
        const int priority;
        const bool multiThreaded;
        const std::chrono::milliseconds delay;

    protected:
        std::optional<std::reference_wrapper<Engine>> engine;
    };

} // namespace Harmony::Internals