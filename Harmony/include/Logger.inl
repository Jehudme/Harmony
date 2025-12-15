// Define log levels
#include "Exceptions.h"

#define HARMONY_LOG_LEVEL_TRACE    0
#define HARMONY_LOG_LEVEL_DEBUG    1
#define HARMONY_LOG_LEVEL_INFO     2
#define HARMONY_LOG_LEVEL_WARN     3
#define HARMONY_LOG_LEVEL_ERROR    4
#define HARMONY_LOG_LEVEL_CRITICAL 5

// Set the active log level (default: INFO)
#ifndef HARMONY_LOG_LEVEL
#define HARMONY_LOG_LEVEL HARMONY_LOG_LEVEL_INFO
#endif

// Conditional macros
#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_TRACE
#define HARMONY_TRACE(...)    Harmony::Logger::trace(__VA_ARGS__)
#else
#define HARMONY_TRACE(...)    ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_DEBUG
#define HARMONY_DEBUG(...)    Harmony::Logger::debug(__VA_ARGS__)
#else
#define HARMONY_DEBUG(...)    ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_INFO
#define HARMONY_INFO(...)     Harmony::Logger::info(__VA_ARGS__)
#else
#define HARMONY_INFO(...)     ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_WARN
#define HARMONY_WARN(...)     Harmony::Logger::warn(__VA_ARGS__)
#else
#define HARMONY_WARN(...)     ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_ERROR
#define HARMONY_ERROR(...)    Harmony::Logger::error(__VA_ARGS__)
#else
#define HARMONY_ERROR(...)    ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_CRITICAL
#define HARMONY_CRITICAL(...) Harmony::Logger::critical(__VA_ARGS__)
#else
#define HARMONY_CRITICAL(...) ((void)0)
#endif

namespace Harmony
{
    template<typename... Args>
    void Logger::trace(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        trace(s);
    }

    template<typename... Args>
    void Logger::debug(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        debug(s);
    }

    template<typename... Args>
    void Logger::info(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        info(s);
    }

    template<typename... Args>
    void Logger::warn(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        warn(s);
    }

    template<typename... Args>
    void Logger::error(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        error(s);
    }

    template<typename... Args>
    void Logger::critical(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        critical(s);
    }
}