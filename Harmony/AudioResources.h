#pragma once

#include "Resource.h"
#include <raylib.h>

namespace Harmony {

	// ============================================================================
	// Sound Resource
	// ============================================================================

	class SoundResource : public Resource_t {
	public:
		SoundResource(ResourceID id, const std::string& filePath,
			          float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~SoundResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Sound"; }
		
		const Sound& getSound() const;

	private:
		Sound sound_;
		bool loaded_;
	};

	// ============================================================================
	// Music Resource
	// ============================================================================

	class MusicResource : public Resource_t {
	public:
		MusicResource(ResourceID id, const std::string& filePath,
			          float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~MusicResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Music"; }
		
		Music& getMusic();  // Non-const because UpdateMusicStream modifies it
		const Music& getMusic() const;
		
		// Update music streaming (call every frame if playing)
		void updateStreaming();

	private:
		Music music_;
		bool loaded_;
	};

} // namespace Harmony
