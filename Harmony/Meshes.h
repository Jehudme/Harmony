#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Meshes : public Resource
	{
	public:
		Meshes(ResourceID id, const Configuration& configuration, Harmony::Internals::ResourcesHandler& handler);
		~Meshes();

		const char* getType() const override;

		void load() override;
		void unload() override;

		Mesh getMesh() const;

	private:
		Mesh mesh_;
	};
}
