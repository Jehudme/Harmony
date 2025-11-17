#pragma once
namespace Harmony::Exceptions
{
	// Configuration Exceptions
	struct ConfigurationExceptions : public std::runtime_error {
		explicit ConfigurationExceptions(const std::string& message);
	};

	struct OpenConfigurationFileException : public ConfigurationExceptions {
		explicit OpenConfigurationFileException(const std::string& filepath);
	};

	struct ParseConfigurationFileException : public ConfigurationExceptions {
		explicit ParseConfigurationFileException(const std::string& filepath, const std::string& details);
	};
}

