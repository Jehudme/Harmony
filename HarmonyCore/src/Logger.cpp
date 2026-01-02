#include <cassert>

#include "Harmony/Assert.h"
#include "Harmony/Logger.h"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Harmony
{
// ========================================================
// Logger Internal State
// ========================================================

std::once_flag initializationFlag;
std::shared_ptr<spdlog::logger> globalLogger;

// ========================================================
// Logger Initialization and Shutdown
// ========================================================

void Logger::Initialize(const std::string& logFile, std::size_t maximumFileSize,
                        std::size_t maximumFiles, std::size_t queueSize, std::size_t workerThreads)
{
    HARMONY_ASSERT(!logFile.empty(), "Log file path cannot be empty");
    HARMONY_ASSERT(maximumFileSize > 0, "Maximum file size must be greater than 0");
    HARMONY_ASSERT(maximumFiles > 0, "Maximum number of files must be greater than 0");
    HARMONY_ASSERT(queueSize > 0, "Queue size must be greater than 0");
    HARMONY_ASSERT(workerThreads > 0, "Number of worker threads must be greater than 0");

    std::call_once(initializationFlag,
                   [&]
                   {
                       auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                       consoleSink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");

                       auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                           std::string{logFile}, maximumFileSize, maximumFiles);

                       std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};

#ifdef _DEBUG
                       globalLogger =
                           std::make_shared<spdlog::logger>("Harmony", sinks.begin(), sinks.end());

                       globalLogger->flush_on(spdlog::level::trace);
#else
            spdlog::init_thread_pool(queueSize, workerThreads);

            globalLogger = std::make_shared<spdlog::async_logger>(
                "Harmony",
                sinks.begin(), 
                sinks.end(),
                spdlog::thread_pool(),
                spdlog::async_overflow_policy::block);

            globalLogger->flush_on(spdlog::level::warn);
#endif

                       if (globalLogger == nullptr)
                       {
                           return;
                       }

                       spdlog::register_logger(globalLogger);
                       spdlog::set_default_logger(globalLogger);
                       spdlog::set_level(spdlog::level::trace);

                       HARMONY_INFO("Logger initialized successfully with log file: {}", logFile);
                   });
}

void Logger::Shutdown()
{
    if (globalLogger)
    {
        HARMONY_INFO("Logger shutting down");
        spdlog::shutdown();
    }
}

// ========================================================
// Logger Output Functions
// ========================================================

void Logger::Trace(std::string_view message)
{
    if (globalLogger)
    {
        globalLogger->trace(message);
    }
}

void Logger::Debug(std::string_view message)
{
    if (globalLogger)
    {
        globalLogger->debug(message);
    }
}

void Logger::Info(std::string_view message)
{
    if (globalLogger)
    {
        globalLogger->info(message);
    }
}

void Logger::Warn(std::string_view message)
{
    if (globalLogger)
    {
        globalLogger->warn(message);
    }
}

void Logger::Error(std::string_view message)
{
    if (globalLogger)
    {
        globalLogger->error(message);
    }
}

void Logger::Critical(std::string_view message)
{
    if (globalLogger)
    {
        globalLogger->critical(message);
    }
}

} // namespace Harmony