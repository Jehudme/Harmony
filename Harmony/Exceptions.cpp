#include "pch.h"
#include "Exceptions.h"
#include "Logger.h"

#include <format>

namespace Harmony::Exceptions
{
	// Configuration Exceptions
	ConfigurationExceptions::ConfigurationExceptions(const std::string& message)
		: std::runtime_error(std::format("Configuration Exception: {}", message)) {
		HARMONY_ERROR(what());
	}

	OpenConfigurationFileException::OpenConfigurationFileException(const std::string& filepath)
		: ConfigurationExceptions(std::format("Failed to open configuration file: {}", filepath)) {
		HARMONY_ERROR(what());
	}

	ParseConfigurationFileException::ParseConfigurationFileException(const std::string& filepath, const std::string& details)
		: ConfigurationExceptions(std::format("Failed to parse configuration file: {}. Details: {}", filepath, details)) {
		HARMONY_ERROR(what(
	}

	// Component Exceptions
	ComponentNotRegistered::ComponentNotRegistered(const std::string& componentName)
		: std::runtime_error("Component not registered: " + componentName) {
		HARMONY_ERROR(what());
	}

	// Scene Exceptions
	SceneConfigurationNotFoundError::SceneConfigurationNotFoundError(const std::string& sceneKey)
		: std::runtime_error("Missing scene configuration: " + sceneKey) {
		HARMONY_ERROR(what());
	}

	ExpiredSceneError::ExpiredSceneError(const Utilities::UUID sceneId)
		: std::runtime_error("Scene expired: " + std::to_string(sceneId)) {
		HARMONY_ERROR(what());
	}

	ComponentNotFoundException::ComponentNotFoundException(const std::uint32_t entityId)
		: std::runtime_error("Entity " + std::to_string(entityId) + " missing requested component") {
		HARMONY_CRITICAL(what());
	}

	// State Exceptions
	StateStackPushFailed::StateStackPushFailed(const std::string& reason)
		: std::runtime_error("Failed to push state onto stack: " + reason) {
		HARMONY_ERROR(what());
	}

	StateStackEmptyError::StateStackEmptyError()
		: std::runtime_error("State stack is empty") {
		HARMONY_ERROR(what());
	}

	StartupStatesNotDefined::StartupStatesNotDefined()
		: std::runtime_error("Startup states not defined in configuration") {
		HARMONY_ERROR(what());
	}

	StateConfigurationNotFound::StateConfigurationNotFound(const std::string& stateKey)
		: std::runtime_error("Missing state configuration: " + stateKey) {
		HARMONY_ERROR(what());
	}

	// Task Exceptions
	NullTaskException::NullTaskException(const std::string& message)
		: std::runtime_error(message) {
		HARMONY_ERROR(what());
	}

	FailedTaskExecutionException::FailedTaskExecutionException(const std::string& message)
		: std::runtime_error(message) {
		HARMONY_ERROR(what());
	}

	TaskNotAssociatedWithEngineException::TaskNotAssociatedWithEngineException()
		: std::runtime_error("Task is not associated with an Engine.") {
		HARMONY_ERROR(what());
	}

	// Engine Exceptions
	EngineError::EngineError(const std::string& message)
		: std::runtime_error("Engine error: " + message) {
		HARMONY_ERROR(what());
	}

	ResourceNotFoundException::ResourceNotFoundException(const std::string& resourceType, const Utilities::UUID resourceId)
		: ResourceException("Resource not found: Type='" + resourceType + "', ID=" + std::to_string(resourceId)) {
		HARMONY_ERROR(what());
	}

	ResourceLoadException::ResourceLoadException(const std::string& resourceType, const Utilities::UUID resourceId, const std::string& details)
		: ResourceException("Failed to load resource: Type='" + resourceType + "', ID=" + std::to_string(resourceId) + ". Details: " + details) {
	}

	ResourceException::ResourceException(const std::string& message)
		: std::runtime_error("Resource Exception: " + message) {
		HARMONY_ERROR(what());
	}

	ResourceUnloadException::ResourceUnloadException(const Utilities::UUID resourceId)
		: ResourceException("Failed to unload resource ID: " + std::to_string(resourceId)) {
	}

	ResourceConversionException::ResourceConversionException(const std::string& target, const std::string& type)
		: ResourceException("Failed to convert resource of type '" + type + "' to target type '" + target + "'") {
	}
}
