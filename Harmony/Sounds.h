#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Sounds : public Resource
	{
	public:
		Sounds(ResourceID id, const Configuration& configuration);
		~Sounds();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Sound getSound() const;

	private:
		Sound sound_;
	};
}
