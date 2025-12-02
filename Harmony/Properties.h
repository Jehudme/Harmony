#pragma once
#include "Resource.h"
#include "Configuration.h"

namespace Harmony::Resources {
	class Properties : public Resource, public Configuration
	{
	public:
		Properties(ResourceID id, const Configuration& configuration);

		const char* getType() const override;

		void load() override;
		void unload() override;

	private:
		using Configuration::subsection;
		using Configuration::merge;
		using Configuration::save;
		using Configuration::load;
		using Configuration::set;
	};
}

