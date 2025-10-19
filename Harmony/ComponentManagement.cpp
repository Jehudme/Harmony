#include "pch.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Logger.h"

namespace Harmony::Management
{
	ComponentManager::ComponentManager(Engine& engine)
		: engine_(engine) {}

	ComponentManager::~ComponentManager() = default;

	void ComponentManager::create(const std::string& name, const Utilities::Configuration& configuation, const entt::entity entityId, Scenes::Scene& scene)
	{
		if (!componentFactories_.contains(name))
		{
			HARMONY_CRITICAL("Component type '{}' not registered.", name);
			throw Errors::ComponentError("Component type '" + name + "' not registered.");
		}
	
		componentFactories_[name](configuation, entityId, scene);
		HARMONY_DEBUG("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
	}
}

Harmony::Errors::ComponentError::ComponentError(const std::string& msg)
	: std::runtime_error("Component error: " + msg) {}

