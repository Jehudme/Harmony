#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

#include "Task.h"

namespace Harmony::Internals 
{
    class TaskManagement 
	{
    public:
		friend class Engine;

		TaskManagement(Engine& engine_);
		~TaskManagement();

		void submit(std::unique_ptr<Task> task);

	private:
		struct WorkerPool;
		struct Compare
		{
			bool operator()(const std::unique_ptr<Task>& leftTask, const std::unique_ptr<Task>& rightTask) const
			{
				if (leftTask->priority == rightTask->priority)
					return leftTask->delay > rightTask->delay;

				return leftTask->priority < rightTask->priority;
			}
		};

		void handleTasks();
		void handleTask(std::unique_ptr<Task> task);

	private:
		Engine& engine_;
		std::mutex mutex_;
		std::unique_ptr<WorkerPool> workerPool_;
		std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, Compare> tasks_;
    };

} // namespace Harmony