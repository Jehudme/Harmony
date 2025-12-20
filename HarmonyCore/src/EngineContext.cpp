#include "Harmony/EngineContext.h"
#include "Harmony/TaskDispatcher.h"

namespace Harmony {

	EngineContext::EngineContext(Engine& engine, TaskDispatcher& TaskDispatcher) :
		engine(engine), taskDispatcher(TaskDispatcher) {}
}