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
		std::function<void(const Utilities::Configuration&, entt::entity, Scenes::Scene&)> factory;
		
		{
			if (!getComponentFactories().contains(name)) {
				throw Exceptions::ComponentNotRegistered(name);
			}
			factory = getComponentFactories()[name];
		}

		factory(configuation, entityId, scene);
		HARMONY_TRACE("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
	}
	std::mutex& ComponentManager::getMutex()
	{
		static std::mutex mutex_;
		return mutex_;
	}

	std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, entt::entity, Scenes::Scene&)>>& ComponentManager::getComponentFactories()
	{
		static std::unordered_map<std::string, std::function<void(const Utilities::Configuration&, entt::entity, Scenes::Scene&)>> componentFactories_;
		return componentFactories_;
	}
}