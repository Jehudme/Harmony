#include "pch.h"
#include "Resource.h"

namespace Harmony 
{
	Harmony::Resource::Resource(std::shared_ptr<Configuration> configuration)
		: Object(configuration), configuration_(configuration) 
	{
	}

	bool Harmony::Resource::isValid() const 
	{
		return isValid_;
	}
}
