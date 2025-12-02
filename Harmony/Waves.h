#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Waves : public Resource
	{
	public:
		Waves(ResourceID id, const Configuration& configuration);
		~Waves();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Wave getWave() const;

	private:
		Wave wave_;
	};
}
