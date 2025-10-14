#include "pch.h"
#include "TaskManagement.h"
#include "Engine.h"
#include "Task.h"

namespace Harmony::Internals {
	TaskManagement::TaskManagement(Engine& engine) :
		engine(engine) {
	}
	TaskManagement::~TaskManagement() = default;
	void TaskManagement::submit(std::unique_ptr<Task> task) {

		if (task->priority == 0)
		{
			task->start(); 
			return;
		}

		tasks_.emplace(std::move(task));
	}

	void TaskManagement::handleTasks() {
		const auto now = std::chrono::steady_clock::now();
		while (!tasks_.empty()) {
			auto& task = tasks_.top();
			if (task->delay > std::chrono::milliseconds(0)) {
				break; // Task is still delayed
			}
			task->run();
			tasks_.pop();
		}
	}


	bool TaskManagement::Compare::operator()(const std::unique_ptr<Task>& leftTask, const std::unique_ptr<Task>& rightTask) const {
		return leftTask->priority < rightTask->priority;
	}
}

