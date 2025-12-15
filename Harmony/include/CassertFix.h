// CassertFix.h - MSVC C++20 compatibility fix for nlohmann/json
// This header ensures <cassert> is properly included and assert macro is defined
// before any external headers that use it (like nlohmann/json) are processed.
#pragma once

#include <cassert>

// Ensure JSON_ASSERT is properly defined for nlohmann/json
#ifndef JSON_ASSERT
#define JSON_ASSERT(x) assert(x)
#endif
