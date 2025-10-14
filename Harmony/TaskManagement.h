#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <chrono>
#include <vector>

namespace Harmony::Internals {

    class TaskManagement {
		friend class Engine;

    public:
		TaskManagement(Engine& engine);
		~TaskManagement();

		void submit(std::unique_ptr<Task> task);

	private:
		void handleTasks();

		struct Compare {
			bool operator()(const std::unique_ptr<Task>& leftTask, const std::unique_ptr<Task>& rightTask) const;
		};

	private:
		Engine& engine;
		std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, Compare> tasks_;
    };

} // namespace Harmony