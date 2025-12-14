#include "pch.h"
#include "ModelIDComponent.h"
#include "ComponentsHandler.h"

HARMONY_REGISTER_COMPONENT(Harmony::Components::ModelID, model);

namespace Harmony::Components
{
	ModelID::ModelID(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS)
	{
		if (!configuration.get<Resources::ResourceID>({}).has_value())
		{
			HARMONY_ERROR("ModelID initialization failed: 'modelResourceId' not specified in configuration");
			throw Exceptions::ConfigurationKeyNotFoundException("modelResourceId");
		}

		m_modelResourceId = configuration.get<Resources::ResourceID>({}).value();
	}

	ModelID::~ModelID() = default;

	Resources::ResourceID ModelID::resourceId() const
	{
		return m_modelResourceId;
	}
}
