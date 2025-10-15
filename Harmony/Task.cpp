#include "pch.h"
#include "Task.h"

namespace Harmony::Internals 
{
	Task::Task(int priority, bool multiThreaded, std::chrono::milliseconds delay) :
		priority(priority), delay(delay), multiThreaded(multiThreaded)
	{
	}

	Task::~Task() = default;

	void Task::start()
	{
		std::this_thread::sleep_for(delay);
		run();
	}


}