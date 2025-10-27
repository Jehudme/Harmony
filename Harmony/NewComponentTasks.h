#pragma once
#include "Task.h"
#include "Configuration.h"

namespace Harmony::Tasks
{
	// ============================================================================
	// Sprite Component Tasks
	// ============================================================================

	/// @brief Play sprite animation
	class PlaySpriteAnimationTask : public Tasks::Task
	{
	public:
		PlaySpriteAnimationTask(const Utilities::UUID sceneId, EntityID entityId, bool loop = true);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const bool loop_;
	};

	/// @brief Stop sprite animation
	class StopSpriteAnimationTask : public Tasks::Task
	{
	public:
		StopSpriteAnimationTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Set specific sprite animation frame
	class SetSpriteFrameTask : public Tasks::Task
	{
	public:
		SetSpriteFrameTask(const Utilities::UUID sceneId, EntityID entityId, size_t frameIndex);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const size_t frameIndex_;
	};

	/// @brief Set sprite texture
	class SetSpriteTextureTask : public Tasks::Task
	{
	public:
		SetSpriteTextureTask(const Utilities::UUID sceneId, EntityID entityId, Utilities::UUID textureId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const Utilities::UUID textureId_;
	};

	// ============================================================================
	// Camera Component Tasks
	// ============================================================================

	/// @brief Set active camera for scene
	class SetActiveCameraTask : public Tasks::Task
	{
	public:
		SetActiveCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID cameraEntityId_;
	};

	/// @brief Move camera to position
	class MoveCameraTask : public Tasks::Task
	{
	public:
		MoveCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId, float x, float y);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID cameraEntityId_;
		const float x_, y_;
	};

	/// @brief Zoom camera
	class ZoomCameraTask : public Tasks::Task
	{
	public:
		ZoomCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId, float zoomLevel);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID cameraEntityId_;
		const float zoomLevel_;
	};

	/// @brief Reset camera to default view
	class ResetCameraTask : public Tasks::Task
	{
	public:
		ResetCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID cameraEntityId_;
	};

	// ============================================================================
	// Audio Component Tasks
	// ============================================================================

	/// @brief Play sound on entity
	class PlayEntityAudioTask : public Tasks::Task
	{
	public:
		PlayEntityAudioTask(const Utilities::UUID sceneId, EntityID entityId, Utilities::UUID soundId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const Utilities::UUID soundId_;
	};

	/// @brief Stop audio on entity
	class StopEntityAudioTask : public Tasks::Task
	{
	public:
		StopEntityAudioTask(const Utilities::UUID sceneId, EntityID entityId, bool stopSound = true, bool stopMusic = true);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const bool stopSound_;
		const bool stopMusic_;
	};

	/// @brief Set audio volume
	class SetAudioVolumeTask : public Tasks::Task
	{
	public:
		SetAudioVolumeTask(const Utilities::UUID sceneId, EntityID entityId, float volume, bool isMusic = false);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const float volume_;
		const bool isMusic_;
	};

	/// @brief Play music on entity
	class PlayEntityMusicTask : public Tasks::Task
	{
	public:
		PlayEntityMusicTask(const Utilities::UUID sceneId, EntityID entityId, Utilities::UUID musicId, bool loop = true);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const Utilities::UUID musicId_;
		const bool loop_;
	};

	// ============================================================================
	// ParticleEmitter Component Tasks
	// ============================================================================

	/// @brief Start particle emitter
	class StartParticleEmitterTask : public Tasks::Task
	{
	public:
		StartParticleEmitterTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Stop particle emitter
	class StopParticleEmitterTask : public Tasks::Task
	{
	public:
		StopParticleEmitterTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Clear all particles
	class ClearParticlesTask : public Tasks::Task
	{
	public:
		ClearParticlesTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	// ============================================================================
	// PhysicsBody Component Tasks
	// ============================================================================

	/// @brief Apply force to physics body
	class ApplyForceTask : public Tasks::Task
	{
	public:
		ApplyForceTask(const Utilities::UUID sceneId, EntityID entityId, float forceX, float forceY);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const float forceX_, forceY_;
	};

	/// @brief Apply impulse to physics body
	class ApplyImpulseTask : public Tasks::Task
	{
	public:
		ApplyImpulseTask(const Utilities::UUID sceneId, EntityID entityId, float impulseX, float impulseY);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const float impulseX_, impulseY_;
	};

	/// @brief Set velocity of physics body
	class SetVelocityTask : public Tasks::Task
	{
	public:
		SetVelocityTask(const Utilities::UUID sceneId, EntityID entityId, float velocityX, float velocityY);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const float velocityX_, velocityY_;
	};

	/// @brief Clear forces on physics body
	class ClearForcesTask : public Tasks::Task
	{
	public:
		ClearForcesTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	// ============================================================================
	// Collision Component Tasks
	// ============================================================================

	/// @brief Enable collision on entity
	class EnableCollisionTask : public Tasks::Task
	{
	public:
		EnableCollisionTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Disable collision on entity
	class DisableCollisionTask : public Tasks::Task
	{
	public:
		DisableCollisionTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Set collision layer
	class SetCollisionLayerTask : public Tasks::Task
	{
	public:
		SetCollisionLayerTask(const Utilities::UUID sceneId, EntityID entityId, int layer);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const int layer_;
	};

	// ============================================================================
	// Timer Component Tasks
	// ============================================================================

	/// @brief Start timer on entity
	class StartTimerTask : public Tasks::Task
	{
	public:
		StartTimerTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Stop timer on entity
	class StopTimerTask : public Tasks::Task
	{
	public:
		StopTimerTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	/// @brief Reset timer on entity
	class ResetTimerTask : public Tasks::Task
	{
	public:
		ResetTimerTask(const Utilities::UUID sceneId, EntityID entityId);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
	};

	// ============================================================================
	// StateMachine Component Tasks
	// ============================================================================

	/// @brief Change entity state
	class ChangeEntityStateTask : public Tasks::Task
	{
	public:
		ChangeEntityStateTask(const Utilities::UUID sceneId, EntityID entityId, const std::string& stateName);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const std::string stateName_;
	};

	/// @brief Add state to entity state machine
	class AddEntityStateTask : public Tasks::Task
	{
	public:
		AddEntityStateTask(const Utilities::UUID sceneId, EntityID entityId, const std::string& stateName);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const std::string stateName_;
	};

	/// @brief Remove state from entity state machine
	class RemoveEntityStateTask : public Tasks::Task
	{
	public:
		RemoveEntityStateTask(const Utilities::UUID sceneId, EntityID entityId, const std::string& stateName);

	private:
		void run() override;

	private:
		const Utilities::UUID sceneId_;
		const EntityID entityId_;
		const std::string stateName_;
	};
}
