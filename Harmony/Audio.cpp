#include "pch.h"
#include "Audio.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "Music.h"
#include "Logger.h"
#include <SFML/Audio.hpp>

HARMONY_REGISTER_COMPONENT(Harmony::Components::Audio, Audio)

namespace Harmony::Components
{
	// PImpl to hide SFML audio types
	struct Audio::AudioImpl {
		sf::Sound sound;
		sf::Music* music = nullptr;
		Utilities::UUID currentSoundId = 0;
		Utilities::UUID currentMusicId = 0;
		Scenes::Scene& scene;

		AudioImpl(Scenes::Scene& s) : scene(s) {}
	};

	Audio::Audio(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<AudioImpl>(scene))
	{
		try {
			// Load and play sound if specified
			if (auto soundId = configuration.get<Utilities::UUID>({ "sound" })) {
				impl_->currentSoundId = soundId.value();
				bool autoPlay = configuration.get<bool>({ "auto_play_sound" }).value_or(false);
				if (autoPlay) {
					playSound(impl_->currentSoundId);
				}
			}

			// Load and play music if specified
			if (auto musicId = configuration.get<Utilities::UUID>({ "music" })) {
				impl_->currentMusicId = musicId.value();
				bool autoPlay = configuration.get<bool>({ "auto_play_music" }).value_or(false);
				bool loop = configuration.get<bool>({ "loop_music" }).value_or(true);
				if (autoPlay) {
					playMusic(impl_->currentMusicId, loop);
				}
			}

			// Set volumes
			float soundVolume = configuration.get<float>({ "sound_volume" }).value_or(100.0f);
			float musicVolume = configuration.get<float>({ "music_volume" }).value_or(100.0f);
			impl_->sound.setVolume(soundVolume);

			// Set pitch
			float soundPitch = configuration.get<float>({ "sound_pitch" }).value_or(1.0f);
			float musicPitch = configuration.get<float>({ "music_pitch" }).value_or(1.0f);
			impl_->sound.setPitch(soundPitch);

			// Positional audio
			float x = configuration.get<float>({ "position", "x" }).value_or(0.0f);
			float y = configuration.get<float>({ "position", "y" }).value_or(0.0f);
			float z = configuration.get<float>({ "position", "z" }).value_or(0.0f);
			impl_->sound.setPosition(x, y, z);

			float minDistance = configuration.get<float>({ "min_distance" }).value_or(1.0f);
			impl_->sound.setMinDistance(minDistance);

			float attenuation = configuration.get<float>({ "attenuation" }).value_or(1.0f);
			impl_->sound.setAttenuation(attenuation);

			HARMONY_DEBUG("Audio component initialized");
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Audio component: {}", e.what());
			throw;
		}
	}

	Audio::~Audio() = default;

	void Audio::playSound(Utilities::UUID soundId) {
		try {
			impl_->currentSoundId = soundId;
			auto& resource = impl_->scene.engine.resourceManager->get("sound", soundId);
			auto& soundRes = convert<Harmony::Resources::Sound>(resource);
			impl_->sound.setBuffer(soundRes.get());
			impl_->sound.play();
			HARMONY_DEBUG("Playing sound {}", soundId);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to play sound {}: {}", soundId, e.what());
		}
	}

	void Audio::stopSound() {
		impl_->sound.stop();
	}

	void Audio::pauseSound() {
		impl_->sound.pause();
	}

	void Audio::resumeSound() {
		impl_->sound.play();
	}

	bool Audio::isSoundPlaying() const {
		return impl_->sound.getStatus() == sf::Sound::Playing;
	}

	void Audio::playMusic(Utilities::UUID musicId, bool loop) {
		try {
			impl_->currentMusicId = musicId;
			auto& resource = impl_->scene.engine.resourceManager->get("music", musicId);
			auto& musicRes = convert<Harmony::Resources::Music>(resource);
			impl_->music = &musicRes.get();
			impl_->music->setLoop(loop);
			impl_->music->play();
			HARMONY_DEBUG("Playing music {}", musicId);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to play music {}: {}", musicId, e.what());
		}
	}

	void Audio::stopMusic() {
		if (impl_->music) {
			impl_->music->stop();
		}
	}

	void Audio::pauseMusic() {
		if (impl_->music) {
			impl_->music->pause();
		}
	}

	void Audio::resumeMusic() {
		if (impl_->music) {
			impl_->music->play();
		}
	}

	bool Audio::isMusicPlaying() const {
		return impl_->music && impl_->music->getStatus() == sf::Music::Playing;
	}

	void Audio::setSoundVolume(float volume) {
		impl_->sound.setVolume(volume);
	}

	float Audio::getSoundVolume() const {
		return impl_->sound.getVolume();
	}

	void Audio::setMusicVolume(float volume) {
		if (impl_->music) {
			impl_->music->setVolume(volume);
		}
	}

	float Audio::getMusicVolume() const {
		return impl_->music ? impl_->music->getVolume() : 0.0f;
	}

	void Audio::setSoundPitch(float pitch) {
		impl_->sound.setPitch(pitch);
	}

	float Audio::getSoundPitch() const {
		return impl_->sound.getPitch();
	}

	void Audio::setMusicPitch(float pitch) {
		if (impl_->music) {
			impl_->music->setPitch(pitch);
		}
	}

	float Audio::getMusicPitch() const {
		return impl_->music ? impl_->music->getPitch() : 1.0f;
	}

	void Audio::setPosition(float x, float y, float z) {
		impl_->sound.setPosition(x, y, z);
	}

	void Audio::getPosition(float& x, float& y, float& z) const {
		const auto& pos = impl_->sound.getPosition();
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}

	void Audio::setMinDistance(float distance) {
		impl_->sound.setMinDistance(distance);
	}

	float Audio::getMinDistance() const {
		return impl_->sound.getMinDistance();
	}

	void Audio::setAttenuation(float attenuation) {
		impl_->sound.setAttenuation(attenuation);
	}

	float Audio::getAttenuation() const {
		return impl_->sound.getAttenuation();
	}
}
