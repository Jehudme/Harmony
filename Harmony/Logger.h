#pragma once

#include "include.h"

#ifdef _DEBUG
    #define HM_LOGGER_TRACE(MESSAGE, ...)       Harmony::Core::Logger::instance->trace(MESSAGE, __VA_ARGS__)
    #define HM_LOGGER_INFO(MESSAGE, ...)        Harmony::Core::Logger::instance->info(MESSAGE, __VA_ARGS__)
    #define HM_LOGGER_WARN(MESSAGE, ...)        Harmony::Core::Logger::instance->warn(MESSAGE, __VA_ARGS__)
    #define HM_LOGGER_ERROR(MESSAGE, ...)       Harmony::Core::Logger::instance->error(MESSAGE, __VA_ARGS__)
    #define HM_LOGGER_CRITICAL(MESSAGE, ...)    Harmony::Core::Logger::instance->critical(MESSAGE, __VA_ARGS__)
    #define HM_LOGGER_DEBUG(MESSAGE, ...)       Harmony::Core::Logger::instance->debug(MESSAGE, __VA_ARGS__)
#else
    #define HM_LOGGER_INITIALIZE(SYSTEM_NAME, ...)
    #define HM_LOGGER_TRACE(MESSAGE, ...)
    #define HM_LOGGER_INFO(MESSAGE, ...)
    #define HM_LOGGER_WARN(MESSAGE, ...)
    #define HM_LOGGER_ERROR(MESSAGE, ...)
    #define HM_LOGGER_CRITICAL(MESSAGE, ...)
    #define HM_LOGGER_DEBUG(MESSAGE, ...)
#endif

namespace Harmony::Core
{
    class Logger {
    public:
        static std::shared_ptr<Logger> create(const std::string& systemName, const spdlog::level::level_enum level = spdlog::level::trace) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_instances.contains(systemName)) {
                m_instances[systemName] = std::shared_ptr<Logger>(new Logger(systemName, level));
            }
            return m_instances[systemName];
        }

        template <typename... Args>
        void trace(const spdlog::format_string_t<Args...>& message, Args&&... args) {
            m_logger->trace(message, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void info(const spdlog::format_string_t<Args...>& message, Args&&... args) {
            m_logger->info(message, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void warn(const spdlog::format_string_t<Args...>& message, Args&&... args) {
            m_logger->warn(message, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(const spdlog::format_string_t<Args...>& message, Args&&... args) {
            m_logger->error(message, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void critical(const spdlog::format_string_t<Args...>& message, Args&&... args) {
            m_logger->critical(message, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void debug(const spdlog::format_string_t<Args...>& message, Args&&... args) {
            m_logger->debug(message, std::forward<Args>(args)...);
        }

    private:
          Logger(const std::string& systemName, const spdlog::level::level_enum level) {
            std::vector<spdlog::sink_ptr> sinks;
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(systemName + ".log", true));

            m_logger = std::make_shared<spdlog::logger>(systemName, begin(sinks), end(sinks));
            spdlog::register_logger(m_logger);
            m_logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] %v");
            m_logger->set_level(level);
        }

        static inline std::unordered_map<std::string, std::shared_ptr<Logger>> m_instances;
        static inline std::mutex m_mutex;

        std::shared_ptr<spdlog::logger> m_logger;

    public:
        static inline auto instance = Harmony::Core::Logger::create("Core", spdlog::level::info);
    };
}
