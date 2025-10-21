#pragma once

namespace Harmony::Tasks
{
    struct Task
    {
        enum Mode
        {
            SingleThreaded,
            FastMultiThreaded,
			SlowMultiThreaded,
		};

    public:
        Task(int priority = 0, Mode mode = SingleThreaded, std::chrono::milliseconds delay = std::chrono::milliseconds(0));
        virtual ~Task();

        void start();

    protected:
        Engine& getEngine();

    private:
        virtual void run() = 0;

    public:
        const Mode mode;
        const int priority;
        const Utilities::UUID id;
        const std::chrono::milliseconds delay;
        std::optional<std::reference_wrapper<Engine>> engine;
    };

}