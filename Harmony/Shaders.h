#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Shaders : public Resource
	{
	public:
		Shaders(ResourceID id, const Configuration& configuration);
		~Shaders();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Shader getShader() const;

	private:
		Shader shader_;
	};
}
