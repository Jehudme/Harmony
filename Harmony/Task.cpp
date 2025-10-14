#include "pch.h"
#include "Task.h"

namespace Harmony::Internals {
	Task::Task(int priority, std::chrono::milliseconds delay) :
		priority(priority), delay(delay) {}

	Task::~Task() = default;
}