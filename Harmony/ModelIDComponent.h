#pragma once
#include "ComponentsHandler.h"
#include "Resource.h"

namespace Harmony::Components 
{
	class ModelID
	{
	public:
		ModelID(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS);
		~ModelID();

		Resources::ResourceID resourceId() const;

	private:
		Resources::ResourceID m_modelResourceId;
	};
}

