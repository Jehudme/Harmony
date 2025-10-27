#include "pch.h"
#include "NewComponentTasks.h"
#include "Engine.h"
#include "SceneManagement.h"
#include "Scene.h"
#include "Logger.h"
#include "Sprite.h"
#include "Camera.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "PhysicsBody.h"
#include "Collision.h"
#include "Timer.h"
#include "StateMachine.h"

namespace Harmony::Tasks
{
	// ============================================================================
	// Sprite Component Tasks
	// ============================================================================

	PlaySpriteAnimationTask::PlaySpriteAnimationTask(const Utilities::UUID sceneId, EntityID entityId, bool loop)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), loop_(loop) {}

	void PlaySpriteAnimationTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("PlaySpriteAnimationTask: Scene {} not found", sceneId_);
				return;
			}

			auto& sprite = scene->componentReference<Components::Sprite>(entityId_);
			sprite.playAnimation(loop_);
			HARMONY_DEBUG("Started sprite animation on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("PlaySpriteAnimationTask failed: {}", e.what());
		}
	}

	StopSpriteAnimationTask::StopSpriteAnimationTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void StopSpriteAnimationTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("StopSpriteAnimationTask: Scene {} not found", sceneId_);
				return;
			}

			auto& sprite = scene->componentReference<Components::Sprite>(entityId_);
			sprite.stopAnimation();
			HARMONY_DEBUG("Stopped sprite animation on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("StopSpriteAnimationTask failed: {}", e.what());
		}
	}

	SetSpriteFrameTask::SetSpriteFrameTask(const Utilities::UUID sceneId, EntityID entityId, size_t frameIndex)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), frameIndex_(frameIndex) {}

	void SetSpriteFrameTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("SetSpriteFrameTask: Scene {} not found", sceneId_);
				return;
			}

			auto& sprite = scene->componentReference<Components::Sprite>(entityId_);
			sprite.setAnimationFrame(frameIndex_);
			HARMONY_DEBUG("Set sprite frame to {} on entity {}", frameIndex_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("SetSpriteFrameTask failed: {}", e.what());
		}
	}

	SetSpriteTextureTask::SetSpriteTextureTask(const Utilities::UUID sceneId, EntityID entityId, Utilities::UUID textureId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), textureId_(textureId) {}

	void SetSpriteTextureTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("SetSpriteTextureTask: Scene {} not found", sceneId_);
				return;
			}

			auto& sprite = scene->componentReference<Components::Sprite>(entityId_);
			sprite.setTexture(textureId_);
			HARMONY_DEBUG("Set sprite texture to {} on entity {}", textureId_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("SetSpriteTextureTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// Camera Component Tasks
	// ============================================================================

	SetActiveCameraTask::SetActiveCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), cameraEntityId_(cameraEntityId) {}

	void SetActiveCameraTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("SetActiveCameraTask: Scene {} not found", sceneId_);
				return;
			}

			auto& camera = scene->componentReference<Components::Camera>(cameraEntityId_);
			camera.setActive(true);
			HARMONY_INFO("Camera entity {} set as active", static_cast<unsigned int>(cameraEntityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("SetActiveCameraTask failed: {}", e.what());
		}
	}

	MoveCameraTask::MoveCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId, float x, float y)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), cameraEntityId_(cameraEntityId), x_(x), y_(y) {}

	void MoveCameraTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("MoveCameraTask: Scene {} not found", sceneId_);
				return;
			}

			auto& camera = scene->componentReference<Components::Camera>(cameraEntityId_);
			camera.setCenter(x_, y_);
			HARMONY_DEBUG("Moved camera to ({}, {})", x_, y_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("MoveCameraTask failed: {}", e.what());
		}
	}

	ZoomCameraTask::ZoomCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId, float zoomLevel)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), cameraEntityId_(cameraEntityId), zoomLevel_(zoomLevel) {}

	void ZoomCameraTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ZoomCameraTask: Scene {} not found", sceneId_);
				return;
			}

			auto& camera = scene->componentReference<Components::Camera>(cameraEntityId_);
			camera.setZoom(zoomLevel_);
			HARMONY_DEBUG("Set camera zoom to {}", zoomLevel_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ZoomCameraTask failed: {}", e.what());
		}
	}

	ResetCameraTask::ResetCameraTask(const Utilities::UUID sceneId, EntityID cameraEntityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), cameraEntityId_(cameraEntityId) {}

	void ResetCameraTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ResetCameraTask: Scene {} not found", sceneId_);
				return;
			}

			auto& camera = scene->componentReference<Components::Camera>(cameraEntityId_);
			camera.reset();
			HARMONY_DEBUG("Reset camera to default view");
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ResetCameraTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// Audio Component Tasks
	// ============================================================================

	PlayEntityAudioTask::PlayEntityAudioTask(const Utilities::UUID sceneId, EntityID entityId, Utilities::UUID soundId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), soundId_(soundId) {}

	void PlayEntityAudioTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("PlayEntityAudioTask: Scene {} not found", sceneId_);
				return;
			}

			auto& audio = scene->componentReference<Components::Audio>(entityId_);
			audio.playSound(soundId_);
			HARMONY_DEBUG("Playing sound {} on entity {}", soundId_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("PlayEntityAudioTask failed: {}", e.what());
		}
	}

	StopEntityAudioTask::StopEntityAudioTask(const Utilities::UUID sceneId, EntityID entityId, bool stopSound, bool stopMusic)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), stopSound_(stopSound), stopMusic_(stopMusic) {}

	void StopEntityAudioTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("StopEntityAudioTask: Scene {} not found", sceneId_);
				return;
			}

			auto& audio = scene->componentReference<Components::Audio>(entityId_);
			if (stopSound_) audio.stopSound();
			if (stopMusic_) audio.stopMusic();
			HARMONY_DEBUG("Stopped audio on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("StopEntityAudioTask failed: {}", e.what());
		}
	}

	SetAudioVolumeTask::SetAudioVolumeTask(const Utilities::UUID sceneId, EntityID entityId, float volume, bool isMusic)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), volume_(volume), isMusic_(isMusic) {}

	void SetAudioVolumeTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("SetAudioVolumeTask: Scene {} not found", sceneId_);
				return;
			}

			auto& audio = scene->componentReference<Components::Audio>(entityId_);
			if (isMusic_) {
				audio.setMusicVolume(volume_);
			}
			else {
				audio.setSoundVolume(volume_);
			}
			HARMONY_DEBUG("Set {} volume to {} on entity {}",
				isMusic_ ? "music" : "sound", volume_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("SetAudioVolumeTask failed: {}", e.what());
		}
	}

	PlayEntityMusicTask::PlayEntityMusicTask(const Utilities::UUID sceneId, EntityID entityId, Utilities::UUID musicId, bool loop)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), musicId_(musicId), loop_(loop) {}

	void PlayEntityMusicTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("PlayEntityMusicTask: Scene {} not found", sceneId_);
				return;
			}

			auto& audio = scene->componentReference<Components::Audio>(entityId_);
			audio.playMusic(musicId_, loop_);
			HARMONY_DEBUG("Playing music {} on entity {}", musicId_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("PlayEntityMusicTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// ParticleEmitter Component Tasks
	// ============================================================================

	StartParticleEmitterTask::StartParticleEmitterTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void StartParticleEmitterTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("StartParticleEmitterTask: Scene {} not found", sceneId_);
				return;
			}

			auto& emitter = scene->componentReference<Components::ParticleEmitter>(entityId_);
			emitter.start();
			HARMONY_DEBUG("Started particle emitter on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("StartParticleEmitterTask failed: {}", e.what());
		}
	}

	StopParticleEmitterTask::StopParticleEmitterTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void StopParticleEmitterTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("StopParticleEmitterTask: Scene {} not found", sceneId_);
				return;
			}

			auto& emitter = scene->componentReference<Components::ParticleEmitter>(entityId_);
			emitter.stop();
			HARMONY_DEBUG("Stopped particle emitter on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("StopParticleEmitterTask failed: {}", e.what());
		}
	}

	ClearParticlesTask::ClearParticlesTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void ClearParticlesTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ClearParticlesTask: Scene {} not found", sceneId_);
				return;
			}

			auto& emitter = scene->componentReference<Components::ParticleEmitter>(entityId_);
			emitter.clear();
			HARMONY_DEBUG("Cleared all particles on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ClearParticlesTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// PhysicsBody Component Tasks
	// ============================================================================

	ApplyForceTask::ApplyForceTask(const Utilities::UUID sceneId, EntityID entityId, float forceX, float forceY)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), forceX_(forceX), forceY_(forceY) {}

	void ApplyForceTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ApplyForceTask: Scene {} not found", sceneId_);
				return;
			}

			auto& physics = scene->componentReference<Components::PhysicsBody>(entityId_);
			physics.applyForce(forceX_, forceY_);
			HARMONY_DEBUG("Applied force ({}, {}) to entity {}", forceX_, forceY_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ApplyForceTask failed: {}", e.what());
		}
	}

	ApplyImpulseTask::ApplyImpulseTask(const Utilities::UUID sceneId, EntityID entityId, float impulseX, float impulseY)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), impulseX_(impulseX), impulseY_(impulseY) {}

	void ApplyImpulseTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ApplyImpulseTask: Scene {} not found", sceneId_);
				return;
			}

			auto& physics = scene->componentReference<Components::PhysicsBody>(entityId_);
			physics.applyImpulse(impulseX_, impulseY_);
			HARMONY_DEBUG("Applied impulse ({}, {}) to entity {}", impulseX_, impulseY_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ApplyImpulseTask failed: {}", e.what());
		}
	}

	SetVelocityTask::SetVelocityTask(const Utilities::UUID sceneId, EntityID entityId, float velocityX, float velocityY)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), velocityX_(velocityX), velocityY_(velocityY) {}

	void SetVelocityTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("SetVelocityTask: Scene {} not found", sceneId_);
				return;
			}

			auto& physics = scene->componentReference<Components::PhysicsBody>(entityId_);
			physics.setVelocity(velocityX_, velocityY_);
			HARMONY_DEBUG("Set velocity ({}, {}) on entity {}", velocityX_, velocityY_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("SetVelocityTask failed: {}", e.what());
		}
	}

	ClearForcesTask::ClearForcesTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void ClearForcesTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ClearForcesTask: Scene {} not found", sceneId_);
				return;
			}

			auto& physics = scene->componentReference<Components::PhysicsBody>(entityId_);
			physics.clearForces();
			HARMONY_DEBUG("Cleared forces on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ClearForcesTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// Collision Component Tasks
	// ============================================================================

	EnableCollisionTask::EnableCollisionTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void EnableCollisionTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("EnableCollisionTask: Scene {} not found", sceneId_);
				return;
			}

			auto& collision = scene->componentReference<Components::Collision>(entityId_);
			collision.setEnabled(true);
			HARMONY_DEBUG("Enabled collision on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("EnableCollisionTask failed: {}", e.what());
		}
	}

	DisableCollisionTask::DisableCollisionTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void DisableCollisionTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("DisableCollisionTask: Scene {} not found", sceneId_);
				return;
			}

			auto& collision = scene->componentReference<Components::Collision>(entityId_);
			collision.setEnabled(false);
			HARMONY_DEBUG("Disabled collision on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("DisableCollisionTask failed: {}", e.what());
		}
	}

	SetCollisionLayerTask::SetCollisionLayerTask(const Utilities::UUID sceneId, EntityID entityId, int layer)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), layer_(layer) {}

	void SetCollisionLayerTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("SetCollisionLayerTask: Scene {} not found", sceneId_);
				return;
			}

			auto& collision = scene->componentReference<Components::Collision>(entityId_);
			collision.setLayer(layer_);
			HARMONY_DEBUG("Set collision layer to {} on entity {}", layer_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("SetCollisionLayerTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// Timer Component Tasks
	// ============================================================================

	StartTimerTask::StartTimerTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void StartTimerTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("StartTimerTask: Scene {} not found", sceneId_);
				return;
			}

			auto& timer = scene->componentReference<Components::Timer>(entityId_);
			timer.start();
			HARMONY_DEBUG("Started timer on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("StartTimerTask failed: {}", e.what());
		}
	}

	StopTimerTask::StopTimerTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void StopTimerTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("StopTimerTask: Scene {} not found", sceneId_);
				return;
			}

			auto& timer = scene->componentReference<Components::Timer>(entityId_);
			timer.stop();
			HARMONY_DEBUG("Stopped timer on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("StopTimerTask failed: {}", e.what());
		}
	}

	ResetTimerTask::ResetTimerTask(const Utilities::UUID sceneId, EntityID entityId)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId) {}

	void ResetTimerTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ResetTimerTask: Scene {} not found", sceneId_);
				return;
			}

			auto& timer = scene->componentReference<Components::Timer>(entityId_);
			timer.reset();
			HARMONY_DEBUG("Reset timer on entity {}", static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ResetTimerTask failed: {}", e.what());
		}
	}

	// ============================================================================
	// StateMachine Component Tasks
	// ============================================================================

	ChangeEntityStateTask::ChangeEntityStateTask(const Utilities::UUID sceneId, EntityID entityId, const std::string& stateName)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), stateName_(stateName) {}

	void ChangeEntityStateTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("ChangeEntityStateTask: Scene {} not found", sceneId_);
				return;
			}

			auto& stateMachine = scene->componentReference<Components::StateMachine>(entityId_);
			stateMachine.changeState(stateName_);
			HARMONY_DEBUG("Changed entity {} state to '{}'", static_cast<unsigned int>(entityId_), stateName_);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("ChangeEntityStateTask failed: {}", e.what());
		}
	}

	AddEntityStateTask::AddEntityStateTask(const Utilities::UUID sceneId, EntityID entityId, const std::string& stateName)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), stateName_(stateName) {}

	void AddEntityStateTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("AddEntityStateTask: Scene {} not found", sceneId_);
				return;
			}

			auto& stateMachine = scene->componentReference<Components::StateMachine>(entityId_);
			stateMachine.addState(stateName_);
			HARMONY_DEBUG("Added state '{}' to entity {}", stateName_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("AddEntityStateTask failed: {}", e.what());
		}
	}

	RemoveEntityStateTask::RemoveEntityStateTask(const Utilities::UUID sceneId, EntityID entityId, const std::string& stateName)
		: Task(50, FastMultiThreaded), sceneId_(sceneId), entityId_(entityId), stateName_(stateName) {}

	void RemoveEntityStateTask::run()
	{
		try {
			auto scene = getEngine().sceneManagement->find(sceneId_);
			if (!scene) {
				HARMONY_ERROR("RemoveEntityStateTask: Scene {} not found", sceneId_);
				return;
			}

			auto& stateMachine = scene->componentReference<Components::StateMachine>(entityId_);
			stateMachine.removeState(stateName_);
			HARMONY_DEBUG("Removed state '{}' from entity {}", stateName_, static_cast<unsigned int>(entityId_));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("RemoveEntityStateTask failed: {}", e.what());
		}
	}
}
