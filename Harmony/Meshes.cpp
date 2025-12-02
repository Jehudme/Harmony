#include "pch.h"
#include "Meshes.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(mesh, Harmony::Resources::Meshes)

namespace Harmony::Resources
{

	Meshes::Meshes(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		mesh_{},
		meshLoaded_(false)
	{
		HARMONY_DEBUG("Meshes resource created with ID: {}", id);
	}

	Meshes::~Meshes()
	{
		if (meshLoaded_)
		{
			UnloadMesh(mesh_);
			meshLoaded_ = false;
		}
	}

	const char* Meshes::getType() const
	{
		return "mesh";
	}

	void Meshes::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });

		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Meshes resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}

		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading mesh from file: {}", filepath);

		Model tempModel = LoadModel(filepath.c_str());

		if (tempModel.meshCount == 0 || tempModel.meshes == nullptr)
		{
			UnloadModel(tempModel);
			HARMONY_ERROR("Failed to load mesh from file: {}", filepath);
			throw Exceptions::MeshLoadException(filepath, "Raylib LoadModel returned no meshes");
		}

		mesh_ = tempModel.meshes[0];
		UploadMesh(&mesh_, false);

		meshLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("Meshes resource loaded successfully from: {}", filepath);
	}

	void Meshes::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading meshes resource");

		if (meshLoaded_)
		{
			UnloadMesh(mesh_);
			mesh_ = Mesh{};
			meshLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("Meshes resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload meshes resource that was not loaded");
		}
	}

	Mesh Meshes::getMesh() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return mesh_;
	}

}
