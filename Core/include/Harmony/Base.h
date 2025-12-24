#pragma once

// --- 1. Platform Detection ---
#ifdef _WIN32
    // Windows (x64 and x86)
#define HARMONY_PLATFORM_WINDOWS
#elif defined(__EMSCRIPTEN__)
    // WebAssembly (Emscripten)
#define HARMONY_PLATFORM_WEB
#elif defined(__linux__)
    // Linux
#define HARMONY_PLATFORM_LINUX
#else
#error "Unknown Platform! Harmony only supports Windows, Linux, and WebAssembly."
#endif

// --- 2. DLL Export/Import Macros ---
// This handles the "Hybrid" logic: Static for Web/Release, Dynamic for Desktop Dev.
// Usage: class HARMONY_API MyClass { ... };

#if defined(HARMONY_STATIC_LINK) || defined(HARMONY_PLATFORM_WEB)
    // Static Linking (Web/Console) -> No DLL attributes needed
#define HARMONY_API
#else
    // Dynamic Linking (Windows/Linux Dev)
#ifdef HARMONY_BUILD_DLL
    // We are currently compiling the Library/Plugin -> EXPORT symbols
#ifdef _MSC_VER
#define HARMONY_API __declspec(dllexport)
#else
#define HARMONY_API __attribute__((visibility("default")))
#endif
#else
    // We are using the Library/Plugin -> IMPORT symbols
#ifdef _MSC_VER
#define HARMONY_API __declspec(dllimport)
#else
#define HARMONY_API
#endif
#endif
#endif

// --- 3. Debug Break Helper ---
// Useful for writing your own custom checks outside of Assert.h
#ifdef HARMONY_DEBUG
#if defined(HARMONY_PLATFORM_WINDOWS)
#define HARMONY_DEBUGBREAK() __debugbreak()
#elif defined(HARMONY_PLATFORM_LINUX)
#include <signal.h>
#define HARMONY_DEBUGBREAK() raise(SIGTRAP)
#else
#define HARMONY_DEBUGBREAK()
#endif
#else
#define HARMONY_DEBUGBREAK()
#endif

// --- 4. BIT (Bitwise) Macro ---
// Easy way to define flags: 1 << x
#define BIT(x) (1 << x)

// --- 5. Standard Type Aliases ---
// Shorthand for fixed-width integers
#include <cstdint>

namespace Harmony {
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
}


// --- 6. Include Assertions ---
// Pull in your existing Assertion system so it's available everywhere Base.h is included
#include "Harmony/Assert.h"