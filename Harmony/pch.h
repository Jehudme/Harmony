// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include "fwd.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <fmt/Core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include <entt/entt.hpp>
#include <shared_mutex>
#include <format>
#include <string>
#include <string_view>
#include <cstddef>

#include "Logger.h"
#include "Assert.h"

#endif //PCH_H
