#include "pch.h"
#include "Fonts.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(font, Harmony::Resources::Fonts)

namespace Harmony::Resources
{

	Fonts::Fonts(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		font_{}
	{
		HARMONY_DEBUG("Fonts resource created with ID: {}", id);
	}

	Fonts::~Fonts()
	{
		if (loaded_)
		{
			UnloadFont(font_);
			loaded_ = false;
		}
	}

	const char* Fonts::getType() const
	{
		return "font";
	}

	void Fonts::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Fonts resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		std::optional<int> fontSizeOpt = configuration_.get<int>({ "fontSize" });
		int fontSize = fontSizeOpt.value_or(32);

		HARMONY_DEBUG("Loading font from file: {} with size: {}", filepath, fontSize);

		font_ = LoadFontEx(filepath.c_str(), fontSize, nullptr, 0);

		if (font_.texture.id == 0)
		{
			HARMONY_ERROR("Failed to load font from file: {}", filepath);
			throw Exceptions::FontLoadException(filepath, "Raylib LoadFontEx returned invalid font texture");
		}

		loaded_ = true;
		HARMONY_INFO("Fonts resource loaded successfully from: {} with size: {}", filepath, fontSize);
	}

	void Fonts::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading fonts resource");

		if (loaded_)
		{
			UnloadFont(font_);
			font_ = Font{};
			loaded_ = false;
			HARMONY_INFO("Fonts resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload fonts resource that was not loaded");
		}
	}

	Font Fonts::getFont() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return font_;
	}

}
