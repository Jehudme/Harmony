#pragma once

#include "Resource.h"
#include <raylib.h>

namespace Harmony {

	// ============================================================================
	// Texture Resource
	// ============================================================================

	class TextureResource : public Resource_t {
	public:
		TextureResource(ResourceID id, const std::string& filePath, 
			            float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~TextureResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Texture"; }
		
		const Texture2D& getTexture() const;

	private:
		Texture2D texture_;
		bool loaded_;
	};

	// ============================================================================
	// Image Resource
	// ============================================================================

	class ImageResource : public Resource_t {
	public:
		ImageResource(ResourceID id, const std::string& filePath,
			          float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~ImageResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Image"; }
		
		const Image& getImage() const;

	private:
		Image image_;
		bool loaded_;
	};

	// ============================================================================
	// Font Resource
	// ============================================================================

	class FontResource : public Resource_t {
	public:
		FontResource(ResourceID id, const std::string& filePath, int fontSize = 32,
			         float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~FontResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Font"; }
		
		const Font& getFont() const;
		int getFontSize() const { return fontSize_; }

	private:
		Font font_;
		int fontSize_;
		bool loaded_;
	};

} // namespace Harmony
