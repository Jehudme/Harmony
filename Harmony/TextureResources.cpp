#include "pch.h"
#include "TextureResources.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assert.h"

namespace Harmony {

	// ============================================================================
	// Texture Resource Implementation
	// ============================================================================

	TextureResource::TextureResource(ResourceID id, const std::string& filePath,
		                             float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, texture_{}
		, loaded_(false)
	{
	}

	TextureResource::~TextureResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void TextureResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Texture file path is empty");

		texture_ = LoadTexture(getFilePath().c_str());

		if (texture_.id == 0) {
			throw Exceptions::TextureLoadException(getFilePath(), "Failed to load texture");
		}

		// Calculate approximate file size (width * height * 4 bytes per pixel)
		std::size_t estimatedSize = texture_.width * texture_.height * 4;
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void TextureResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(texture_.id != 0, "Texture ID is 0");

		UnloadTexture(texture_);
		texture_ = {};
		loaded_ = false;
	}

	bool TextureResource::isLoaded() const
	{
		return loaded_;
	}

	const Texture2D& TextureResource::getTexture() const
	{
		HARMONY_ASSERT(loaded_, "Texture not loaded");
		return texture_;
	}

	// ============================================================================
	// Image Resource Implementation
	// ============================================================================

	ImageResource::ImageResource(ResourceID id, const std::string& filePath,
		                         float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, image_{}
		, loaded_(false)
	{
	}

	ImageResource::~ImageResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void ImageResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Image file path is empty");

		image_ = LoadImage(getFilePath().c_str());

		if (image_.data == nullptr) {
			throw Exceptions::ImageLoadException(getFilePath(), "Failed to load image");
		}

		// Calculate file size
		std::size_t estimatedSize = image_.width * image_.height * 4;  // RGBA
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void ImageResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(image_.data != nullptr, "Image data is null");

		UnloadImage(image_);
		image_ = {};
		loaded_ = false;
	}

	bool ImageResource::isLoaded() const
	{
		return loaded_;
	}

	const Image& ImageResource::getImage() const
	{
		HARMONY_ASSERT(loaded_, "Image not loaded");
		return image_;
	}

	// ============================================================================
	// Font Resource Implementation
	// ============================================================================

	FontResource::FontResource(ResourceID id, const std::string& filePath, int fontSize,
		                       float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, font_{}
		, fontSize_(fontSize)
		, loaded_(false)
	{
	}

	FontResource::~FontResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void FontResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Font file path is empty");
		HARMONY_ASSERT(fontSize_ > 0, "Font size must be positive");

		font_ = LoadFontEx(getFilePath().c_str(), fontSize_, nullptr, 0);

		if (font_.texture.id == 0) {
			throw Exceptions::FontLoadException(getFilePath(), "Failed to load font");
		}

		// Estimate file size based on texture atlas size
		std::size_t estimatedSize = font_.texture.width * font_.texture.height * 4;
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void FontResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(font_.texture.id != 0, "Font texture ID is 0");

		UnloadFont(font_);
		font_ = {};
		loaded_ = false;
	}

	bool FontResource::isLoaded() const
	{
		return loaded_;
	}

	const Font& FontResource::getFont() const
	{
		HARMONY_ASSERT(loaded_, "Font not loaded");
		return font_;
	}

} // namespace Harmony
