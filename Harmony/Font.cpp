#include "pch.h"
#include "Font.h"
#include "Exceptions.h"
#include "Configuration.h"

namespace Harmony::Resources
{
	Font::Font(const Utilities::Configuration& configuration)
	{
		std::optional<std::string> filepath;
		if(filepath = configuration.get<std::string>({ "filepath" }); !filepath.has_value())
			throw Exceptions::ConfigurationExceptions("Font resource missing 'filepath' configuration");

		if (!this->loadFromFile(filepath.value()))
			throw Exceptions::ResourceLoadException("Font", 0, "Failed to load font from file: " + filepath.value());
	}
	Font::~Font() = default;


	template<>
	Font& convert(Resource& resource)
	{
		Font* font = dynamic_cast<Font*>(&resource);
		if (!font)
			throw Exceptions::ResourceConversionException("Font", typeid(resource).name());
		return *font;
	}
}
