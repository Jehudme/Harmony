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
        enum Mode
        {
            SingleThreaded,
            FastMultiThreaded,
			SlowMultiThreaded,
		};

    public:
        Task(int priority = 0, Mode mode = SingleThreaded, std::chrono::milliseconds delay = std::chrono::milliseconds(0));
        virtual ~Task();

    protected:
        Engine& getEngine();

    private:
        void start();
        virtual void run() = 0;

    public:
        const Mode mode;
        const int priority;
        const std::chrono::milliseconds delay;

    private:
        std::optional<std::reference_wrapper<Engine>> engine_;
    };

} // namespace Harmony::Internals