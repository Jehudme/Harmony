#pragma once
#include "pch.h"

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <optional>
#include <fstream>
#include <variant>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>
#include <filesystem>
#include <thread>
#include <future>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <spdlog/spdlog.h>

namespace Harmony {
	inline constexpr const char* VERSION = "1.0.0";
	
	class Engine;
}

namespace Harmony::Scenes {
	class State;
	class Scene;
}

namespace Harmony::Tasks {
	struct Task;
}

namespace Harmony::Management {
	class TaskManagemer;
	class StateManager;
	class SceneManager;
}

namespace Harmony::Utilities {
	using UUID = std::uint64_t;
	using UUIDList = std::vector<UUID>;

	class Configuration;
	class Logger;
}
