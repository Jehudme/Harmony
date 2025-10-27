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

	void ComponentManager::createComponent(const std::string& name, const Utilities::Configuration& configuation, const EntityID entityId, Scenes::Scene& scene)
	{
		if (!getComponentConstructorFactories().contains(name)) throw Exceptions::ComponentNotRegistered(name);

		getComponentConstructorFactories()[name](configuation, entityId, scene);
		HARMONY_TRACE("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
	}

	void ComponentManager::deleteComponent(const std::string& name, EntityID entityId, Scenes::Scene& scene)
	{
		if (!getComponentDestructorFactories().contains(name)) throw Exceptions::ComponentNotRegistered(name);

		getComponentDestructorFactories()[name](entityId, scene);
		HARMONY_TRACE("Component '{}' deleted from entity {}", name, static_cast<std::uint32_t>(entityId));
	}

	std::mutex& ComponentManager::getMutex()
	{
		static std::mutex mutex_;
		return mutex_;
	}

	std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, EntityID, Scenes::Scene&)>>& ComponentManager::getComponentConstructorFactories()
	{
		static std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, EntityID, Scenes::Scene&)>> componentFactories_;
		return componentFactories_;
	}
	std::unordered_map<std::string, std::function<void(EntityID, Scenes::Scene& scene)>>& ComponentManager::getComponentDestructorFactories()
	{
		static std::unordered_map<std::string, std::function<void(EntityID, Scenes::Scene& scene)>> componentFactories_;
		return componentFactories_;
	}
}