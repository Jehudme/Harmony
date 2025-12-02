#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Models : public Resource
	{
	public:
		Models(ResourceID id, const Configuration& configuration);
		~Models();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Model getModel() const;

	private:
		Model model_;
		bool modelLoaded_;
	};
}
