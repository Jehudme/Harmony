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
	// Configuration Exceptions
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
		HARMONY_CRITICAL("Configuration initialization failed: {}", reason);
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

	InvalidEntityException::InvalidEntityException(uint32_t entityId, const std::string& reason)
		: SceneException(std::format("Invalid entity {}: {}", entityId, reason)) {
		HARMONY_ERROR("Invalid entity {}: {}", entityId, reason);
	}

	// ============================================================================
	// Component Exceptions
	// ============================================================================

	ComponentException::ComponentException(const std::string& message)
		: HarmonyException(std::format("Component: {}", message)) {
	}

	ComponentInitializationException::ComponentInitializationException(const std::string& componentName, const std::string& reason)
		: ComponentException(std::format("Failed to initialize component '{}': {}", componentName, reason)) {
		HARMONY_ERROR("Component '{}' initialization failed: {}", componentName, reason);
	}

	ComponentCreationException::ComponentCreationException(const std::string& componentName, uint32_t entityId, const std::string& reason)
		: ComponentException(std::format("Failed to create component '{}' for entity {}: {}", componentName, entityId, reason)) {
		HARMONY_ERROR("Component '{}' creation failed for entity {}: {}", componentName, entityId, reason);
	}

	ComponentNotFoundException::ComponentNotFoundException(const std::string& componentName, uint32_t entityId)
		: ComponentException(std::format("Component '{}' not found on entity {}", componentName, entityId)) {
		HARMONY_WARN("Component '{}' not found on entity {}", componentName, entityId);
	}

	ComponentNotRegisteredException::ComponentNotRegisteredException(const std::string& componentName)
		: ComponentException(std::format("Component type '{}' is not registered", componentName)) {
		HARMONY_ERROR("Component type '{}' is not registered", componentName);
	}

	ComponentOperationException::ComponentOperationException(const std::string& operation, const std::string& reason)
		: ComponentException(std::format("Component operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("Component operation '{}' failed: {}", operation, reason);
	}

	// ============================================================================
	// Resource Handler Exceptions
	// ============================================================================

	ResourceException::ResourceException(const std::string& message)
		: HarmonyException(std::format("Resource: {}", message)) {
	}

	ResourceLoadException::ResourceLoadException(const std::string& resourceType, const std::string& filePath, const std::string& reason)
		: ResourceException(std::format("Failed to load {} from '{}': {}", resourceType, filePath, reason)) {
		HARMONY_ERROR("Failed to load {} from '{}': {}", resourceType, filePath, reason);
	}

	ResourceUnloadException::ResourceUnloadException(const std::string& resourceType, uint64_t resourceId, const std::string& reason)
		: ResourceException(std::format("Failed to unload {} (ID: {}): {}", resourceType, resourceId, reason)) {
		HARMONY_ERROR("Failed to unload {} (ID: {}): {}", resourceType, resourceId, reason);
	}

	ResourceNotFoundException::ResourceNotFoundException(const std::string& resourceType, uint64_t resourceId)
		: ResourceException(std::format("{} with ID {} not found", resourceType, resourceId)) {
		HARMONY_WARN("{} with ID {} not found", resourceType, resourceId);
	}

	ResourceMemoryCapExceededException::ResourceMemoryCapExceededException(std::size_t requestedSize, std::size_t currentSize, std::size_t maxSize)
		: ResourceException(std::format("Memory cap exceeded: requested {} bytes, current {} bytes, max {} bytes", requestedSize, currentSize, maxSize)) {
		HARMONY_ERROR("Memory cap exceeded: requested {} bytes, current {} bytes, max {} bytes", requestedSize, currentSize, maxSize);
	}

	InvalidResourceIdException::InvalidResourceIdException(uint64_t resourceId, const std::string& reason)
		: ResourceException(std::format("Invalid resource ID {}: {}", resourceId, reason)) {
		HARMONY_ERROR("Invalid resource ID {}: {}", resourceId, reason);
	}

	ResourceOperationException::ResourceOperationException(const std::string& operation, const std::string& reason)
		: ResourceException(std::format("Resource operation '{}' failed: {}", operation, reason)) {
		HARMONY_ERROR("Resource operation '{}' failed: {}", operation, reason);
	}

	// ============================================================================
	// Texture Resource Exceptions
	// ============================================================================

	TextureResourceException::TextureResourceException(const std::string& message)
		: ResourceException(std::format("Texture: {}", message)) {
	}

	TextureLoadException::TextureLoadException(const std::string& filePath, const std::string& reason)
		: TextureResourceException(std::format("Failed to load texture from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load texture from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Image Resource Exceptions
	// ============================================================================

	ImageResourceException::ImageResourceException(const std::string& message)
		: ResourceException(std::format("Image: {}", message)) {
	}

	ImageLoadException::ImageLoadException(const std::string& filePath, const std::string& reason)
		: ImageResourceException(std::format("Failed to load image from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load image from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Font Resource Exceptions
	// ============================================================================

	FontResourceException::FontResourceException(const std::string& message)
		: ResourceException(std::format("Font: {}", message)) {
	}

	FontLoadException::FontLoadException(const std::string& filePath, const std::string& reason)
		: FontResourceException(std::format("Failed to load font from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load font from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Sound Resource Exceptions
	// ============================================================================

	SoundResourceException::SoundResourceException(const std::string& message)
		: ResourceException(std::format("Sound: {}", message)) {
	}

	SoundLoadException::SoundLoadException(const std::string& filePath, const std::string& reason)
		: SoundResourceException(std::format("Failed to load sound from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load sound from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Music Resource Exceptions
	// ============================================================================

	MusicResourceException::MusicResourceException(const std::string& message)
		: ResourceException(std::format("Music: {}", message)) {
	}

	MusicLoadException::MusicLoadException(const std::string& filePath, const std::string& reason)
		: MusicResourceException(std::format("Failed to load music from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load music from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Model Resource Exceptions
	// ============================================================================

	ModelResourceException::ModelResourceException(const std::string& message)
		: ResourceException(std::format("Model: {}", message)) {
	}

	ModelLoadException::ModelLoadException(const std::string& filePath, const std::string& reason)
		: ModelResourceException(std::format("Failed to load model from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load model from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Material Resource Exceptions
	// ============================================================================

	MaterialResourceException::MaterialResourceException(const std::string& message)
		: ResourceException(std::format("Material: {}", message)) {
	}

	MaterialLoadException::MaterialLoadException(const std::string& reason)
		: MaterialResourceException(std::format("Failed to load material: {}", reason)) {
		HARMONY_ERROR("Failed to load material: {}", reason);
	}

	// ============================================================================
	// Shader Resource Exceptions
	// ============================================================================

	ShaderResourceException::ShaderResourceException(const std::string& message)
		: ResourceException(std::format("Shader: {}", message)) {
	}

	ShaderLoadException::ShaderLoadException(const std::string& filePath, const std::string& reason)
		: ShaderResourceException(std::format("Failed to load shader from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load shader from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Mesh Resource Exceptions
	// ============================================================================

	MeshResourceException::MeshResourceException(const std::string& message)
		: ResourceException(std::format("Mesh: {}", message)) {
	}

	MeshLoadException::MeshLoadException(const std::string& filePath, const std::string& reason)
		: MeshResourceException(std::format("Failed to load mesh from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load mesh from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Texture Cubemap Resource Exceptions
	// ============================================================================

	TextureCubemapResourceException::TextureCubemapResourceException(const std::string& message)
		: ResourceException(std::format("TextureCubemap: {}", message)) {
	}

	TextureCubemapLoadException::TextureCubemapLoadException(const std::string& reason)
		: TextureCubemapResourceException(std::format("Failed to load texture cubemap: {}", reason)) {
		HARMONY_ERROR("Failed to load texture cubemap: {}", reason);
	}

	// ============================================================================
	// Render Texture Resource Exceptions
	// ============================================================================

	RenderTextureResourceException::RenderTextureResourceException(const std::string& message)
		: ResourceException(std::format("RenderTexture: {}", message)) {
	}

	RenderTextureLoadException::RenderTextureLoadException(const std::string& reason)
		: RenderTextureResourceException(std::format("Failed to load render texture: {}", reason)) {
		HARMONY_ERROR("Failed to load render texture: {}", reason);
	}

	// ============================================================================
	// Wave Resource Exceptions
	// ============================================================================

	WaveResourceException::WaveResourceException(const std::string& message)
		: ResourceException(std::format("Wave: {}", message)) {
	}

	WaveLoadException::WaveLoadException(const std::string& filePath, const std::string& reason)
		: WaveResourceException(std::format("Failed to load wave from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load wave from '{}': {}", filePath, reason);
	}

	// ============================================================================
	// Model Animation Resource Exceptions
	// ============================================================================

	ModelAnimationResourceException::ModelAnimationResourceException(const std::string& message)
		: ResourceException(std::format("ModelAnimation: {}", message)) {
	}

	ModelAnimationLoadException::ModelAnimationLoadException(const std::string& filePath, const std::string& reason)
		: ModelAnimationResourceException(std::format("Failed to load model animation from '{}': {}", filePath, reason)) {
		HARMONY_ERROR("Failed to load model animation from '{}': {}", filePath, reason);
	}

} // namespace Harmony::Exceptions
