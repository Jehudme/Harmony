#include "pch.h"
#include "Task.h"

namespace Harmony::Tasks
{
	Task_t::Task_t(uint16_t priority, Mode mode)
		: mode(mode), priority(static_cast<uint16_t>(priority)) {}

	Task_t::~Task_t() = default;

	void Task_t::start(Internals::Engine& engine) { execute(engine); }
}