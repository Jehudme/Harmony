#pragma once
#include "pch.h"

namespace Harmony::Internals {
	class Configuration;
	class Engine;
	class TaskManagement;
	class StateManagement;
	class SceneManagement;
	class State;
	class Scene;
	struct Task;
}

namespace Harmony::Utilities {
	using UUID = std::uint64_t;
	using UUIDList = std::vector<UUID>;
}