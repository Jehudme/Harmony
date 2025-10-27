#pragma once
#include "pch.h"
#include "entt/fwd.hpp"

namespace Harmony {
	inline constexpr const char* VERSION = "1.0.0";
	
	class Engine;

	using EntityID = entt::entity;
}

namespace Harmony::Components {
	class Component;
	class Transform;
	class Drawable;
}

namespace Harmony::Scenes {
	class State;
	class Scene;
}

namespace Harmony::Tasks {
	struct Task;
}

namespace Harmony::Management {
	class ComponentManager;
	class TaskManager;
	class StateManager;
	class SceneManager;
}

namespace Harmony::Utilities {
	using UUID = std::uint64_t;
	using UUIDList = std::vector<UUID>;

	class Configuration;
	class Logger;
}

