#include "pch.h"
#include "Exceptions.h"
#include "Logger.h"

#include <format>

namespace Harmony::Exceptions
{
	ConfigurationExceptions::ConfigurationExceptions(const std::string& message)
		: std::runtime_error(std::format("Configuration Exception: {}", message)) {
		Harmony::Utilities::Logger::error(what());
	}

	OpenConfigurationFileException::OpenConfigurationFileException(const std::string& filepath)
		: ConfigurationExceptions(std::format("Failed to open configuration file: {}", filepath)) {
		HARMONY_ERROR(what());
	}

	ParseConfigurationFileException::ParseConfigurationFileException(const std::string& filepath, const std::string& details)
		: ConfigurationExceptions(std::format("Failed to parse configuration file: {}. Details: {}", filepath, details)) {
		HARMONY_ERROR(what());
	}
}
