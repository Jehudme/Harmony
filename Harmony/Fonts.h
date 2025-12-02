#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Fonts : public Resource
	{
	public:
		Fonts(ResourceID id, const Configuration& configuration);
		~Fonts();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Font getFont() const;

	private:
		Font font_;
	};
}
