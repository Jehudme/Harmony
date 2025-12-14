#include "pch.h"
#include "ResourceHandler.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assert.h"
#include "Engine.h"

#include <algorithm>

namespace Harmony::Internals {
    ResourcesHandler::ResourcesHandler(Engine& engine) :
        engine_(engine)
    {
        Configuration configuration = engine_.configuration->subsection({ "resources" }).value_or(Configuration());
        
        std::vector<std::string> keys = configuration.extractKeys({});
        
        for (const std::string& key : keys) {
            std::optional<Configuration> resourceConfigurationOpt = configuration.subsection({ key });
            const Configuration resourceConfiguration = resourceConfigurationOpt.value();
            
            try {
                const Resources::ResourceID resourceID = std::stoi(key);
                
                std::optional<std::string> resourceTypeOpt = resourceConfiguration.get<std::string>({ "type" });
                
                if (!resourceTypeOpt.has_value()) {
                    HARMONY_ERROR("Resource type not found for resource ID: {}", resourceID);
                    throw Exceptions::ResourceOperationException("load", "Resource type not specified");
                }
                
                const std::string resourceType = resourceTypeOpt.value();
                
                if (resourcesFactories_.find(resourceType) == resourcesFactories_.end()) {
                    HARMONY_ERROR("No factory registered for resource type: {}", resourceType);
                    throw Exceptions::ResourceOperationException("load", std::format("Unknown resource type: {}", resourceType));
                }
                
                std::function<std::unique_ptr<Resources::Resource>(Resources::ResourceID, Configuration, ResourcesHandler&)> factoryFunction = resourcesFactories_[resourceType];
                std::unique_ptr<Resources::Resource> resource = factoryFunction(resourceID, resourceConfiguration, *this);
                
                resources_.emplace(resourceID, std::move(resource));
                HARMONY_DEBUG("Loaded resource ID {} of type {}", resourceID, resourceType);
            }
            catch (const std::invalid_argument& e) {
                std::string errorMessage = e.what();
                HARMONY_ERROR("Invalid resource ID '{}': {}", key, errorMessage);
                throw Exceptions::ResourceOperationException("load", std::format("Invalid resource ID: {}", key));
            }
            catch (const std::out_of_range& e) {
                std::string errorMessage = e.what();
                HARMONY_ERROR("Resource ID out of range '{}': {}", key, errorMessage);
                throw Exceptions::ResourceOperationException("load", std::format("Resource ID out of range: {}", key));
            }
        }
        
        HARMONY_INFO("ResourcesHandler initialized with {} resources", resources_.size());
    }

    ResourcesHandler::~ResourcesHandler() = default;

    void ResourcesHandler::handleResources()
    {
        std::shared_lock lock(m_mutex);
        
        using ResourceMapEntry = std::pair<const Resources::ResourceID, std::unique_ptr<Resources::Resource>>;
        
        for (ResourceMapEntry& resourcePair : resources_) {
            Resources::Resource* resource = resourcePair.second.get();
            
            if (!resource) {
                HARMONY_WARN("Null resource found in resources map");
                continue;
            }
            
            bool unloadable = resource->unloadable();
            if (unloadable) {
                try {
                    resource->unload();
                    HARMONY_TRACE("Unloaded resource ID: {}", resourcePair.first);
                }
                catch (const std::exception& e) {
                    std::string errorMessage = e.what();
                    HARMONY_ERROR("Failed to unload resource ID {}: {}", resourcePair.first, errorMessage);
                }
            }
        }
    }

    void ResourcesHandler::loadResource(Resources::ResourceID id)
    {
        bool resourceExists = resources_.contains(id);
        
        if (!resourceExists) {
            HARMONY_ERROR("Resource not found: ID {}", id);
            throw Exceptions::ResourceNotFoundException("Resource", id);
        }
        
        try {
            Resources::Resource* resource = resources_.at(id).get();
            
            if (!resource) {
                HARMONY_ERROR("Resource pointer is null for ID {}", id);
                throw Exceptions::ResourceOperationException("load", "Resource pointer is null");
            }
            
            resource->load();
            HARMONY_DEBUG("Resource ID {} loaded successfully", id);
        }
        catch (const Exceptions::ResourceException&) {
            throw;
        }
        catch (const std::exception& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to load resource ID {}: {}", id, errorMessage);
            throw Exceptions::ResourceLoadException("Resource", std::to_string(id), errorMessage);
        }
    }

    void ResourcesHandler::unloadResource(Resources::ResourceID id)
    {
        bool resourceExists = resources_.contains(id);
        
        if (!resourceExists) {
            HARMONY_ERROR("Resource not found: ID {}", id);
            throw Exceptions::ResourceNotFoundException("Resource", id);
        }
        
        try {
            Resources::Resource* resource = resources_.at(id).get();
            
            if (!resource) {
                HARMONY_ERROR("Resource pointer is null for ID {}", id);
                throw Exceptions::ResourceOperationException("unload", "Resource pointer is null");
            }
            
            resource->unload();
            HARMONY_DEBUG("Resource ID {} unloaded successfully", id);
        }
        catch (const Exceptions::ResourceException&) {
            throw;
        }
        catch (const std::exception& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to unload resource ID {}: {}", id, errorMessage);
            throw Exceptions::ResourceUnloadException("Resource", id, errorMessage);
        }
    }
    std::unique_ptr<Resources::ScopedResourceAccess> ResourcesHandler::acquireResource(Resources::ResourceID id)
    {
        bool resourceExists = resources_.contains(id);
        
        if (!resourceExists) {
            HARMONY_ERROR("Resource not found: ID {}", id);
            throw Exceptions::ResourceNotFoundException("Resource", id);
        }
        
        try {
            Resources::Resource* resource = resources_[id].get();
            
            if (!resource) {
                HARMONY_ERROR("Resource pointer is null for ID {}", id);
                throw Exceptions::ResourceOperationException("acquire", "Resource pointer is null");
            }
            
            bool loaded = resource->loaded();
            
            if (!loaded) {
                HARMONY_DEBUG("Resource ID {} not available, loading now", id);
                resource->load();
            }

            std::shared_mutex& resourceMutex = resource->m_mutex;
            std::shared_lock<std::shared_mutex> resourceLock(resourceMutex);
            std::unique_ptr<Resources::ScopedResourceAccess> acquirement = std::make_unique<Resources::ScopedResourceAccess>(std::move(resourceLock), *resource);
            
            HARMONY_TRACE("Resource ID {} acquired successfully", id);
            return acquirement;
        }
        catch (const Exceptions::ResourceException&) {
            throw;
        }
        catch (const std::exception& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to acquire resource ID {}: {}", id, errorMessage);
            throw Exceptions::ResourceOperationException("acquire", errorMessage);
        }
    }
    
    void ResourcesHandler::registerResourceType(const std::string& typeName, std::function<std::unique_ptr<Resources::Resource>(Resources::ResourceID, Configuration, ResourcesHandler&)> factoryFunction) {
        HARMONY_ASSERT(!typeName.empty(), "Resource type name cannot be empty");
        
        if (typeName.empty()) {
            HARMONY_ERROR("Attempted to register resource type with empty name");
            throw Exceptions::ResourceOperationException("registerResourceType", "Type name cannot be empty");
        }
        
        bool typeExists = resourcesFactories_.find(typeName) != resourcesFactories_.end();
        
        if (typeExists) {
            HARMONY_WARN("Resource type '{}' is already registered, overwriting", typeName);
        }
        
        resourcesFactories_.emplace(typeName, factoryFunction);
        HARMONY_DEBUG("Registered resource type: {}", typeName);
    }
}