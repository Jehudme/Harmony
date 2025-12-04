#include "pch.h"
#include "ComponentsHandler.h"
#include "Scene.h"
#include "Configuration.h"
#include "Logger.h"

namespace Harmony::Internals
{
	ComponentsHandler::ComponentsHandler(Engine& engine)
		: engine_(engine) {}

	ComponentsHandler::~ComponentsHandler() = default;

	bool ComponentsHandler::containsComponent(const std::string& name, EntityID entityId, const Scene& scene)
	{
		bool contains = getComponentContainsFactories()[name](entityId, scene);
		HARMONY_TRACE("Component '{}' presence checked for entity {}: {}", name, static_cast<std::uint32_t>(entityId), contains);
		return contains;
	}

	void ComponentsHandler::createComponent(const std::string& name, const Configuration& configuation, const EntityID entityId, Scene& scene)
	{
		getComponentConstructorFactories()[name](configuation, entityId, scene);
		HARMONY_TRACE("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
	}

	void ComponentsHandler::deleteComponent(const std::string& name, EntityID entityId, Scene& scene)
	{
		getComponentDestructorFactories()[name](entityId, scene);
		HARMONY_TRACE("Component '{}' deleted from entity {}", name, static_cast<std::uint32_t>(entityId));
	}

	std::mutex& ComponentsHandler::getMutex()
	{
		static std::mutex mutex_;
		return mutex_;
	}

	std::unordered_map<std::string, std::function<void(const Configuration&, EntityID, Scene&)>>& ComponentsHandler::getComponentConstructorFactories()
	{
		static std::unordered_map<std::string, std::function<void(const Configuration&, EntityID, Scene&)>> componentFactories_;
		return componentFactories_;
	}
	std::unordered_map<std::string, std::function<void(EntityID, Scene& scene)>>& ComponentsHandler::getComponentDestructorFactories()
	{
		static std::unordered_map<std::string, std::function<void(EntityID, Scene& scene)>> componentFactories_;
		return componentFactories_;
	}
	std::unordered_map<std::string, std::function<bool(EntityID, const Scene& scene)>>& ComponentsHandler::getComponentContainsFactories()
	{
		static std::unordered_map<std::string, std::function<bool(EntityID, const Scene& scene)>> componentFactories_;
		return componentFactories_;
	}
}