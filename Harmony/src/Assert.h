#pragma once

#include "Logger.h"
#include <cstdlib>

namespace Harmony::Utilities {

    // Internal assertion handler that logs and optionally aborts
    inline void assertHandler(
        bool condition,
        const char* conditionStr,
        const char* message,
        const char* file,
        int line,
        bool critical = false
    ) {
        if (!condition) {
            if (message && message[0] != '\0') {
                Logger::error("Assertion failed: {} | {} | File: {} | Line: {}", 
                    conditionStr, message, file, line);
            } else {
                Logger::error("Assertion failed: {} | File: {} | Line: {}", 
                    conditionStr, file, line);
            }

            // In debug mode or for critical assertions, abort the program
            #if defined(_DEBUG) || defined(DEBUG)
            std::abort();
            #else
            if (critical) {
                std::abort();
            }
            #endif
        }
    }

    // Warning assertion handler that logs but never aborts
    inline void assertWarnHandler(
        bool condition,
        const char* conditionStr,
        const char* message,
        const char* file,
        int line
    ) {
        if (!condition) {
            if (message && message[0] != '\0') {
                Logger::warn("Assertion warning: {} | {} | File: {} | Line: {}", 
                    conditionStr, message, file, line);
            } else {
                Logger::warn("Assertion warning: {} | File: {} | Line: {}", 
                    conditionStr, file, line);
            }
        }
    }

    // Critical assertion handler that always aborts on failure
    inline void assertCriticalHandler(
        bool condition,
        const char* conditionStr,
        const char* message,
        const char* file,
        int line
    ) {
        if (!condition) {
            if (message && message[0] != '\0') {
                Logger::critical("Critical assertion failed: {} | {} | File: {} | Line: {}", 
                    conditionStr, message, file, line);
            } else {
                Logger::critical("Critical assertion failed: {} | File: {} | Line: {}", 
                    conditionStr, file, line);
            }
            std::abort();
        }
    }
}

// Main assertion macros

// Standard assertion - aborts in debug builds only
#define HARMONY_ASSERT(condition, ...) \
    Harmony::Utilities::assertHandler((condition), #condition, "" __VA_ARGS__, __FILE__, __LINE__, false)

// Warning assertion - only logs, never aborts
#define HARMONY_ASSERT_WARN(condition, ...) \
    Harmony::Utilities::assertWarnHandler((condition), #condition, "" __VA_ARGS__, __FILE__, __LINE__)

// Critical assertion - always aborts on failure (even in release builds)
#define HARMONY_ASSERT_CRITICAL(condition, ...) \
    Harmony::Utilities::assertCriticalHandler((condition), #condition, "" __VA_ARGS__, __FILE__, __LINE__)

// Debug-only assertion - completely removed in release builds
#if defined(_DEBUG) || defined(DEBUG)
    #define HARMONY_ASSERT_DEBUG(condition, ...) \
        Harmony::Utilities::assertHandler((condition), #condition, "" __VA_ARGS__, __FILE__, __LINE__, false)
#else
    #define HARMONY_ASSERT_DEBUG(condition, ...) ((void)0)
#endif

// Convenience macros for common use cases

// Assert that a pointer is not null
#define HARMONY_ASSERT_NOT_NULL(ptr, ...) \
    HARMONY_ASSERT((ptr) != nullptr, "" __VA_ARGS__)

// Assert that a value is within a range
#define HARMONY_ASSERT_RANGE(value, min, max, ...) \
    HARMONY_ASSERT((value) >= (min) && (value) <= (max), "" __VA_ARGS__)

// Assert that two values are equal
#define HARMONY_ASSERT_EQUAL(a, b, ...) \
    HARMONY_ASSERT((a) == (b), "" __VA_ARGS__)

// Assert that two values are not equal
#define HARMONY_ASSERT_NOT_EQUAL(a, b, ...) \
    HARMONY_ASSERT((a) != (b), "" __VA_ARGS__)
