#include <cassert> // must be first to avoid errors on some platforms

#include "Harmony/Assert.h"
#include "Harmony/Exceptions.h"
#include "Harmony/Logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace Harmony
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
		// Validate parameters
		HARMONY_ASSERT(!logFile.empty(), "Log file path cannot be empty");
		HARMONY_ASSERT(maxFileSize > 0, "Maximum file size must be greater than 0");
		HARMONY_ASSERT(maxFiles > 0, "Maximum number of files must be greater than 0");
		HARMONY_ASSERT(queueSize > 0, "Queue size must be greater than 0");
		HARMONY_ASSERT(workerThreads > 0, "Number of worker threads must be greater than 0");

		std::call_once(initFlag, [&] {
			try {
				auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
				console_sink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");

				auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
					std::string{ logFile }, maxFileSize, maxFiles);

				std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
#ifdef _DEBUG
				globalLogger = std::make_shared<spdlog::logger>(
					"Harmony",
					sinks.begin(),
					sinks.end()
				);

				globalLogger->flush_on(spdlog::level::trace);

#else
				spdlog::init_thread_pool(queueSize, workerThreads);

				globalLogger = std::make_shared<spdlog::async_logger>(
					"Harmony",
					sinks.begin(), sinks.end(),
					spdlog::thread_pool(),
					spdlog::async_overflow_policy::block);

				globalLogger->flush_on(spdlog::level::warn);
#endif

				spdlog::register_logger(globalLogger);
				spdlog::set_default_logger(globalLogger);

				spdlog::set_level(spdlog::level::trace);
			}

			catch (const std::exception& ex) {
				HARMONY_THROW("Logger initialization failed: {}", ex.what());
			}
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