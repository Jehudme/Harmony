// CassertFix.h - MSVC C++20 compatibility fix for nlohmann/json
// This header ensures <cassert> is properly included and assert macro is defined
// before any external headers that use it (like nlohmann/json) are processed.
#pragma once

#include <cassert>

// Define JSON_ASSERT before nlohmann/json is included.
// The nlohmann/json library checks for this macro and uses it if defined,
// otherwise it includes <cassert> and defines its own. By defining it here,
// we ensure the assert macro is visible during template instantiation.
#ifndef JSON_ASSERT
#define JSON_ASSERT(x) assert(x)
#endif
