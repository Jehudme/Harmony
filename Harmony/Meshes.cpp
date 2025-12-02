#include "pch.h"
#include "Meshes.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(mesh, Harmony::Resources::Meshes)

namespace Harmony::Resources
{

	Meshes::Meshes(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		mesh_{}
	{
		HARMONY_DEBUG("Meshes resource created with ID: {}", id);
	}

	Meshes::~Meshes()
	{
		if (loaded_)
		{
			UnloadMesh(mesh_);
			loaded_ = false;
		}
	}

	const char* Meshes::getType() const
	{
		return "mesh";
	}

	void Meshes::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		std::optional<std::string> meshTypeOpt = configuration_.get<std::string>({ "meshType" });

		if (!meshTypeOpt.has_value())
		{
			HARMONY_ERROR("Meshes resource load failed: 'meshType' not specified in configuration");
			throw Exceptions::ConfigurationException("'meshType' must be specified in configuration");
		}

		std::string meshType = meshTypeOpt.value();
		HARMONY_DEBUG("Loading mesh of type: {}", meshType);

		if (meshType == "plane")
		{
			float width = configuration_.get<float>({ "width" }).value_or(1.0f);
			float length = configuration_.get<float>({ "length" }).value_or(1.0f);
			int resX = configuration_.get<int>({ "resX" }).value_or(1);
			int resZ = configuration_.get<int>({ "resZ" }).value_or(1);
			mesh_ = GenMeshPlane(width, length, resX, resZ);
		}
		else if (meshType == "cube")
		{
			float width = configuration_.get<float>({ "width" }).value_or(1.0f);
			float height = configuration_.get<float>({ "height" }).value_or(1.0f);
			float length = configuration_.get<float>({ "length" }).value_or(1.0f);
			mesh_ = GenMeshCube(width, height, length);
		}
		else if (meshType == "sphere")
		{
			float radius = configuration_.get<float>({ "radius" }).value_or(1.0f);
			int rings = configuration_.get<int>({ "rings" }).value_or(16);
			int slices = configuration_.get<int>({ "slices" }).value_or(16);
			mesh_ = GenMeshSphere(radius, rings, slices);
		}
		else if (meshType == "cylinder")
		{
			float radius = configuration_.get<float>({ "radius" }).value_or(1.0f);
			float height = configuration_.get<float>({ "height" }).value_or(1.0f);
			int slices = configuration_.get<int>({ "slices" }).value_or(16);
			mesh_ = GenMeshCylinder(radius, height, slices);
		}
		else
		{
			HARMONY_ERROR("Unsupported mesh type: {}", meshType);
			throw Exceptions::InvalidArgumentException("meshType", std::format("Unsupported mesh type: {}", meshType));
		}

		if (mesh_.vertexCount == 0)
		{
			HARMONY_ERROR("Failed to generate mesh of type: {}", meshType);
			throw Exceptions::MeshLoadException(meshType, "Raylib mesh generation failed");
		}

		loaded_ = true;
		HARMONY_INFO("Meshes resource loaded successfully with type: {}", meshType);
	}

	void Meshes::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading meshes resource");

		if (loaded_)
		{
			UnloadMesh(mesh_);
			mesh_ = Mesh{};
			loaded_ = false;
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
