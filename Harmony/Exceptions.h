#pragma once

#include <stdexcept>
#include <string>

namespace Harmony::Exceptions
{
	// ============================================================================
	// Base Exception Classes
	// ============================================================================

	/// Base class for all Harmony runtime errors
	struct HarmonyException : public std::runtime_error {
		explicit HarmonyException(const std::string& message);
	};

	/// Base class for all Harmony logic errors (programming mistakes)
	struct HarmonyLogicError : public std::logic_error {
		explicit HarmonyLogicError(const std::string& message);
	};

	// ============================================================================
	// Configuration Exceptions
	// ============================================================================

	/// Base exception for configuration-related errors
	struct ConfigurationException : public HarmonyException {
		explicit ConfigurationException(const std::string& message);
	};

	/// Exception thrown when a configuration file cannot be opened
	struct OpenConfigurationFileException : public ConfigurationException {
		explicit OpenConfigurationFileException(const std::string& filepath);
	};

	/// Exception thrown when a configuration file cannot be parsed
	struct ParseConfigurationFileException : public ConfigurationException {
		explicit ParseConfigurationFileException(const std::string& filepath, const std::string& details);
	};

	/// Exception thrown when a configuration key is not found
	struct ConfigurationKeyNotFoundException : public ConfigurationException {
		explicit ConfigurationKeyNotFoundException(const std::string& key);
	};

	/// Exception thrown when a configuration value has an invalid type
	struct ConfigurationTypeException : public ConfigurationException {
		explicit ConfigurationTypeException(const std::string& key, const std::string& expectedType, const std::string& actualType);
	};

	// ============================================================================
	// Logger Exceptions
	// ============================================================================

	/// Base exception for logger-related errors
	struct LoggerException : public HarmonyException {
		explicit LoggerException(const std::string& message);
	};

	/// Exception thrown when logger initialization fails
	struct LoggerInitializationException : public LoggerException {
		explicit LoggerInitializationException(const std::string& reason);
	};

	/// Exception thrown when logger file operations fail
	struct LoggerFileException : public LoggerException {
		explicit LoggerFileException(const std::string& filepath, const std::string& operation);
	};

	// ============================================================================
	// File System Exceptions
	// ============================================================================

	/// Base exception for file system operations
	struct FileSystemException : public HarmonyException {
		explicit FileSystemException(const std::string& message);
	};

	/// Exception thrown when a file cannot be opened
	struct FileOpenException : public FileSystemException {
		explicit FileOpenException(const std::string& filepath, const std::string& mode);
	};

	/// Exception thrown when a file read operation fails
	struct FileReadException : public FileSystemException {
		explicit FileReadException(const std::string& filepath, const std::string& reason);
	};

	/// Exception thrown when a file write operation fails
	struct FileWriteException : public FileSystemException {
		explicit FileWriteException(const std::string& filepath, const std::string& reason);
	};

	/// Exception thrown when a file path is invalid
	struct InvalidFilePathException : public FileSystemException {
		explicit InvalidFilePathException(const std::string& filepath, const std::string& reason);
	};

	// ============================================================================
	// Validation Exceptions
	// ============================================================================

	/// Base exception for validation errors
	struct ValidationException : public HarmonyLogicError {
		explicit ValidationException(const std::string& message);
	};

	/// Exception thrown when a value is out of valid range
	struct OutOfRangeException : public ValidationException {
		template<typename T>
		OutOfRangeException(const std::string& valueName, T value, T min, T max)
			: ValidationException(std::format("Value '{}' ({}) is out of range [{}, {}]", valueName, value, min, max)) {
			HARMONY_ERROR("Out of range: '{}' = {} (valid range: [{}, {}])", valueName, value, min, max);
		}
		explicit OutOfRangeException(const std::string& message);
	};

	/// Exception thrown when a null pointer is encountered where it shouldn't be
	struct NullPointerException : public ValidationException {
		explicit NullPointerException(const std::string& pointerName);
	};

	/// Exception thrown when an invalid argument is provided
	struct InvalidArgumentException : public ValidationException {
		explicit InvalidArgumentException(const std::string& argumentName, const std::string& reason);
	};

	// ============================================================================
	// Time Exceptions
	// ============================================================================

	/// Base exception for time-related errors
	struct TimeException : public HarmonyLogicError {
		explicit TimeException(const std::string& message);
	};

	/// Exception thrown when a time value is invalid
	struct InvalidTimeValueException : public TimeException {
		explicit InvalidTimeValueException(const std::string& reason);
	};

	/// Exception thrown when a timer operation is invalid
	struct InvalidTimerOperationException : public TimeException {
		explicit InvalidTimerOperationException(const std::string& operation, const std::string& reason);
	};

	// ============================================================================
	// Color Exceptions
	// ============================================================================

	/// Base exception for color-related errors
	struct ColorException : public HarmonyLogicError {
		explicit ColorException(const std::string& message);
	};

	/// Exception thrown when a color value is invalid
	struct InvalidColorValueException : public ColorException {
		explicit InvalidColorValueException(const std::string& component, int value);
	};
}

