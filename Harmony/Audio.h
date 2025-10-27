#pragma once

#include <memory>
#include <string>

namespace Harmony::Components
{
	/// @brief Audio component - sound and music playback attached to entities
	/// Uses composition to hide SFML implementation details
	class Audio
	{
	public:
		Audio(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Audio();

		// Sound playback
		void playSound(Utilities::UUID soundId);
		void stopSound();
		void pauseSound();
		void resumeSound();
		bool isSoundPlaying() const;

		// Music playback
		void playMusic(Utilities::UUID musicId, bool loop = true);
		void stopMusic();
		void pauseMusic();
		void resumeMusic();
		bool isMusicPlaying() const;

		// Volume control (0-100)
		void setSoundVolume(float volume);
		float getSoundVolume() const;
		void setMusicVolume(float volume);
		float getMusicVolume() const;

		// Pitch control (default 1.0)
		void setSoundPitch(float pitch);
		float getSoundPitch() const;
		void setMusicPitch(float pitch);
		float getMusicPitch() const;

		// Positional audio (3D sound)
		void setPosition(float x, float y, float z = 0.0f);
		void getPosition(float& x, float& y, float& z) const;
		void setMinDistance(float distance);
		float getMinDistance() const;
		void setAttenuation(float attenuation);
		float getAttenuation() const;

	private:
		struct AudioImpl;
		std::unique_ptr<AudioImpl> impl_;
	};
}
