#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

// Enable or disable logging macros by commenting/uncommenting
#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING
#define LOG_TRACE(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::critical, __VA_ARGS__)
#else
#define LOG_TRACE(...)
#define LOG_DEBUG(logger, ...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#endif

namespace Harmony
{
    class Logger {
    public:
        static void initialize(const std::string& logFilePath, spdlog::level::level_enum globalLevel = spdlog::level::info);

        static std::shared_ptr<spdlog::logger> createLogger(const std::string& loggerName, spdlog::level::level_enum level = spdlog::level::info);

        static void setGlobalLogLevel(spdlog::level::level_enum level);
        static void setLoggerLevel(const std::string& loggerName, spdlog::level::level_enum level);

    private:
        static inline std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers;
        static inline std::shared_ptr<spdlog::logger> defaultLogger = nullptr;
        static inline std::mutex loggerMutex;

        // Static initialization function
        static inline const bool loggerInitialized = []() {
            Logger::initialize("application.log", spdlog::level::debug); // Initialize the logger
            return true; }();

    public:
        static inline auto core = Logger::createLogger("Core", spdlog::level::info);
    };
}

