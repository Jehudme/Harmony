#pragma once
#include "Object.h"

namespace Harmony
{
	class Configuration;

	class Resource : public Object
	{
	public:
		Resource(std::shared_ptr<Configuration> configuration);
		virtual void reload() = 0;

		bool isValid() const;

	protected:
		std::shared_ptr<Configuration> configuration_;
		bool isValid_;      
	};
}
