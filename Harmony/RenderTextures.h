#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class RenderTextures : public Resource
	{
	public:
		RenderTextures(ResourceID id, const Configuration& configuration, Harmony::Internals::ResourcesHandler& handler);
		~RenderTextures();

		const char* getType() const override;

		void load() override;
		void unload() override;

		RenderTexture2D getRenderTexture() const;

	private:
		RenderTexture2D renderTexture_;
	};
}
