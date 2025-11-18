#include "pch.h"
#include "Task.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Tasks
{
	Task_t::Task_t(uint16_t priority, Mode mode)
		: mode(mode), priority(static_cast<uint16_t>(priority)) {
		HARMONY_TRACE("Task created with priority {} and mode {}", priority, static_cast<int>(mode));
		
		// Validate mode
		HARMONY_ASSERT(
			mode == Mode::SingleThreaded || 
			mode == Mode::FastMultiThreaded || 
			mode == Mode::SlowMultiThreaded,
			"Invalid task mode");
	}

	Task_t::~Task_t() {
		HARMONY_TRACE("Task with priority {} destroyed", priority);
	}

	void Task_t::start(Internals::Engine& engine) {
		HARMONY_TRACE("Starting task with priority {}", priority);
		
		try {
			execute(engine);
		}
		catch (const Exceptions::HarmonyException& e) {
			HARMONY_ERROR("Task execution failed with HarmonyException: {}", e.what());
			throw;
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Task execution failed with exception: {}", e.what());
			throw;
		}
	}
}