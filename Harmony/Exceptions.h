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

	// Component Exceptions
	struct ComponentNotRegistered : std::runtime_error {
		explicit ComponentNotRegistered(const std::string& componentName);
	};

	// Scene Exceptions
	struct SceneConfigurationNotFoundError : std::runtime_error {
		explicit SceneConfigurationNotFoundError(const std::string& sceneKey);
	};

	struct ExpiredSceneError : std::runtime_error {
		explicit ExpiredSceneError(const Utilities::UUID sceneId);
	};

	struct ComponentNotFoundException : std::runtime_error {
		explicit ComponentNotFoundException(const EntityID entityId);
	};

	// State Exceptions
	struct StateStackPushFailed : std::runtime_error {
		explicit StateStackPushFailed(const std::string& reason);
	};

	struct StateStackEmptyError : std::runtime_error {
		explicit StateStackEmptyError();
	};

	struct StartupStatesNotDefined : std::runtime_error {
		explicit StartupStatesNotDefined();
	};

	struct StateConfigurationNotFound : std::runtime_error {
		explicit StateConfigurationNotFound(const std::string& stateKey);
	};

	// Task Exceptions
	struct NullTaskException : std::runtime_error {
		explicit NullTaskException(const std::string& message);
	};

	struct FailedTaskExecutionException : std::runtime_error {
		explicit FailedTaskExecutionException(const std::string& message);
	};

	struct TaskNotAssociatedWithEngineException : std::runtime_error {
		explicit TaskNotAssociatedWithEngineException();
	};

	// Engine Exceptions
	struct EngineError : public std::runtime_error {
		explicit EngineError(const std::string& message);
	};

	// Resource Exceptions
	struct ResourceException : public std::runtime_error {
		explicit ResourceException(const std::string& message);
	};

	struct ResourceNotFoundException : public ResourceException {
		explicit ResourceNotFoundException(const std::string& resourceType, const Utilities::UUID resourceId);
	};

	struct ResourceLoadException : public ResourceException {
		explicit ResourceLoadException(const std::string& resourceType, const Utilities::UUID resourceId, const std::string& details);
	};

	struct ResourceUnloadException : public ResourceException {
		explicit ResourceUnloadException(const Utilities::UUID resourceId);
	};

	struct ResourceConversionException : public ResourceException {
		explicit ResourceConversionException(const std::string& target, const std::string& type);
	};

	struct RenderManagementException : public std::runtime_error {
		explicit RenderManagementException(const std::string& message);
	};

	struct InvalidRenderTargetException : public RenderManagementException {
		explicit InvalidRenderTargetException(const std::string& details);
	};
}

