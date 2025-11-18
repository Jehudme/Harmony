#pragma once

namespace Harmony::Tasks {
	struct Task_t;
}

namespace Harmony::Internals {
    void runTask(class Engine& engine, Tasks::Task_t* task);
}

namespace Harmony::Tasks
{
    struct Task_t
    {
		friend Internals::Engine;
		friend void Internals::runTask(Internals::Engine& engine, Tasks::Task_t* task);

        enum class Mode { SingleThreaded, SlowMultiThreaded, FastMultiThreaded };

    public:
        Task_t(uint16_t priority = 0, Mode mode = Mode::SingleThreaded);
        virtual ~Task_t();

    private:
        void start(Internals::Engine& engine);
        virtual void execute(Internals::Engine& engine) = 0;

    public:
        const Mode mode;
        const uint16_t priority;
    };
}