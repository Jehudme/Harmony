#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>

namespace Harmony::Resources
{
	class Music : public Resource
	{
	public:
		Music(ResourceID id, const Configuration& configuration, Harmony::Internals::ResourcesHandler& handler);
		~Music();

		const char* getType() const override;

		void load() override;
		void unload() override;

		::Music getMusic() const;

	private:
		::Music music_;
	};
}
