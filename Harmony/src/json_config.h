#pragma once

// Configuration header for nlohmann/json library
// This must be included BEFORE nlohmann/json.hpp

#include <cassert>

// Define JSON_ASSERT to use standard assert
// This ensures compatibility across all platforms and compilers
#ifndef JSON_ASSERT
    #define JSON_ASSERT(x) assert(x)
#endif
