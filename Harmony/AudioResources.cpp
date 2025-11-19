#include "pch.h"
#include "AudioResources.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assert.h"

namespace Harmony {

	// ============================================================================
	// Sound Resource Implementation
	// ============================================================================

	SoundResource::SoundResource(ResourceID id, const std::string& filePath,
		                         float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, sound_{}
		, loaded_(false)
	{
	}

	SoundResource::~SoundResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void SoundResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Sound file path is empty");

		sound_ = LoadSound(getFilePath().c_str());

		if (sound_.frameCount == 0) {
			throw Exceptions::SoundLoadException(getFilePath(), "Failed to load sound");
		}

		// Estimate file size (frame count * sample size * channels)
		std::size_t estimatedSize = sound_.frameCount * sizeof(short) * 2;  // Assuming stereo 16-bit
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void SoundResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(sound_.frameCount > 0, "Sound frame count is 0");

		UnloadSound(sound_);
		sound_ = {};
		loaded_ = false;
	}

	bool SoundResource::isLoaded() const
	{
		return loaded_;
	}

	const Sound& SoundResource::getSound() const
	{
		HARMONY_ASSERT(loaded_, "Sound not loaded");
		return sound_;
	}

	// ============================================================================
	// Music Resource Implementation
	// ============================================================================

	MusicResource::MusicResource(ResourceID id, const std::string& filePath,
		                         float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, music_{}
		, loaded_(false)
	{
	}

	MusicResource::~MusicResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void MusicResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Music file path is empty");

		music_ = LoadMusicStream(getFilePath().c_str());

		if (music_.frameCount == 0) {
			throw Exceptions::MusicLoadException(getFilePath(), "Failed to load music stream");
		}

		// Music streams are typically smaller in memory (streaming)
		// Estimate a reasonable buffer size
		std::size_t estimatedSize = 1024 * 1024;  // 1MB buffer estimate
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void MusicResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(music_.frameCount > 0, "Music frame count is 0");

		UnloadMusicStream(music_);
		music_ = {};
		loaded_ = false;
	}

	bool MusicResource::isLoaded() const
	{
		return loaded_;
	}

	Music& MusicResource::getMusic()
	{
		HARMONY_ASSERT(loaded_, "Music not loaded");
		return music_;
	}

	const Music& MusicResource::getMusic() const
	{
		HARMONY_ASSERT(loaded_, "Music not loaded");
		return music_;
	}

	void MusicResource::updateStreaming()
	{
		if (loaded_ && IsMusicStreamPlaying(music_)) {
			UpdateMusicStream(music_);
			updateLastUsedTime();
		}
	}

} // namespace Harmony
