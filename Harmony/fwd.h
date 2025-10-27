#pragma once
#include "pch.h"
#include "entt/fwd.hpp"

namespace Harmony {
	inline constexpr const char* VERSION = "1.0.0";
	
	// Core classes
	class Engine;
	class InputManager;

	// Type aliases
	using EntityID = entt::entity;
}

namespace Harmony::Components {
	// Component classes
	class Circle;
	class Node;
	class Rectangle;
	class Script;
	template<typename Type> class Shape;
	class Text;
	class Transform;
	class View;
}

namespace Harmony::Scenes {
	// Scene classes
	class Scene;
	class State;
}

namespace Harmony::Tasks {
	// Task structure and classes
	struct Task;
	
	// Component tasks
	class AddComponentTask;
	class BatchCreateEntitiesTask;
	class BatchDestroyEntitiesTask;
	
	// Engine tasks
	class SetFPS;
	class StopEngineTask;
	class DelayedActionTask;
	
	// Resource tasks
	class LoadResourceTask;
	class UnloadResourceTask;
	class UnloadAllResourcesTask;
	class BatchLoadResourcesTask;
	
	// Scene tasks
	class CreateEntityTask;
	class DestroyEntityTask;
	class CreateSceneTask;
	class ResetSceneTask;
	class EnableSceneDrawingTask;
	class DisableSceneDrawingTask;
	class EnableSceneUpdatingTask;
	class DisableSceneUpdatingTask;
	class DeleteSceneTask;
	class CreateSceneViewTask;
	class DeleteSceneViewTask;
	
	// State tasks
	class PushState;
	class PopState;
	class ClearStatesTask;
	class SwitchStateTask;
	
	// Utility tasks
	class CallbackTask;
	class LambdaTask;
	class LogMessageTask;
	class ConditionalTask;
	class RepeatedTask;
}

namespace Harmony::Management {
	// Manager classes
	class ComponentManager;
	class RenderManager;
	class ResourceManager;
	class SceneManager;
	class StateManager;
	class TaskManager;
	
	// Enums
	enum class RenderTargetType;
}

namespace Harmony::Resources {
	// Resource classes
	class Resource;
	class Font;
	class Music;
	class Shader;
	class Sound;
	class Texture;
}

namespace Harmony::Utilities {
	// Type aliases
	using UUID = std::uint64_t;
	using UUIDList = std::vector<UUID>;

	// Utility classes
	class Configuration;
	class Logger;
}

namespace Harmony::Input {
	// Input enums
	enum class Key;
	enum class MouseButton;
	enum class MouseWheel;
	enum class InputState;
}

namespace Harmony::Exceptions {
	// Configuration exceptions
	struct ConfigurationExceptions;
	struct OpenConfigurationFileException;
	struct ParseConfigurationFileException;
	
	// Component exceptions
	struct ComponentNotRegistered;
	struct ComponentNotFoundException;
	
	// Scene exceptions
	struct SceneConfigurationNotFoundError;
	struct ExpiredSceneError;
	
	// State exceptions
	struct StateStackPushFailed;
	struct StateStackEmptyError;
	struct StartupStatesNotDefined;
	struct StateConfigurationNotFound;
	
	// Task exceptions
	struct NullTaskException;
	struct FailedTaskExecutionException;
	struct TaskNotAssociatedWithEngineException;
	
	// Engine exceptions
	struct EngineError;
	
	// Resource exceptions
	struct ResourceException;
	struct ResourceNotFoundException;
	struct ResourceLoadException;
	struct ResourceUnloadException;
	struct ResourceConversionException;
	
	// Render exceptions
	struct RenderManagementException;
	struct InvalidRenderTargetException;
}

