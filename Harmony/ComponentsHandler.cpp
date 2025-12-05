#include "pch.h"
#include "ComponentsHandler.h"
#include "Scene.h"
#include "Configuration.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"
#include <shared_mutex>

namespace Harmony::Internals
{
	ComponentsHandler::ComponentsHandler(Engine& engine)
		: engine_(engine) 
	{
		HARMONY_DEBUG("ComponentsHandler initialized");
	}

	ComponentsHandler::~ComponentsHandler() 
	{
		HARMONY_DEBUG("ComponentsHandler destroyed");
	}

	bool ComponentsHandler::containsComponent(const std::string& name, EntityID entityId, const Scene& scene)
	{
		HARMONY_ASSERT(!name.empty(), "Component name cannot be empty");
		
		// Thread-safe read access to the contains factories
		std::shared_lock<std::shared_mutex> lock(getSharedMutex());
		
		auto& factories = getComponentContainsFactories();
		auto it = factories.find(name);
		
		if (it == factories.end()) {
			HARMONY_WARN("Component '{}' is not registered", name);
			return false;
		}
		
		bool contains = it->second(entityId, scene);
		HARMONY_TRACE("Component '{}' presence checked for entity {}: {}", name, static_cast<std::uint32_t>(entityId), contains);
		return contains;
	}

	void ComponentsHandler::createComponent(const std::string& name, const Configuration& configuration, const EntityID entityId, Scene& scene)
	{
		HARMONY_ASSERT(!name.empty(), "Component name cannot be empty");
		
		// Thread-safe read access to the constructor factories
		std::shared_lock<std::shared_mutex> lock(getSharedMutex());
		
		auto& factories = getComponentConstructorFactories();
		auto it = factories.find(name);
		
		if (it == factories.end()) {
			HARMONY_ERROR("Failed to create component '{}' for entity {}: component type not registered", 
				name, static_cast<std::uint32_t>(entityId));
			throw Exceptions::ComponentNotRegisteredException(name);
		}
		
		try {
			it->second(configuration, entityId, scene);
			HARMONY_DEBUG("Component '{}' created for entity {}", name, static_cast<std::uint32_t>(entityId));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception while creating component '{}' for entity {}: {}", 
				name, static_cast<std::uint32_t>(entityId), e.what());
			throw Exceptions::ComponentCreationException(name, static_cast<std::uint32_t>(entityId), e.what());
		}
	}

	void ComponentsHandler::deleteComponent(const std::string& name, EntityID entityId, Scene& scene)
	{
		HARMONY_ASSERT(!name.empty(), "Component name cannot be empty");
		
		// Thread-safe read access to the destructor factories
		std::shared_lock<std::shared_mutex> lock(getSharedMutex());
		
		auto& factories = getComponentDestructorFactories();
		auto it = factories.find(name);
		
		if (it == factories.end()) {
			HARMONY_ERROR("Failed to delete component '{}' from entity {}: component type not registered", 
				name, static_cast<std::uint32_t>(entityId));
			throw Exceptions::ComponentNotRegisteredException(name);
		}
		
		try {
			it->second(entityId, scene);
			HARMONY_DEBUG("Component '{}' deleted from entity {}", name, static_cast<std::uint32_t>(entityId));
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception while deleting component '{}' from entity {}: {}", 
				name, static_cast<std::uint32_t>(entityId), e.what());
			throw Exceptions::ComponentOperationException("delete", e.what());
		}
	}

	std::mutex& ComponentsHandler::getMutex()
	{
		static std::mutex mutex_;
		return mutex_;
	}

	std::shared_mutex& ComponentsHandler::getSharedMutex()
	{
		static std::shared_mutex sharedMutex_;
		return sharedMutex_;
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