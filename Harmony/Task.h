#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

namespace Harmony::Internals {

    struct Task 
	{
		friend class TaskManagement;

    public:
		Task(int priority = 0, std::chrono::milliseconds delay = std::chrono::milliseconds(0));
		~Task();

    private:
		virtual void start() = 0;
        virtual void run() = 0;

	public:
		const int priority;
		const std::chrono::milliseconds delay;
    };
}