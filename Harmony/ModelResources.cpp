#include "pch.h"
#include "ModelResources.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assert.h"

namespace Harmony {

	// ============================================================================
	// Model Resource Implementation
	// ============================================================================

	ModelResource::ModelResource(ResourceID id, const std::string& filePath,
		                         float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, model_{}
		, loaded_(false)
	{
	}

	ModelResource::~ModelResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void ModelResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Model file path is empty");

		model_ = LoadModel(getFilePath().c_str());

		if (model_.meshCount == 0) {
			throw Exceptions::ModelLoadException(getFilePath(), "Failed to load model");
		}

		// Estimate file size based on mesh data
		std::size_t estimatedSize = 0;
		for (int i = 0; i < model_.meshCount; i++) {
			estimatedSize += model_.meshes[i].vertexCount * sizeof(float) * 3;  // vertices
			estimatedSize += model_.meshes[i].triangleCount * sizeof(unsigned short) * 3;  // indices
		}
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void ModelResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(model_.meshCount > 0, "Model mesh count is 0");

		UnloadModel(model_);
		model_ = {};
		loaded_ = false;
	}

	bool ModelResource::isLoaded() const
	{
		return loaded_;
	}

	const Model& ModelResource::getModel() const
	{
		HARMONY_ASSERT(loaded_, "Model not loaded");
		return model_;
	}

	// ============================================================================
	// Material Resource Implementation
	// ============================================================================

	MaterialResource::MaterialResource(ResourceID id, float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, "", 0, unloadDelay, alwaysLoaded)
		, material_{}
		, loaded_(false)
	{
	}

	MaterialResource::~MaterialResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void MaterialResource::load()
	{
		if (loaded_) {
			return;
		}

		// Load default material
		material_ = LoadMaterialDefault();

		// Estimate material size (small, mostly shader uniforms)
		std::size_t estimatedSize = 1024;  // 1KB estimate
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void MaterialResource::unload()
	{
		if (!loaded_) {
			return;
		}

		UnloadMaterial(material_);
		material_ = {};
		loaded_ = false;
	}

	bool MaterialResource::isLoaded() const
	{
		return loaded_;
	}

	const Material& MaterialResource::getMaterial() const
	{
		HARMONY_ASSERT(loaded_, "Material not loaded");
		return material_;
	}

	Material& MaterialResource::getMaterial()
	{
		HARMONY_ASSERT(loaded_, "Material not loaded");
		return material_;
	}

	// ============================================================================
	// Shader Resource Implementation
	// ============================================================================

	ShaderResource::ShaderResource(ResourceID id, const std::string& vsFilePath, const std::string& fsFilePath,
		                           float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, vsFilePath, 0, unloadDelay, alwaysLoaded)
		, vsFilePath_(vsFilePath)
		, fsFilePath_(fsFilePath)
		, shader_{}
		, loaded_(false)
	{
	}

	ShaderResource::~ShaderResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void ShaderResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!vsFilePath_.empty() && !fsFilePath_.empty(), "Shader file paths are empty");

		shader_ = LoadShader(vsFilePath_.c_str(), fsFilePath_.c_str());

		if (shader_.id == 0) {
			throw Exceptions::ShaderLoadException(vsFilePath_ + " / " + fsFilePath_, "Failed to load shader");
		}

		// Estimate shader size (small, compiled shader code)
		std::size_t estimatedSize = 10 * 1024;  // 10KB estimate
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void ShaderResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(shader_.id != 0, "Shader ID is 0");

