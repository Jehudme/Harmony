#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Images : public Resource
	{
	public:
		Images(ResourceID id, const Configuration& configuration);
		~Images();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Image getImage() const;

	private:
		Image image_;
	};
}
