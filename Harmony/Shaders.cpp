#include "pch.h"
#include "Shaders.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(shader, Harmony::Resources::Shaders)

namespace Harmony::Resources
{

	Shaders::Shaders(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		shader_{}
	{
		HARMONY_DEBUG("Shaders resource created with ID: {}", id);
	}

	Shaders::~Shaders()
	{
		if (loaded_)
		{
			UnloadShader(shader_);
			loaded_ = false;
		}
	}

	const char* Shaders::getType() const
	{
		return "shader";
	}

	void Shaders::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> vsPathOpt = configuration_.get<std::string>({ "vertexShader" });
		std::optional<std::string> fsPathOpt = configuration_.get<std::string>({ "fragmentShader" });

		const char* vsPath = nullptr;
		const char* fsPath = nullptr;

		std::string vsPathStr;
		std::string fsPathStr;

		if (vsPathOpt.has_value())
		{
			vsPathStr = vsPathOpt.value();
			vsPath = vsPathStr.c_str();
			HARMONY_DEBUG("Vertex shader path: {}", vsPathStr);
		}

		if (fsPathOpt.has_value())
		{
			fsPathStr = fsPathOpt.value();
			fsPath = fsPathStr.c_str();
			HARMONY_DEBUG("Fragment shader path: {}", fsPathStr);
		}

		HARMONY_DEBUG("Loading shader");

		shader_ = LoadShader(vsPath, fsPath);

		if (shader_.id == 0)
		{
			HARMONY_ERROR("Failed to load shader");
			
			std::string shaderInfo = "vertex: ";
			shaderInfo += (vsPath != nullptr) ? vsPathStr : "default";
			shaderInfo += ", fragment: ";
			shaderInfo += (fsPath != nullptr) ? fsPathStr : "default";
			
			throw Exceptions::ShaderLoadException(shaderInfo, "Raylib LoadShader returned invalid shader ID");
		}

		loaded_ = true;
		HARMONY_INFO("Shaders resource loaded successfully");
	}

	void Shaders::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading shaders resource");

		if (loaded_)
		{
			UnloadShader(shader_);
			shader_ = Shader{};
			loaded_ = false;
			HARMONY_INFO("Shaders resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload shaders resource that was not loaded");
		}
	}

	Shader Shaders::getShader() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return shader_;
	}

}
