#include "pch.h"
#include "Task.h"
#include "RandomValue.h"
#include "Exceptions.h"

namespace Harmony::Tasks
{
	Task::Task(int priority, Mode mode, std::chrono::milliseconds delay) :
		priority(priority), delay(delay), mode(mode), id(Utilities::random_value<Utilities::UUID>()) {}

	Task::~Task() = default;

	Engine& Task::getEngine()
	{
		if (!engine.has_value())
			throw Exceptions::TaskNotAssociatedWithEngineException();
		return engine->get();
	}

	void Task::start()
	{
		std::this_thread::sleep_for(delay);
		HARMONY_DEBUG("Executing task (mode = {}, id = {})", static_cast<int>(mode), id);
		run();
		HARMONY_DEBUG("Task executed successfully (mode = {}, id = {})", static_cast<int>(mode), id);
	}


}