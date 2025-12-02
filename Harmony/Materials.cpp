#include "pch.h"
#include "Materials.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(material, Harmony::Resources::Materials)

namespace Harmony::Resources
{

	Materials::Materials(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration),
		material_{},
		materialLoaded_(false)
	{
		HARMONY_DEBUG("Materials resource created with ID: {}", id);
	}

	Materials::~Materials()
	{
		if (materialLoaded_)
		{
			UnloadMaterial(material_);
			materialLoaded_ = false;
		}
	}

	const char* Materials::getType() const
	{
		return "material";
	}

	void Materials::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Loading default material");

		material_ = LoadMaterialDefault();

		materialLoaded_ = true;
		setAvailable(true);
		HARMONY_INFO("Materials resource loaded successfully");
	}

	void Materials::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);

		HARMONY_DEBUG("Unloading materials resource");

		if (materialLoaded_)
		{
			UnloadMaterial(material_);
			material_ = Material{};
			materialLoaded_ = false;
			setAvailable(false);
			HARMONY_INFO("Materials resource unloaded successfully");
		}
		else
		{
			HARMONY_WARN("Attempted to unload materials resource that was not loaded");
		}
	}

	Material Materials::getMaterial() const
	{
		std::shared_lock<std::shared_mutex> lock(Resource::mutex_);
		return material_;
	}

}