		UnloadShader(shader_);
		shader_ = {};
		loaded_ = false;
	}

	bool ShaderResource::isLoaded() const
	{
		return loaded_;
	}

	const Shader& ShaderResource::getShader() const
	{
		HARMONY_ASSERT(loaded_, "Shader not loaded");
		return shader_;
	}

	// ============================================================================
	// Mesh Resource Implementation
	// ============================================================================

	MeshResource::MeshResource(ResourceID id, const std::string& filePath,
		                       float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, filePath, 0, unloadDelay, alwaysLoaded)
		, mesh_{}
		, loaded_(false)
	{
	}

	MeshResource::~MeshResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void MeshResource::load()
	{
		if (loaded_) {
			return;
		}

		HARMONY_ASSERT(!getFilePath().empty(), "Mesh file path is empty");

		// Load model first to extract mesh
		Model tempModel = LoadModel(getFilePath().c_str());

		if (tempModel.meshCount == 0) {
			UnloadModel(tempModel);
			throw Exceptions::MeshLoadException(getFilePath(), "Failed to load mesh from model");
		}

		// Take the first mesh
		mesh_ = tempModel.meshes[0];

		// Estimate file size
		std::size_t estimatedSize = mesh_.vertexCount * sizeof(float) * 3;
		estimatedSize += mesh_.triangleCount * sizeof(unsigned short) * 3;
		setFileSize(estimatedSize);

		// Don't unload the model yet as it contains the mesh data
		// In a real scenario, you'd need to properly manage this

		loaded_ = true;
		updateLastUsedTime();
	}

	void MeshResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(mesh_.vertexCount > 0, "Mesh vertex count is 0");

		UnloadMesh(mesh_);
		mesh_ = {};
		loaded_ = false;
	}

	bool MeshResource::isLoaded() const
	{
		return loaded_;
	}

	const Mesh& MeshResource::getMesh() const
	{
		HARMONY_ASSERT(loaded_, "Mesh not loaded");
		return mesh_;
	}

	// ============================================================================
	// Texture Cubemap Resource Implementation
	// ============================================================================

	TextureCubemapResource::TextureCubemapResource(ResourceID id, const std::string faces[6],
		                                           float unloadDelay, bool alwaysLoaded)
		: Resource_t(id, "", 0, unloadDelay, alwaysLoaded)
		, cubemap_{}
		, loaded_(false)
	{
		for (int i = 0; i < 6; i++) {
			faces_[i] = faces[i];
		}
	}

	TextureCubemapResource::~TextureCubemapResource()
	{
		if (loaded_) {
			unload();
		}
	}

	void TextureCubemapResource::load()
	{
		if (loaded_) {
			return;
		}

		// Load individual face images
		Image faceImages[6];
		bool allLoaded = true;

		for (int i = 0; i < 6; i++) {
			HARMONY_ASSERT(!faces_[i].empty(), "Cubemap face path is empty");
			faceImages[i] = LoadImage(faces_[i].c_str());
			if (faceImages[i].data == nullptr) {
				allLoaded = false;
				break;
			}
		}

		if (!allLoaded) {
			// Unload any loaded images
			for (int i = 0; i < 6; i++) {
				if (faceImages[i].data != nullptr) {
					UnloadImage(faceImages[i]);
				}
			}
			throw Exceptions::TextureCubemapLoadException("Failed to load one or more cubemap faces");
		}

		// Create cubemap from images
		cubemap_ = LoadTextureCubemap(faceImages[0], CUBEMAP_LAYOUT_AUTO_DETECT);

		// Unload face images
		for (int i = 0; i < 6; i++) {
			UnloadImage(faceImages[i]);
		}

		if (cubemap_.id == 0) {
			throw Exceptions::TextureCubemapLoadException("Failed to create cubemap texture");
		}

		// Estimate size (6 faces * width * height * 4 bytes)
		std::size_t estimatedSize = cubemap_.width * cubemap_.height * 4 * 6;
		setFileSize(estimatedSize);

		loaded_ = true;
		updateLastUsedTime();
	}

	void TextureCubemapResource::unload()
	{
		if (!loaded_) {
			return;
		}

		HARMONY_ASSERT(cubemap_.id != 0, "Cubemap ID is 0");

		UnloadTexture(cubemap_);
		cubemap_ = {};
		loaded_ = false;
	}

	bool TextureCubemapResource::isLoaded() const
	{
		return loaded_;
	}

	const TextureCubemap& TextureCubemapResource::getCubemap() const
	{
		HARMONY_ASSERT(loaded_, "Cubemap not loaded");
		return cubemap_;
	}

} // namespace Harmony
