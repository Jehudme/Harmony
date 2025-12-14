#pragma once
#include "Resource.h"

namespace Harmony::Resources 
{
	class ModelResource : public Resource
	{
	public:
		ModelResource(ResourceID id, Configuration configuration, Harmony::Internals::ResourcesHandler& handler);
		virtual ~ModelResource();

		virtual const char* type() const override;
		virtual void load() override;
		virtual void unload() override;
		virtual bool loaded() const override;

		Model& model();

	private:
		Model m_model;
	};
}

