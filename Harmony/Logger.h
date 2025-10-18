#pragma once

#include <spdlog/logger.h>
#include <memory>
#include <string_view>

namespace Harmony::Utilities {

    /// Central async logger for the engine.
    /// Wraps spdlog and provides a single global logging facility.
    class Logger final {
    public:
        /// Initialize the logger once at engine startup.
        static void initialize(std::string_view logFile = "Harmony.log",
            size_t maxFileSize = 10 * 1024 * 1024,
            size_t maxFiles = 3,
            size_t queueSize = 1 << 15,
            size_t workerThreads = 1);

        /// Retrieve the global logger instance.
        [[nodiscard]] static spdlog::logger& get() noexcept;

        /// Shutdown and flush all loggers.
        static void shutdown();

    private:
        Logger() = default;
    };

} // namespace Harmony::Log

#ifdef _DEBUG

#define HARMONY_TRACE(...)    SPDLOG_TRACE(__VA_ARGS__)
#define HARMONY_DEBUG(...)    SPDLOG_DEBUG(__VA_ARGS__)
#define HARMONY_INFO(...)     SPDLOG_INFO(__VA_ARGS__)
#define HARMONY_WARN(...)     SPDLOG_WARN(__VA_ARGS__)
#define HARMONY_ERROR(...)    SPDLOG_ERROR(__VA_ARGS__)
#define HARMONY_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

#endif
