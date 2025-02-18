#pragma once
#include "SceneNode.h"
namespace Harmony
{
	class Configuration;

	class Group : public SceneNode {
	
	public:
		Group(std::shared_ptr<Configuration> configuration);

	};
}

