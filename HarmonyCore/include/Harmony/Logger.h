#pragma once

#include <string>
#include <string_view>
#include <cstddef>
#include <format>

namespace Harmony 
{
    // ========================================================
    // Logger Class for Application Logging
    // ========================================================

    class Logger 
    {
    public:
        static void Initialize(
            const std::string& logFile = "Engine.log",
            std::size_t maximumFileSize = 10 * 1024 * 1024,
            std::size_t maximumFiles = 3,
            std::size_t queueSize = 1 << 15,
            std::size_t workerThreads = 1
        );

        static void Shutdown();

        static void Trace(std::string_view message);
        static void Debug(std::string_view message);
        static void Info(std::string_view message);
        static void Warn(std::string_view message);
        static void Error(std::string_view message);
        static void Critical(std::string_view message);

        template<typename... Args>
        static void Trace(std::string_view fmt, Args&&... arguments);
        template<typename... Args>
        static void Debug(std::string_view fmt, Args&&... arguments);
        template<typename... Args>
        static void Info(std::string_view fmt, Args&&... arguments);
        template<typename... Args>
        static void Warn(std::string_view fmt, Args&&... arguments);
        template<typename... Args>
        static void Error(std::string_view fmt, Args&&... arguments);
        template<typename... Args>
        static void Critical(std::string_view fmt, Args&&... arguments);
    };

} // namespace Harmony

#include "Logger.inl"