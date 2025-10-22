#include "pch.h"
#include "Shader.h"
#include "Exceptions.h"
#include "Configuration.h"

namespace Harmony::Resources
{
	Shader::Shader(const Utilities::Configuration& configuration)
	{
		std::optional<std::string> vertexPath = configuration.get<std::string>({ "vertexShader" });
		std::optional<std::string> fragmentPath = configuration.get<std::string>({ "fragmentShader" });
		std::optional<std::string> geometryPath = configuration.get<std::string>({ "geometryShader" });

		// At least one shader type must be provided
		if (!vertexPath.has_value() && !fragmentPath.has_value() && !geometryPath.has_value())
			throw Exceptions::ConfigurationExceptions("Shader resource missing shader file configuration (at least one of 'vertexShader', 'fragmentShader', or 'geometryShader' is required)");

		bool loaded = false;

		// Load vertex and fragment shaders
		if (vertexPath.has_value() && fragmentPath.has_value() && !geometryPath.has_value()) {
			loaded = this->loadFromFile(vertexPath.value(), fragmentPath.value());
		}
		// Load vertex, geometry, and fragment shaders
		else if (vertexPath.has_value() && fragmentPath.has_value() && geometryPath.has_value()) {
			loaded = this->loadFromFile(vertexPath.value(), geometryPath.value(), fragmentPath.value());
		}
		// Load only fragment shader
		else if (!vertexPath.has_value() && fragmentPath.has_value() && !geometryPath.has_value()) {
			loaded = this->loadFromFile(fragmentPath.value(), sf::Shader::Fragment);
		}
		// Load only vertex shader
		else if (vertexPath.has_value() && !fragmentPath.has_value() && !geometryPath.has_value()) {
			loaded = this->loadFromFile(vertexPath.value(), sf::Shader::Vertex);
		}
		// Load only geometry shader
		else if (!vertexPath.has_value() && !fragmentPath.has_value() && geometryPath.has_value()) {
			loaded = this->loadFromFile(geometryPath.value(), sf::Shader::Geometry);
		}
		else {
			throw Exceptions::ConfigurationExceptions("Shader resource has invalid combination of shader files");
		}

		if (!loaded)
			throw Exceptions::ResourceLoadException("Shader", 0, "Failed to load shader from configured files");
	}
	Shader::~Shader() = default;


	template<>
	Shader& convert(Resource& resource)
	{
		Shader* shader = dynamic_cast<Shader*>(&resource);
		if (!shader)
			throw Exceptions::ResourceConversionException("Shader", typeid(resource).name());
		return *shader;
	}
}
