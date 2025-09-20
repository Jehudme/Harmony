#pragma once
#include "Value.h"

namespace Harmony {
	class Configuration {
	public:
		Configuration();

		void merge(const Configuration& configuration);

		void save(const std::filesystem::path& filePath);
		void load(const std::filesystem::path& filePath);

		Configuration subsection(const std::vector<std::string>& keys) const;

		template<typename Type>
		std::optional<Type> get(const std::vector<std::string>& keys) const;

		template<typename Type>
		void set(const std::vector<std::string>& keys, const Type& value);

	private:
		struct Dataset;
		std::unique_ptr<Dataset> dataset_;
	};
}
