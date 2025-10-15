#include "pch.h"
#include "Engine.h"
#include "EngineTask.h"

namespace Harmony::Tasks
{
	SetFPS::SetFPS(const unsigned int targetFPS) :
		Task(0, true), targetFPS_(targetFPS) {}

	GetFPS::GetFPS() :
		Task(0, true)
	{}

}
