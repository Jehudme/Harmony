#pragma once
#include "Resource.h"
#include "Configuration.h"

namespace Harmony::Internals {
	class Properties : public Resource_t, public Configuration
	{
	public:
		Properties(ResourceID id, ResourcesHandler& resourcesHandler, const Configuration& configuration);

		const char* getType() const override;

		void load() override;
		void unload() override;

	private:
		using Configuration::subsection;
		using Configuration::unload;
		using Configuration::merge;
		using Configuration::save;
		using Configuration::load;
		using Configuration::set;
	};
}

