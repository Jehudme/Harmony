#pragma once

#include <cstdlib> 

// ---------------------------------------------------------
// FIX: Manually define JSON_ASSERT to bypass the missing system assert
// This tells the library: "If 'x' is false, stop the program."
// ---------------------------------------------------------
#define JSON_ASSERT(x) if (!(x)) std::abort()