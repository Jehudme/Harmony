#pragma once
#include "Object.h"
#include "Task.h"
#include <queue>
namespace Harmony
{
	class TaskQueue : Object
	{
	public:
		TaskQueue(const uint64_t& uniqueId = NULL);

		void push(std::shared_ptr<Task> task);
		void clean();

		bool isEmpty() const;
		void execute();

	private:
		std::queue<std::shared_ptr<Task>> m_tasks;
	};
}

