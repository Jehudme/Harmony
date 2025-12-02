#include "pch.h"
#include "Properties.h"
#include "Exceptions.h"
#include "ResourceHandler.h"

HARMONY_REGISTER_RESOURCE(properties, Harmony::Resources::Properties)


namespace Harmony::Resources
{

	Properties::Properties(ResourceID id, const Configuration& configuration) :
		Resource(id, configuration) {}

	const char* Properties::getType() const  { 
		return "properties"; 
	}

	void Properties::load()
	{
		std::lock_guard lock(Resource::mutex_);
		if (std::optional<std::string> filepath = configuration_.get<std::string>({ "filepath" })) { Configuration::load(*filepath); }
		else { throw Exceptions::ConfigurationException("'filepath' not specified in configuration"); }

		setAvailable(true);
	}

	void Properties::unload()
	{
		std::lock_guard lock(Resource::mutex_);
		Configuration::clear();

		setAvailable(false);
	}

}
