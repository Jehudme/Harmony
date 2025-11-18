#include "pch.h"
#include "Exceptions.h"
#include "Logger.h"

#include <format>

namespace Harmony::Exceptions
{
	// ============================================================================
	// Base Exception Classes
	// ============================================================================

	HarmonyException::HarmonyException(const std::string& message)
		: std::runtime_error(std::format("[Harmony Exception] {}", message)) {
		HARMONY_ERROR(what());
	}

	HarmonyLogicError::HarmonyLogicError(const std::string& message)
		: std::logic_error(std::format("[Harmony Logic Error] {}", message)) {
		HARMONY_ERROR(what());
	}

	// ============================================================================
	// Configuration Exceptions
	// ============================================================================

	ConfigurationException::ConfigurationException(const std::string& message)
		: HarmonyException(std::format("Configuration: {}", message)) {
	}

	OpenConfigurationFileException::OpenConfigurationFileException(const std::string& filepath)
		: ConfigurationException(std::format("Failed to open configuration file: '{}'", filepath)) {
		HARMONY_ERROR("Cannot open configuration file: {}", filepath);
	}

	ParseConfigurationFileException::ParseConfigurationFileException(const std::string& filepath, const std::string& details)
		: ConfigurationException(std::format("Failed to parse configuration file: '{}'. Details: {}", filepath, details)) {
		HARMONY_ERROR("Parse error in configuration file '{}': {}", filepath, details);
	}

	ConfigurationKeyNotFoundException::ConfigurationKeyNotFoundException(const std::string& key)
		: ConfigurationException(std::format("Configuration key not found: '{}'", key)) {
		HARMONY_WARN("Configuration key '{}' not found", key);
	}

	ConfigurationTypeException::ConfigurationTypeException(const std::string& key, const std::string& expectedType, const std::string& actualType)
		: ConfigurationException(std::format("Type mismatch for key '{}': expected '{}', got '{}'", key, expectedType, actualType)) {
		HARMONY_ERROR("Type mismatch for configuration key '{}': expected {}, got {}", key, expectedType, actualType);
	}

	// ============================================================================
	// Logger Exceptions
	// ============================================================================

	LoggerException::LoggerException(const std::string& message)
		: HarmonyException(std::format("Logger: {}", message)) {
	}

	LoggerInitializationException::LoggerInitializationException(const std::string& reason)
		: LoggerException(std::format("Failed to initialize logger: {}", reason)) {
		// Note: We can't use HARMONY_ERROR here as the logger might not be initialized
	}

	LoggerFileException::LoggerFileException(const std::string& filepath, const std::string& operation)
		: LoggerException(std::format("File operation '{}' failed on log file: '{}'", operation, filepath)) {
		HARMONY_CRITICAL("Logger file operation '{}' failed: {}", operation, filepath);
	}

	// ============================================================================
	// File System Exceptions
	// ============================================================================

	FileSystemException::FileSystemException(const std::string& message)
		: HarmonyException(std::format("File System: {}", message)) {
	}

	FileOpenException::FileOpenException(const std::string& filepath, const std::string& mode)
		: FileSystemException(std::format("Failed to open file '{}' in mode '{}'", filepath, mode)) {
		HARMONY_ERROR("Cannot open file '{}' in mode '{}'", filepath, mode);
	}

	FileReadException::FileReadException(const std::string& filepath, const std::string& reason)
		: FileSystemException(std::format("Failed to read from file '{}': {}", filepath, reason)) {
		HARMONY_ERROR("Read error from file '{}': {}", filepath, reason);
	}

	FileWriteException::FileWriteException(const std::string& filepath, const std::string& reason)
		: FileSystemException(std::format("Failed to write to file '{}': {}", filepath, reason)) {
		HARMONY_ERROR("Write error to file '{}': {}", filepath, reason);
	}

	InvalidFilePathException::InvalidFilePathException(const std::string& filepath, const std::string& reason)
		: FileSystemException(std::format("Invalid file path '{}': {}", filepath, reason)) {
		HARMONY_WARN("Invalid file path '{}': {}", filepath, reason);
	}

	// ============================================================================
	// Validation Exceptions
	// ============================================================================

	ValidationException::ValidationException(const std::string& message)
		: HarmonyLogicError(std::format("Validation: {}", message)) {
	}

	OutOfRangeException::OutOfRangeException(const std::string& message)
		: ValidationException(message) {
	}

	NullPointerException::NullPointerException(const std::string& pointerName)
		: ValidationException(std::format("Null pointer encountered: '{}'", pointerName)) {
		HARMONY_CRITICAL("Null pointer exception: {}", pointerName);
	}

	InvalidArgumentException::InvalidArgumentException(const std::string& argumentName, const std::string& reason)
		: ValidationException(std::format("Invalid argument '{}': {}", argumentName, reason)) {
		HARMONY_ERROR("Invalid argument '{}': {}", argumentName, reason);
	}

	// ============================================================================
	// Time Exceptions
	// ============================================================================

	TimeException::TimeException(const std::string& message)
		: HarmonyLogicError(std::format("Time: {}", message)) {
	}

	InvalidTimeValueException::InvalidTimeValueException(const std::string& reason)
		: TimeException(std::format("Invalid time value: {}", reason)) {
		HARMONY_ERROR("Invalid time value: {}", reason);
	}

	InvalidTimerOperationException::InvalidTimerOperationException(const std::string& operation, const std::string& reason)
		: TimeException(std::format("Invalid timer operation '{}': {}", operation, reason)) {
		HARMONY_WARN("Invalid timer operation '{}': {}", operation, reason);
	}

	// ============================================================================
	// Color Exceptions
	// ============================================================================

	ColorException::ColorException(const std::string& message)
		: HarmonyLogicError(std::format("Color: {}", message)) {
	}

	InvalidColorValueException::InvalidColorValueException(const std::string& component, int value)
		: ColorException(std::format("Invalid {} component value: {} (expected 0-255)", component, value)) {
		HARMONY_ERROR("Invalid color {} component: {} (valid range: 0-255)", component, value);
	}

} // namespace Harmony::Exceptions
