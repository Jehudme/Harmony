#pragma once

#include "include.h"

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
        explicit Logger(const std::string& systemName, const spdlog::level::level_enum level) {
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
    };
}
