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

	// ============================================================================
	// Task Handler Exceptions
	// ============================================================================

	TaskHandlerException::TaskHandlerException(const std::string& message)
		: HarmonyException(std::format("Task Handler: {}", message)) {
	}

	TaskSubmissionException::TaskSubmissionException(const std::string& reason)
		: TaskHandlerException(std::format("Failed to submit task: {}", reason)) {
		HARMONY_ERROR("Task submission failed: {}", reason);
	}

	InvalidTaskException::InvalidTaskException(const std::string& reason)
		: TaskHandlerException(std::format("Invalid task: {}", reason)) {
		HARMONY_ERROR("Invalid task encountered: {}", reason);
	}

	WorkerPoolException::WorkerPoolException(const std::string& operation, const std::string& reason)
		: TaskHandlerException(std::format("Worker pool operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("Worker pool operation '{}' failed: {}", operation, reason);
	}

	TaskExecutionException::TaskExecutionException(const std::string& taskInfo, const std::string& reason)
		: TaskHandlerException(std::format("Task execution failed [{}]: {}", taskInfo, reason)) {
		HARMONY_ERROR("Task execution failed [{}]: {}", taskInfo, reason);
	}

	// ============================================================================
	// Window Handler Exceptions
	// ============================================================================

	WindowHandlerException::WindowHandlerException(const std::string& message)
		: HarmonyException(std::format("Window Handler: {}", message)) {
	}

	WindowInitializationException::WindowInitializationException(const std::string& reason)
		: WindowHandlerException(std::format("Failed to initialize window: {}", reason)) {
		HARMONY_CRITICAL("Window initialization failed: {}", reason);
	}

	WindowOperationException::WindowOperationException(const std::string& operation, const std::string& reason)
		: WindowHandlerException(std::format("Window operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("Window operation '{}' failed: {}", operation, reason);
	}

	InvalidWindowConfigurationException::InvalidWindowConfigurationException(const std::string& parameter, const std::string& reason)
		: WindowHandlerException(std::format("Invalid window configuration parameter '{}': {}", parameter, reason)) {
		HARMONY_WARN("Invalid window configuration for '{}': {}", parameter, reason);
	}

	// ============================================================================
	// Engine Exceptions
	// ============================================================================

	EngineException::EngineException(const std::string& message)
		: HarmonyException(std::format("Engine: {}", message)) {
	}

	EngineInitializationException::EngineInitializationException(const std::string& reason)
		: EngineException(std::format("Failed to initialize engine: {}", reason)) {
		HARMONY_CRITICAL("Engine initialization failed: {}", reason);
	}

	InvalidEngineStateException::InvalidEngineStateException(const std::string& operation, const std::string& currentState)
		: EngineException(std::format("Invalid engine state for operation '{}': currently in state '{}'", operation, currentState)) {
		HARMONY_ERROR("Invalid engine state for '{}': state is '{}'", operation, currentState);
	}

	// ============================================================================
	// Task Exceptions
	// ============================================================================

	TaskException::TaskException(const std::string& message)
		: HarmonyLogicError(std::format("Task: {}", message)) {
	}

	InvalidTaskPriorityException::InvalidTaskPriorityException(uint16_t priority, const std::string& reason)
		: TaskException(std::format("Invalid task priority {}: {}", priority, reason)) {
		HARMONY_ERROR("Invalid task priority {}: {}", priority, reason);
	}

	InvalidTaskModeException::InvalidTaskModeException(const std::string& reason)
		: TaskException(std::format("Invalid task mode: {}", reason)) {
		HARMONY_ERROR("Invalid task mode: {}", reason);
	}

	// ============================================================================
	// ConfigurationHandler Exceptions
	// ============================================================================

	ConfigurationHandlerException::ConfigurationHandlerException(const std::string& message)
		: HarmonyException(std::format("Configuration Handler: {}", message)) {
	}

	MissingConfigurationValueException::MissingConfigurationValueException(const std::string& key)
		: ConfigurationHandlerException(std::format("Missing required configuration value: '{}'", key)) {
		HARMONY_ERROR("Missing required configuration value: '{}'", key);
	}

	ConfigurationHandlerInitializationException::ConfigurationHandlerInitializationException(const std::string& reason)
		: ConfigurationHandlerException(std::format("Failed to initialize configuration handler: {}", reason)) {
		HARMONY_CRITICAL("ConfigurationHandler initialization failed: {}", reason);
	}

	// ============================================================================
	// StateStack Exceptions
	// ============================================================================

	StateStackException::StateStackException(const std::string& message)
		: HarmonyException(std::format("State Stack: {}", message)) {
	}

	EmptyStateStackException::EmptyStateStackException()
		: StateStackException("Attempted to access or pop from an empty state stack") {
		HARMONY_WARN("Attempted operation on empty state stack");
	}

	StateStackOperationException::StateStackOperationException(const std::string& operation, const std::string& reason)
		: StateStackException(std::format("State stack operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("State stack operation '{}' failed: {}", operation, reason);
	}

	NullStateException::NullStateException(const std::string& operation)
		: StateStackException(std::format("Null state provided to operation '{}'", operation)) {
		HARMONY_ERROR("Null state provided to operation: {}", operation);
	}

	// ============================================================================
	// State Exceptions
	// ============================================================================

	StateException::StateException(const std::string& message)
		: HarmonyException(std::format("State: {}", message)) {
	}

	StateInitializationException::StateInitializationException(const std::string& stateName, const std::string& reason)
		: StateException(std::format("Failed to initialize state '{}': {}", stateName, reason)) {
		HARMONY_ERROR("State '{}' initialization failed: {}", stateName, reason);
	}

	StateOperationException::StateOperationException(const std::string& operation, const std::string& reason)
		: StateException(std::format("State operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("State operation '{}' failed: {}", operation, reason);
	}

	// ============================================================================
	// Scene Exceptions
	// ============================================================================

	SceneException::SceneException(const std::string& message)
		: HarmonyException(std::format("Scene: {}", message)) {
	}

	SceneInitializationException::SceneInitializationException(const std::string& sceneName, const std::string& reason)
		: SceneException(std::format("Failed to initialize scene '{}': {}", sceneName, reason)) {
		HARMONY_ERROR("Scene '{}' initialization failed: {}", sceneName, reason);
	}

	SceneOperationException::SceneOperationException(const std::string& operation, const std::string& reason)
		: SceneException(std::format("Scene operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("Scene operation '{}' failed: {}", operation, reason);
	}

} // namespace Harmony::Exceptions
