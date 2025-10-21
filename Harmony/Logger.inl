namespace Harmony::Utilities
{
    template<typename... Args>
    static void Logger::trace(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        trace(s);
    }

    template<typename... Args>
    static void Logger::debug(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        debug(s);
    }

    template<typename... Args>
    static void Logger::info(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        info(s);
    }

    template<typename... Args>
    static void Logger::warn(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        warn(s);
    }

    template<typename... Args>
    static void Logger::error(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        error(s);
    }

    template<typename... Args>
    static void Logger::critical(std::string_view fmt, Args&&... args) {
        auto s = std::vformat(fmt, std::make_format_args(args...));
        critical(s);
    }
}



