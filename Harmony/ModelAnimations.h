#pragma once
#include "Resource.h"
#include "Configuration.h"
#include <raylib.h>
#include <vector>

namespace Harmony::Resources
{
	class ModelAnimations : public Resource
	{
	public:
		ModelAnimations(ResourceID id, const Configuration& configuration);
		~ModelAnimations();

		const char* getType() const override;

		void load() override;
		void unload() override;

		ModelAnimation* getAnimations() const;
		int getAnimationCount() const;

	private:
		ModelAnimation* animations_;
		int animationCount_;
		bool animationsLoaded_;
	};
}
