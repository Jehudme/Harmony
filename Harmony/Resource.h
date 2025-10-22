#pragma once

namespace Harmony::Resources
{
	class Resource 
	{
	public:
		Resource();
		virtual ~Resource();
	};

	template<typename Type>
	Type& convert(Resource& resource);
}