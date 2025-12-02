#include "pch.h"
#include "Properties.h"
#include "Exceptions.h"

namespace Harmony::Internals {
    Properties::Properties(ResourceID id, ResourcesHandler& resourcesHandler, const Configuration& configuration) :
		Resource_t(id, resourcesHandler, configuration) {}

	const char* Properties::getType() const { return "Properties"; }

	void Properties::load()
	{
		std::lock_guard lock(Resource_t::mutex_);
		if (std::optional<std::string> filepath = configuration.get<std::string>({ "filepath" })) { Configuration::load(*filepath); }
		else { throw Exceptions::ConfigurationException("'filepath' not specified in configuration"); }

		setLoaded(true);
	}

	void Properties::unload()
	{
		std::lock_guard lock(Resource_t::mutex_);
		Configuration::unload();
	}

}
