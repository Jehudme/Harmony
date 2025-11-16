#pragma once

#include <string>
#include <string_view>
#include <cstddef>
#include <format>

namespace Harmony::Utilities {

    class Logger final {
    public:
        static void initialize(
            const std::string& logFile = "Engine.log",
            std::size_t maxFileSize = 10 * 1024 * 1024,
            std::size_t maxFiles = 3,
            std::size_t queueSize = 1 << 15,
            std::size_t workerThreads = 1
        );

        static void shutdown();

        static void trace(std::string_view message);
        static void debug(std::string_view message);
        static void info(std::string_view message);
        static void warn(std::string_view message);
        static void error(std::string_view message);
        static void critical(std::string_view message);

        template<typename... Args>
        static void trace(std::string_view fmt, Args&&... args);
        template<typename... Args>
        static void debug(std::string_view fmt, Args&&... args);
        template<typename... Args>
        static void info(std::string_view fmt, Args&&... args);
        template<typename... Args>
        static void warn(std::string_view fmt, Args&&... args);
        template<typename... Args>
        static void error(std::string_view fmt, Args&&... args);
        template<typename... Args>
        static void critical(std::string_view fmt, Args&&... args);
    };

}

#include "Logger.inl"