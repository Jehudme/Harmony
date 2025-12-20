#include "Harmony/EngineContext.h"
#include "Harmony/TaskDispatcher.h"

namespace Harmony {

	EngineContext::EngineContext(Engine& engine, TaskDispatcher& TaskDispatcher) :
		engine(&engine), taskDispatcher(&TaskDispatcher) {}

	EngineContext::EngineContext(const EngineContext& instance)
	{
		engine = instance.engine;
		taskDispatcher = instance.taskDispatcher;
	}

	EngineContext& EngineContext::operator=(const EngineContext& rhs)
	{
		engine = rhs.engine;
		taskDispatcher = rhs.taskDispatcher;

		return *this;
	}
}