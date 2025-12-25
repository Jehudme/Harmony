// ========================================================
// Log Level Definitions
// ========================================================

#pragma once

#define HARMONY_LOG_LEVEL_TRACE    0
#define HARMONY_LOG_LEVEL_DEBUG    1
#define HARMONY_LOG_LEVEL_INFO     2
#define HARMONY_LOG_LEVEL_WARN     3
#define HARMONY_LOG_LEVEL_ERROR    4
#define HARMONY_LOG_LEVEL_CRITICAL 5

#ifndef HARMONY_LOG_LEVEL
#define HARMONY_LOG_LEVEL HARMONY_LOG_LEVEL_INFO
#endif

// ========================================================
// Conditional Logging Macros
// ========================================================

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_TRACE
#define HARMONY_TRACE(...)    Harmony::Logger::Trace(__VA_ARGS__)
#else
#define HARMONY_TRACE(...)    ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_DEBUG
#define HARMONY_DEBUG(...)    Harmony::Logger::Debug(__VA_ARGS__)
#else
#define HARMONY_DEBUG(...)    ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_INFO
#define HARMONY_INFO(...)     Harmony::Logger::Info(__VA_ARGS__)
#else
#define HARMONY_INFO(...)     ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_WARN
#define HARMONY_WARN(...)     Harmony::Logger::Warn(__VA_ARGS__)
#else
#define HARMONY_WARN(...)     ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_ERROR
#define HARMONY_ERROR(...)    Harmony::Logger::Error(__VA_ARGS__)
#else
#define HARMONY_ERROR(...)    ((void)0)
#endif

#if HARMONY_LOG_LEVEL <= HARMONY_LOG_LEVEL_CRITICAL
#define HARMONY_CRITICAL(...) Harmony::Logger::Critical(__VA_ARGS__)
#else
#define HARMONY_CRITICAL(...) ((void)0)
#endif

// ========================================================
// Logger Template Implementations
// ========================================================

namespace Harmony
{
    template<typename... Args>
    void Logger::Trace(std::string_view fmt, Args&&... arguments) 
    {
        auto formattedString = std::vformat(fmt, std::make_format_args(arguments...));
        Trace(formattedString);
    }

    template<typename... Args>
    void Logger::Debug(std::string_view fmt, Args&&... arguments) 
    {
        auto formattedString = std::vformat(fmt, std::make_format_args(arguments...));
        Debug(formattedString);
    }

    template<typename... Args>
    void Logger::Info(std::string_view fmt, Args&&... arguments) 
    {
        auto formattedString = std::vformat(fmt, std::make_format_args(arguments...));
        Info(formattedString);
    }

    template<typename... Args>
    void Logger::Warn(std::string_view fmt, Args&&... arguments) 
    {
        auto formattedString = std::vformat(fmt, std::make_format_args(arguments...));
        Warn(formattedString);
    }

    template<typename... Args>
    void Logger::Error(std::string_view fmt, Args&&... arguments) 
    {
        auto formattedString = std::vformat(fmt, std::make_format_args(arguments...));
        Error(formattedString);
    }

    template<typename... Args>
    void Logger::Critical(std::string_view fmt, Args&&... arguments) 
    {
        auto formattedString = std::vformat(fmt, std::make_format_args(arguments...));
        Critical(formattedString);
    }

} // namespace Harmony