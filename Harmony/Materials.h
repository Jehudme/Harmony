#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Materials : public Resource
	{
	public:
		Materials(ResourceID id, const Configuration& configuration);
		~Materials();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Material getMaterial() const;

	private:
		Material material_;
	};
}
