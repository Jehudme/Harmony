#pragma once
#include "Value.h"

namespace Harmony 
{
	class Configuration
	{
	public:
		using Value = Harmony::Utilities::Value<int, float, bool, std::string>;


	public:
		Configuration();

		void merge(const Configuration& configuration);

		void save(const std::string& filePath);
		void load(const std::string& filePath);

		Configuration subsection(const std::vector<std::string>& keys) const;

		std::optional<Value> get(const std::vector<std::string>& keys) const;
		void set(const std::vector<std::string>& keys, const Value& value);

	private:
		struct Dataset;
		std::unique_ptr<Dataset> dataset_;
	};


}
