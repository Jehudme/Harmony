#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <format>

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

	// ============================================================================
	// Task Handler Exceptions
	// ============================================================================

	/// Base exception for task handler-related errors
	struct TaskHandlerException : public HarmonyException {
		explicit TaskHandlerException(const std::string& message);
	};

	/// Exception thrown when task submission fails
	struct TaskSubmissionException : public TaskHandlerException {
		explicit TaskSubmissionException(const std::string& reason);
	};

	/// Exception thrown when a task is null or invalid
	struct InvalidTaskException : public TaskHandlerException {
		explicit InvalidTaskException(const std::string& reason);
	};

	/// Exception thrown when worker pool operations fail
	struct WorkerPoolException : public TaskHandlerException {
		explicit WorkerPoolException(const std::string& operation, const std::string& reason);
	};

	/// Exception thrown when task execution fails
	struct TaskExecutionException : public TaskHandlerException {
		explicit TaskExecutionException(const std::string& taskInfo, const std::string& reason);
	};

	// ============================================================================
	// Window Handler Exceptions
	// ============================================================================

	/// Base exception for window handler-related errors
	struct WindowHandlerException : public HarmonyException {
		explicit WindowHandlerException(const std::string& message);
	};

	/// Exception thrown when window initialization fails
	struct WindowInitializationException : public WindowHandlerException {
		explicit WindowInitializationException(const std::string& reason);
	};

	/// Exception thrown when window operations fail
	struct WindowOperationException : public WindowHandlerException {
		explicit WindowOperationException(const std::string& operation, const std::string& reason);
	};

	/// Exception thrown when window configuration is invalid
	struct InvalidWindowConfigurationException : public WindowHandlerException {
		explicit InvalidWindowConfigurationException(const std::string& parameter, const std::string& reason);
	};

	// ============================================================================
	// Engine Exceptions
	// ============================================================================

	/// Base exception for engine-related errors
	struct EngineException : public HarmonyException {
		explicit EngineException(const std::string& message);
	};

	/// Exception thrown when engine initialization fails
	struct EngineInitializationException : public EngineException {
		explicit EngineInitializationException(const std::string& reason);
	};

	/// Exception thrown when engine state is invalid for an operation
	struct InvalidEngineStateException : public EngineException {
		explicit InvalidEngineStateException(const std::string& operation, const std::string& currentState);
	};

	// ============================================================================
	// Task Exceptions
	// ============================================================================

	/// Base exception for task-related errors
	struct TaskException : public HarmonyLogicError {
		explicit TaskException(const std::string& message);
	};

	/// Exception thrown when task priority is invalid
	struct InvalidTaskPriorityException : public TaskException {
		explicit InvalidTaskPriorityException(uint16_t priority, const std::string& reason);
	};

	/// Exception thrown when task mode is invalid
	struct InvalidTaskModeException : public TaskException {
		explicit InvalidTaskModeException(const std::string& reason);
	};

	// ============================================================================
	// Configuration Exceptions
	// ============================================================================

	/// Base exception for configuration handler-related errors
	struct ConfigurationHandlerException : public HarmonyException {
		explicit ConfigurationHandlerException(const std::string& message);
	};

	/// Exception thrown when a configuration value is missing
	struct MissingConfigurationValueException : public ConfigurationHandlerException {
		explicit MissingConfigurationValueException(const std::string& key);
	};

	/// Exception thrown when configuration handler initialization fails
	struct ConfigurationHandlerInitializationException : public ConfigurationHandlerException {
		explicit ConfigurationHandlerInitializationException(const std::string& reason);
	};

	// ============================================================================
	// StateStack Exceptions
	// ============================================================================

	/// Base exception for state stack-related errors
	struct StateStackException : public HarmonyException {
		explicit StateStackException(const std::string& message);
	};

	/// Exception thrown when attempting to pop from empty state stack
	struct EmptyStateStackException : public StateStackException {
		explicit EmptyStateStackException();
	};

	/// Exception thrown when state stack operation fails
	struct StateStackOperationException : public StateStackException {
		explicit StateStackOperationException(const std::string& operation, const std::string& reason);
	};

	/// Exception thrown when a null state is provided
	struct NullStateException : public StateStackException {
		explicit NullStateException(const std::string& operation);
	};

	// ============================================================================
	// State Exceptions
	// ============================================================================

	/// Base exception for state-related errors
	struct StateException : public HarmonyException {
		explicit StateException(const std::string& message);
	};

	/// Exception thrown when state initialization fails
	struct StateInitializationException : public StateException {
		explicit StateInitializationException(const std::string& stateName, const std::string& reason);
	};

	/// Exception thrown when state operation fails
	struct StateOperationException : public StateException {
		explicit StateOperationException(const std::string& operation, const std::string& reason);
	};

	// ============================================================================
	// Scene Exceptions
	// ============================================================================

	/// Base exception for scene-related errors
	struct SceneException : public HarmonyException {
		explicit SceneException(const std::string& message);
	};

	/// Exception thrown when scene initialization fails
	struct SceneInitializationException : public SceneException {
		explicit SceneInitializationException(const std::string& sceneName, const std::string& reason);
	};

	/// Exception thrown when scene operation fails
	struct SceneOperationException : public SceneException {
		explicit SceneOperationException(const std::string& operation, const std::string& reason);
	};

	/// Exception thrown when entity is invalid or not found
	struct InvalidEntityException : public SceneException {
		explicit InvalidEntityException(uint32_t entityId, const std::string& reason);
	};

	// ============================================================================
	// Component Exceptions
	// ============================================================================

	/// Base exception for component-related errors
	struct ComponentException : public HarmonyException {
		explicit ComponentException(const std::string& message);
	};

	/// Exception thrown when component initialization fails
	struct ComponentInitializationException : public ComponentException {
		explicit ComponentInitializationException(const std::string& componentName, const std::string& reason);
	};

	/// Exception thrown when component creation fails
	struct ComponentCreationException : public ComponentException {
		explicit ComponentCreationException(const std::string& componentName, uint32_t entityId, const std::string& reason);
	};

	/// Exception thrown when component is not found
	struct ComponentNotFoundException : public ComponentException {
		explicit ComponentNotFoundException(const std::string& componentName, uint32_t entityId);
	};

	/// Exception thrown when component type is not registered
	struct ComponentNotRegisteredException : public ComponentException {
		explicit ComponentNotRegisteredException(const std::string& componentName);
	};

	/// Exception thrown when component operation fails
	struct ComponentOperationException : public ComponentException {
		explicit ComponentOperationException(const std::string& operation, const std::string& reason);
	};

	// ============================================================================
	// Resource Handler Exceptions
	// ============================================================================

	/// Base exception for resource handler-related errors
	struct ResourceException : public HarmonyException {
		explicit ResourceException(const std::string& message);
	};

	/// Exception thrown when resource loading fails
	struct ResourceLoadException : public ResourceException {
		explicit ResourceLoadException(const std::string& resourceType, const std::string& filePath, const std::string& reason);
	};

	/// Exception thrown when resource unloading fails
	struct ResourceUnloadException : public ResourceException {
		explicit ResourceUnloadException(const std::string& resourceType, uint64_t resourceId, const std::string& reason);
	};

	/// Exception thrown when resource is not found
	struct ResourceNotFoundException : public ResourceException {
		explicit ResourceNotFoundException(const std::string& resourceType, uint64_t resourceId);
	};

	/// Exception thrown when memory cap is exceeded
	struct ResourceMemoryCapExceededException : public ResourceException {
		explicit ResourceMemoryCapExceededException(std::size_t requestedSize, std::size_t currentSize, std::size_t maxSize);
	};

	/// Exception thrown when resource ID is invalid
	struct InvalidResourceIdException : public ResourceException {
		explicit InvalidResourceIdException(uint64_t resourceId, const std::string& reason);
	};

	/// Exception thrown when resource operation fails
	struct ResourceOperationException : public ResourceException {
		explicit ResourceOperationException(const std::string& operation, const std::string& reason);
	};

	// ============================================================================
	// Texture Resource Exceptions
	// ============================================================================

	/// Base exception for texture resource errors
	struct TextureResourceException : public ResourceException {
		explicit TextureResourceException(const std::string& message);
	};

	/// Exception thrown when texture loading fails
	struct TextureLoadException : public TextureResourceException {
		explicit TextureLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Image Resource Exceptions
	// ============================================================================

	/// Base exception for image resource errors
	struct ImageResourceException : public ResourceException {
		explicit ImageResourceException(const std::string& message);
	};

	/// Exception thrown when image loading fails
	struct ImageLoadException : public ImageResourceException {
		explicit ImageLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Font Resource Exceptions
	// ============================================================================

	/// Base exception for font resource errors
	struct FontResourceException : public ResourceException {
		explicit FontResourceException(const std::string& message);
	};

	/// Exception thrown when font loading fails
	struct FontLoadException : public FontResourceException {
		explicit FontLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Sound Resource Exceptions
	// ============================================================================

	/// Base exception for sound resource errors
	struct SoundResourceException : public ResourceException {
		explicit SoundResourceException(const std::string& message);
	};

	/// Exception thrown when sound loading fails
	struct SoundLoadException : public SoundResourceException {
		explicit SoundLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Music Resource Exceptions
	// ============================================================================

	/// Base exception for music resource errors
	struct MusicResourceException : public ResourceException {
		explicit MusicResourceException(const std::string& message);
	};

	/// Exception thrown when music loading fails
	struct MusicLoadException : public MusicResourceException {
		explicit MusicLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Model Resource Exceptions
	// ============================================================================

	/// Base exception for model resource errors
	struct ModelResourceException : public ResourceException {
		explicit ModelResourceException(const std::string& message);
	};

	/// Exception thrown when model loading fails
	struct ModelLoadException : public ModelResourceException {
		explicit ModelLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Material Resource Exceptions
	// ============================================================================

	/// Base exception for material resource errors
	struct MaterialResourceException : public ResourceException {
		explicit MaterialResourceException(const std::string& message);
	};

	/// Exception thrown when material loading fails
	struct MaterialLoadException : public MaterialResourceException {
		explicit MaterialLoadException(const std::string& reason);
	};

	// ============================================================================
	// Shader Resource Exceptions
	// ============================================================================

	/// Base exception for shader resource errors
	struct ShaderResourceException : public ResourceException {
		explicit ShaderResourceException(const std::string& message);
	};

	/// Exception thrown when shader loading fails
	struct ShaderLoadException : public ShaderResourceException {
		explicit ShaderLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Mesh Resource Exceptions
	// ============================================================================

	/// Base exception for mesh resource errors
	struct MeshResourceException : public ResourceException {
		explicit MeshResourceException(const std::string& message);
	};

	/// Exception thrown when mesh loading fails
	struct MeshLoadException : public MeshResourceException {
		explicit MeshLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Texture Cubemap Resource Exceptions
	// ============================================================================

	/// Base exception for texture cubemap resource errors
	struct TextureCubemapResourceException : public ResourceException {
		explicit TextureCubemapResourceException(const std::string& message);
	};

	/// Exception thrown when texture cubemap loading fails
	struct TextureCubemapLoadException : public TextureCubemapResourceException {
		explicit TextureCubemapLoadException(const std::string& reason);
	};

	// ============================================================================
	// Render Texture Resource Exceptions
	// ============================================================================

	/// Base exception for render texture resource errors
	struct RenderTextureResourceException : public ResourceException {
		explicit RenderTextureResourceException(const std::string& message);
	};

	/// Exception thrown when render texture loading fails
	struct RenderTextureLoadException : public RenderTextureResourceException {
		explicit RenderTextureLoadException(const std::string& reason);
	};

	// ============================================================================
	// Wave Resource Exceptions
	// ============================================================================

	/// Base exception for wave resource errors
	struct WaveResourceException : public ResourceException {
		explicit WaveResourceException(const std::string& message);
	};

	/// Exception thrown when wave loading fails
	struct WaveLoadException : public WaveResourceException {
		explicit WaveLoadException(const std::string& filePath, const std::string& reason);
	};

	// ============================================================================
	// Model Animation Resource Exceptions
	// ============================================================================

	/// Base exception for model animation resource errors
	struct ModelAnimationResourceException : public ResourceException {
		explicit ModelAnimationResourceException(const std::string& message);
	};

	/// Exception thrown when model animation loading fails
	struct ModelAnimationLoadException : public ModelAnimationResourceException {
		explicit ModelAnimationLoadException(const std::string& filePath, const std::string& reason);
	};
}

