# JSON Configuration Header

## Purpose

The `json_config.h` header ensures proper configuration of the nlohmann/json library across all platforms and compilers.

## The Problem

The nlohmann/json library uses `assert()` internally for runtime checks. With MSVC's conforming preprocessor (`/Zc:preprocessor`) and C++20's strict two-phase name lookup, the `assert` macro may not be visible during template instantiation, causing compilation errors like:

```
error C3861: 'assert': identifier not found
```

## The Solution

The `json_config.h` header:
1. Includes `<cassert>` to make the `assert` macro available
2. Defines `JSON_ASSERT(x)` to use the standard `assert(x)` macro
3. Must be included BEFORE `nlohmann/json.hpp`

## Usage

In any source file that uses nlohmann/json:

```cpp
// CORRECT - json_config.h FIRST
#include "json_config.h"
#include <nlohmann/json.hpp>

// Your code using nlohmann::json
```

**DO NOT** include nlohmann/json.hpp before json_config.h:

```cpp
// WRONG - This may cause compilation errors on MSVC
#include <nlohmann/json.hpp>
#include "json_config.h"
```

## Testing

This fix has been tested on:
- ✓ Linux (GCC) with C++20 - all tests pass
- ⚠ MSVC with C++20/latest - requires Windows build environment for verification
  - This fix directly addresses the MSVC compilation error by ensuring assert is available
  - The approach follows standard practices for configuring nlohmann/json

## References

- Original issue: MSVC C++20 compilation errors with nlohmann/json
- Related files: `Harmony/src/Properties.cpp`
- Historical: `Harmony/include/CassertFix.h` contains notes about a previous (unsuccessful) approach
