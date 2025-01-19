#include "pch.h"
#include "Logger.h"

namespace harmony
{
    void Logger::create(const std::string& logFilePath, spdlog::level::level_enum globalLevel) {
        // Create sinks for console and file
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

        // Set global logging level for sinks
        consoleSink->set_level(globalLevel);
        fileSink->set_level(globalLevel);

        // Create a default logger with these sinks
        defaultLogger = std::make_shared<spdlog::logger>("Default", spdlog::sinks_init_list{ consoleSink, fileSink });
        defaultLogger->set_level(globalLevel);
        defaultLogger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v");

        // Register the default logger
        spdlog::register_logger(defaultLogger);
        spdlog::set_default_logger(defaultLogger);
        spdlog::set_level(globalLevel); // Set the global logging level
        spdlog::flush_on(globalLevel);
    }

    std::shared_ptr<spdlog::logger> Logger::createLogger(const std::string& loggerName, spdlog::level::level_enum level) {
        std::lock_guard<std::mutex> lock(loggerMutex);

        if (loggers.find(loggerName) != loggers.end()) {
            return loggers[loggerName];
        }

        // Create a new logger with console and file sinks
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(loggerName + ".log", true);

        consoleSink->set_level(level);
        fileSink->set_level(level);

        auto logger = std::make_shared<spdlog::logger>(loggerName, spdlog::sinks_init_list{ consoleSink, fileSink });
        logger->set_level(level);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v");

        spdlog::register_logger(logger);
        loggers[loggerName] = logger;

        return logger;
    }

    void Logger::setGlobalLogLevel(spdlog::level::level_enum level) {
        spdlog::set_level(level);
        for (auto& pair : loggers) {
            pair.second->set_level(level);
        }
    }

    void Logger::setLoggerLevel(const std::string& loggerName, spdlog::level::level_enum level) {
        std::lock_guard<std::mutex> lock(loggerMutex);

        if (loggers.find(loggerName) != loggers.end()) {
            loggers[loggerName]->set_level(level);
        }
    }
}
