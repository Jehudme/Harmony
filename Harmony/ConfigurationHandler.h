#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <optional>
#include "Configuration.h"

namespace Harmony::Internals {
	class Engine;
}

namespace Harmony::Internals {

	/// ConfigurationHandler - Thread-safe storage and management of engine configuration
	/// Provides centralized access to configuration values with proper validation
	class ConfigurationHandler {
	public:
		/// Constructor - Initializes with a configuration object
		/// @param configuration The configuration to manage
		explicit ConfigurationHandler(const Configuration& configuration);

		/// Destructor
		~ConfigurationHandler();

		// Prevent copying (configuration handler should be unique)
		ConfigurationHandler(const ConfigurationHandler&) = delete;
		ConfigurationHandler& operator=(const ConfigurationHandler&) = delete;

		// Allow moving
		ConfigurationHandler(ConfigurationHandler&&) noexcept = default;
		ConfigurationHandler& operator=(ConfigurationHandler&&) noexcept = default;

		/// Get a configuration value by key path
		/// @tparam Type The type of value to retrieve
		/// @param keys The key path to the value
		/// @return Optional containing the value if found
		template<typename Type>
		std::optional<Type> get(const std::vector<std::string>& keys) const;

		/// Set a configuration value by key path
		/// @tparam Type The type of value to set
		/// @param keys The key path to the value
		/// @param value The value to set
		template<typename Type>
		void set(const std::vector<std::string>& keys, const Type& value);

		/// Get a subsection of the configuration
		/// @param keys The key path to the subsection
		/// @return Optional containing the subsection configuration if found
		std::optional<Configuration> subsection(const std::vector<std::string>& keys) const;

		/// Merge another configuration into this one
		/// @param configuration The configuration to merge
		void merge(const Configuration& configuration);

		/// Save the configuration to a file
		/// @param filePath The path to save to
		void save(const std::filesystem::path& filePath);

		/// Load configuration from a file
		/// @param filePath The path to load from
		void load(const std::filesystem::path& filePath);

		/// Debug print the configuration
		void debugPrint() const;

		/// Get the underlying configuration (const version)
		const Configuration& getConfiguration() const;

		/// Get a copy of the underlying configuration
		Configuration getConfigurationCopy() const;

	private:
		mutable std::mutex mutex_;
		Configuration configuration_;
	};

	// Template implementations
	template<typename Type>
	std::optional<Type> ConfigurationHandler::get(const std::vector<std::string>& keys) const {
		std::lock_guard lock(mutex_);
		return configuration_.get<Type>(keys);
	}

	template<typename Type>
	void ConfigurationHandler::set(const std::vector<std::string>& keys, const Type& value) {
		std::lock_guard lock(mutex_);
		configuration_.set<Type>(keys, value);
	}

} // namespace Harmony::Internals
