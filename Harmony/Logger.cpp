#include "Logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace Harmony::Utilities 
{
	 // Internal state
	 std::once_flag initFlag;
	 std::shared_ptr<spdlog::logger> globalLogger;

	void Logger::initialize(const std::string& logFile,
		size_t maxFileSize,
		size_t maxFiles,
		size_t queueSize,
		size_t workerThreads)
	{
		std::call_once(initFlag, [&] {
			// Create async thread pool
			spdlog::init_thread_pool(queueSize, workerThreads);

			// Console sink
			auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console_sink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");

			// Rotating file sink
			auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
				std::string{ logFile }, maxFileSize, maxFiles);

			std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };

			// Create async logger
			globalLogger = std::make_shared<spdlog::async_logger>(
				"Harmony",
				sinks.begin(), sinks.end(),
				spdlog::thread_pool(),
				spdlog::async_overflow_policy::block);

			spdlog::register_logger(globalLogger);
			spdlog::set_default_logger(globalLogger);

			// Default levels
			spdlog::set_level(spdlog::level::trace);   // log everything
			spdlog::flush_on(spdlog::level::warn);     // flush on warnings or higher
			});
	}

	void Logger::trace(std::string_view message) {
		if (globalLogger) globalLogger->trace(message);
	}

	void Logger::debug(std::string_view message) {
		if (globalLogger) globalLogger->debug(message);
	}

	void Logger::info(std::string_view message) {
		if (globalLogger) globalLogger->info(message);
	}

	void Logger::warn(std::string_view message) {
		if (globalLogger) globalLogger->warn(message);
	}

	void Logger::error(std::string_view message) {
		if (globalLogger) globalLogger->error(message);
	}

	void Logger::critical(std::string_view message) {
		if (globalLogger) globalLogger->critical(message);
	}

	void Logger::shutdown() {
		spdlog::shutdown();
	}

}