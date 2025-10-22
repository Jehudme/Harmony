#include "pch.h"
#include "Texture.h"
#include "Exceptions.h"
#include "Configuration.h"

namespace Harmony::Resources
{
	Texture::Texture(const Utilities::Configuration& configuration)
	{
		if (std::optional<bool> smooth = configuration.get<bool>({ "smooth" }))
			this->setSmooth(smooth.value());
		else HARMONY_WARN("Texture resource missing 'smooth' configuration, defaulting to false");

		if (std::optional<bool> repeated = configuration.get<bool>({ "repeated" }))
			this->setRepeated(repeated.value());
		else HARMONY_WARN("Texture resource missing 'repeated' configuration, defaulting to false");

		std::optional<std::string> filepath;
		if(filepath = configuration.get<std::string>({ "filepath" }); !filepath.has_value())
			throw Exceptions::ConfigurationExceptions("Texture resource missing 'filepath' configuration");

		this->loadFromFile(filepath.value());
	}
	Texture::~Texture() = default;


	template<>
	Texture& convert(Resource& resource)
	{
		Texture* texture = dynamic_cast<Texture*>(&resource);
		if (!texture)
			throw Exceptions::ResourceConversionException("Texture", typeid(resource).name());
		return *texture;
	}
}
