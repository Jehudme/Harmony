#pragma once
#include "Object.h"
#include "Configuration.h"

namespace Harmony
{
	class Configuration;

	class Resource : public Object
	{
	public:
		Resource(std::shared_ptr<Configuration> configuration);
		virtual void reload() = 0;

		bool isValid() const;

		template<typename Type>
		static std::shared_ptr<Type> find(const std::shared_ptr<Configuration>& configuration, const std::string& defaultName);

	protected:
		std::shared_ptr<Configuration> configuration_;
		bool isValid_;      
	};


	template<typename Type>
	inline std::shared_ptr<Type> Resource::find(const std::shared_ptr<Configuration>& configuration, const std::string& defaultName)
	{
		try
		{
			return Harmony::find<Type>(configuration->get<std::string>({ "Name"}).value_or(defaultName));
		}
		catch (const std::exception&)
		{
			return create<Type>(configuration);
		}
	}
}
