#include "pch.h"
#include "Scene.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Logger.h"

namespace Harmony::Management
{
	ComponentManager::ComponentManager(Engine& engine)
		: engine_(engine) {}

	ComponentManager::~ComponentManager() = default;

	void ComponentManager::createComponent(const std::string& name, const Utilities::Configuration& configuation, const entt::entity entityId, Scenes::Scene& scene)
	{
		if (!componentFactories_.contains(name))
			throw Exceptions::ComponentNotRegistered(name);

		componentFactories_[name](configuation, entityId, scene);
		HARMONY_TRACE("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
	}
}