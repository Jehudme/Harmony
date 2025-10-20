#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

#include "Task.h"

namespace Harmony::Management
{
    class TaskManagemer
	{
    public:
		friend class Engine;
		friend struct WorkerPool;

		TaskManagemer(Engine& engine);
		~TaskManagemer();

		void submit(std::unique_ptr<Tasks::Task> task);

	private:
		struct WorkerPool;
		struct Compare
		{
			bool operator()(const std::unique_ptr<Tasks::Task>& leftTask, const std::unique_ptr<Tasks::Task>& rightTask) const
			{
				if (leftTask->priority == rightTask->priority)
					return leftTask->delay > rightTask->delay;

				return leftTask->priority < rightTask->priority;
			}
		};

		void handleTasks();
		void handleTask(std::unique_ptr<Tasks::Task> task);

	private:
		Engine& engine;
		std::mutex mutex_;
		std::unique_ptr<WorkerPool> workerPool_;
		std::priority_queue<std::unique_ptr<Tasks::Task>, std::vector<std::unique_ptr<Tasks::Task>>, Compare> tasks_;
    };

} // namespace Harmony
