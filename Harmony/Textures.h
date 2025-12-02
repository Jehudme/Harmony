#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Textures : public Resource
	{
	public:
		Textures(ResourceID id, const Configuration& configuration);
		~Textures();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Texture2D getTexture() const;

	private:
		Texture2D texture_;
	};
}
