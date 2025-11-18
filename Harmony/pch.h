#ifndef PCH_H
#define PCH_H

#if defined(_WIN32) || defined(_WIN64)
#define RL_NO_WIN32
#endif

#ifndef NO_RAYLIB
#include <raylib.h>
#endif

#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include <entt/entt.hpp>

#include <shared_mutex>
#include <format>
#include <string>
#include <string_view>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <fstream>
#include <vector>
#include <mutex>
#include <memory>
#include <iostream>
#include <sstream>

#include "Configuration.h"
#include "Logger.h"
#include "Assert.h"

#endif // PCH_H