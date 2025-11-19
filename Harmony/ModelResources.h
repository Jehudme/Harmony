#pragma once

#include "Resource.h"
#include <raylib.h>
#include <string>

namespace Harmony {

	// ============================================================================
	// Model Resource
	// ============================================================================

	class ModelResource : public Resource_t {
	public:
		ModelResource(ResourceID id, const std::string& filePath,
			          float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~ModelResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Model"; }
		
		const Model& getModel() const;

	private:
		Model model_;
		bool loaded_;
	};

	// ============================================================================
	// Material Resource
	// ============================================================================

	class MaterialResource : public Resource_t {
	public:
		MaterialResource(ResourceID id, float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~MaterialResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Material"; }
		
		const Material& getMaterial() const;
		Material& getMaterial();

	private:
		Material material_;
		bool loaded_;
	};

	// ============================================================================
	// Shader Resource
	// ============================================================================

	class ShaderResource : public Resource_t {
	public:
		ShaderResource(ResourceID id, const std::string& vsFilePath, const std::string& fsFilePath,
			           float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~ShaderResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Shader"; }
		
		const Shader& getShader() const;

		const std::string& getVertexShaderPath() const { return vsFilePath_; }
		const std::string& getFragmentShaderPath() const { return fsFilePath_; }

	private:
		std::string vsFilePath_;
		std::string fsFilePath_;
		Shader shader_;
		bool loaded_;
	};

	// ============================================================================
	// Mesh Resource
	// ============================================================================

	class MeshResource : public Resource_t {
	public:
		MeshResource(ResourceID id, const std::string& filePath,
			         float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~MeshResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "Mesh"; }
		
		const Mesh& getMesh() const;

	private:
		Mesh mesh_;
		bool loaded_;
	};

	// ============================================================================
	// Texture Cubemap Resource
	// ============================================================================

	class TextureCubemapResource : public Resource_t {
	public:
		TextureCubemapResource(ResourceID id, const std::string faces[6],
			                   float unloadDelay = 60.0f, bool alwaysLoaded = false);
		~TextureCubemapResource() override;

		void load() override;
		void unload() override;
		bool isLoaded() const override;

		const char* getTypeName() const override { return "TextureCubemap"; }
		
		const TextureCubemap& getCubemap() const;

	private:
		std::string faces_[6];
		TextureCubemap cubemap_;
		bool loaded_;
	};

} // namespace Harmony
