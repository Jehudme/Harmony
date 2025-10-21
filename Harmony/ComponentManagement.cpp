#include "pch.h"
#include "Scene.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Logger.h"

namespace Harmony::Management
{
	void ComponentManager::createComponent(const std::string& name, const Utilities::Configuration& configuation, const entt::entity entityId, Scenes::Scene& scene)
	{
		if (!componentFactories_.contains(name))
			throw Exceptions::ComponentNotRegistered(name);

		componentFactories_[name](configuation, entityId, scene);
		HARMONY_TRACE("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
	}
}

namespace Harmony::Exceptions 
{
	ComponentNotRegistered::ComponentNotRegistered(const std::string& componentName)
		: std::runtime_error("Component not registered: " + componentName) { HARMONY_ERROR(what()); }
}