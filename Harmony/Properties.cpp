#include "pch.h"
#include "Properties.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(properties, Harmony::Resources::Properties)

namespace Harmony::Resources
{

	Properties::Properties(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration) 
	{
		HARMONY_DEBUG("Properties resource created with ID: {}", id);
	}

	const char* Properties::getType() const
	{ 
		return "properties"; 
	}

	void Properties::load()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);
		
		std::optional<std::string> filepathOpt = configuration_.get<std::string>({ "filepath" });
		
		if (!filepathOpt.has_value())
		{
			HARMONY_ERROR("Properties resource load failed: 'filepath' not specified in configuration");
			throw Exceptions::ConfigurationException("'filepath' not specified in configuration");
		}
		
		std::string filepath = filepathOpt.value();
		HARMONY_DEBUG("Loading properties from file: {}", filepath);
		
		try
		{
			Configuration::load(filepath);
			loaded_ = true;
			HARMONY_INFO("Properties resource loaded successfully from: {}", filepath);
		}
		catch (const std::exception& e)
		{
			std::string errorMessage = e.what();
			HARMONY_ERROR("Failed to load properties from file '{}': {}", filepath, errorMessage);
			throw;
		}
	}

	void Properties::unload()
	{
		std::lock_guard<std::shared_mutex> lock(Resource::mutex_);
		
		HARMONY_DEBUG("Unloading properties resource");
		
		try
		{
			Configuration::clear();
			loaded_ = false;
			HARMONY_INFO("Properties resource unloaded successfully");
		}
		catch (const std::exception& e)
		{
			std::string errorMessage = e.what();
			HARMONY_ERROR("Failed to unload properties resource: {}", errorMessage);
			throw;
		}
	}

}
