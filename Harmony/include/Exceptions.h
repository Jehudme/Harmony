#pragma once
#include <stdexcept>
#include <string>
#include <format>

// ============================================================================
// 1. CONFIGURATION MACRO
// ============================================================================
// Set this to 0 to hide file paths (cleaner for Release builds)
// Set this to 1 to show file paths (better for Debugging)
#ifndef HARMONY_SHOW_EXCEPTION_LOCATION
#define HARMONY_SHOW_EXCEPTION_LOCATION 1
#endif

namespace Harmony {

    class EngineException : public std::runtime_error {
    public:
        // Constructor accepts file and line (can be null/0 if disabled)
        template<typename... Args>
        EngineException(const char* file, int line, std::string_view fmt, Args&&... args)
            : std::runtime_error(FormatMessage(file, line, fmt, std::forward<Args>(args)...)) {}

    private:
        template<typename... Args>
        static std::string FormatMessage(const char* file, int line, std::string_view fmt, Args&&... args) {

            // 1. Format the user message
            std::string userMsg = std::vformat(fmt, std::make_format_args(args...));

            // 2. Conditionally append file/line info
            if (file && line > 0) {
                return std::format("{} (at {}:{})", userMsg, file, line);
            }

            return userMsg;
        }
    };
}

// ============================================================================
// 2. EXECUTION MACRO
// ============================================================================

#if HARMONY_SHOW_EXCEPTION_LOCATION
#define HARMONY_THROW(fmt, ...) \
        throw Harmony::EngineException(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define HARMONY_THROW(fmt, ...) \
        throw Harmony::EngineException(nullptr, 0, fmt, ##__VA_ARGS__)
#endif