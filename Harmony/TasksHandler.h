#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

#include "Task.h"
#include "Exceptions.h"

namespace Harmony::Internals
{
	class TasksHandler
	{
	public:
		friend class Engine;

		TasksHandler(Engine& engine);
		~TasksHandler();

		void submit(std::unique_ptr<Tasks::Task_t> task);

		/// Get the number of currently active worker threads
		std::size_t getActiveWorkerCount() const;

	private:
		struct WorkerPool;
		struct Compare
		{
			bool operator()(const Tasks::Task_t* leftTask,
				const Tasks::Task_t* rightTask) const
			{
				if (leftTask->mode == rightTask->mode) 
					return leftTask->priority > rightTask->priority;
				return leftTask->mode < rightTask->mode;
			}
		};

		using priorityQueue = std::priority_queue<Tasks::Task_t*, std::vector<Tasks::Task_t*>, Compare>;

		void start();
		void stop();

		void handleTasks();
		void handleTask(Tasks::Task_t* task);

		bool running_;

		Engine& engine_;
		std::mutex mutex_;
		priorityQueue tasks_;
		std::unique_ptr<WorkerPool> workerPool_;
	};
}