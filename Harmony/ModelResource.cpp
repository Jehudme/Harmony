#include "pch.h"
#include "ModelResource.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(Model, Harmony::Resources::ModelResource);

namespace Harmony::Resources 
{
	ModelResource::ModelResource(ResourceID id, Configuration configuration, Harmony::Internals::ResourcesHandler& handler) :
		Resource(id, configuration, handler),
		m_model{ 0 }
	{
	}

	ModelResource::~ModelResource()
	{
		if (loaded()) {
			unload();
		}
	}

	const char* ModelResource::type() const
	{
		return "Model";
	}

	void ModelResource::load()
	{
		if (loaded()) { return; }

		std::unique_lock lock(m_mutex);
		std::optional<std::string> filename;
		if (filename = m_configuration.get<std::string>({ "filename" }); !filename.has_value())
			throw Exceptions::ResourceLoadException(type(), "missing", "Filename not specified in configuration");

		try { m_model = R3D_LoadModel(filename->c_str()); }
		catch (const std::exception& e) { throw Exceptions::ResourceLoadException(type(), filename.value(), e.what()); }
	}

	void ModelResource::unload()
	{
		if (!loaded()) { return; }
		std::unique_lock lock(m_mutex);
		R3D_UnloadModel(&m_model, true);
		m_model = { 0 };
	}

	bool ModelResource::loaded() const
	{
		return m_model.meshCount > 0;
	}
	R3D_Model& ModelResource::model()
	{
		return m_model;
	}
}
