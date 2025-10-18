#include "pch.h"
#include "Task.h"

namespace Harmony::Tasks
{
	Task::Task(int priority, Mode mode, std::chrono::milliseconds delay) :
		priority(priority), delay(delay), mode(mode) {}

	Task::~Task() = default;

	Engine& Task::getEngine()
	{
		if (!engine.has_value())
			throw std::runtime_error("Task is not associated with an Engine.");
		return engine->get();
	}

	void Task::start()
	{
		std::this_thread::sleep_for(delay);
		run();
	}


}