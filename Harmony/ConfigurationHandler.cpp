#include "pch.h"
#include "ConfigurationHandler.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Internals {

	ConfigurationHandler::ConfigurationHandler(const Configuration& configuration)
		: configuration_(configuration) {
		HARMONY_INFO("Initializing ConfigurationHandler");

		try {
			// Validate that configuration is not empty
			HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration reference cannot be null");
			HARMONY_DEBUG("ConfigurationHandler initialized with configuration");
		}
		catch (const Exceptions::HarmonyException& e) {
			HARMONY_CRITICAL("ConfigurationHandler initialization failed with HarmonyException: {}", e.what());
			throw Exceptions::ConfigurationHandlerInitializationException(e.what());
		}
		catch (const std::exception& e) {
			HARMONY_CRITICAL("ConfigurationHandler initialization failed with exception: {}", e.what());
			throw Exceptions::ConfigurationHandlerInitializationException(e.what());
		}

		HARMONY_INFO("ConfigurationHandler initialized successfully");
	}

	ConfigurationHandler::~ConfigurationHandler() {
		HARMONY_INFO("Destroying ConfigurationHandler");
	}

	std::optional<Configuration> ConfigurationHandler::subsection(const std::vector<std::string>& keys) const {
		std::lock_guard lock(mutex_);

		HARMONY_ASSERT(!keys.empty(), "Configuration subsection keys cannot be empty");
		HARMONY_TRACE("Getting configuration subsection: {}", fmt::join(keys, "."));

		try {
			return configuration_.subsection(keys);
		}
		catch (const Exceptions::ConfigurationException& e) {
			HARMONY_WARN("Failed to get configuration subsection '{}': {}", fmt::join(keys, "."), e.what());
			return std::nullopt;
		}
	}

	void ConfigurationHandler::merge(const Configuration& configuration) {
		std::lock_guard lock(mutex_);

		HARMONY_INFO("Merging configuration into ConfigurationHandler");
		HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration to merge cannot be null");

		try {
			configuration_.merge(configuration);
			HARMONY_DEBUG("Configuration merge completed successfully");
		}
		catch (const Exceptions::ConfigurationException& e) {
			HARMONY_ERROR("Failed to merge configuration: {}", e.what());
			throw;
		}
	}

	void ConfigurationHandler::save(const std::filesystem::path& filePath) {
		std::lock_guard lock(mutex_);

		HARMONY_INFO("Saving configuration to file: {}", filePath.string());
		HARMONY_ASSERT(!filePath.empty(), "Configuration file path cannot be empty");

		try {
			configuration_.save(filePath);
			HARMONY_INFO("Configuration saved successfully to: {}", filePath.string());
		}
		catch (const Exceptions::ConfigurationException& e) {
			HARMONY_ERROR("Failed to save configuration to '{}': {}", filePath.string(), e.what());
			throw;
		}
	}

	void ConfigurationHandler::load(const std::filesystem::path& filePath) {
		std::lock_guard lock(mutex_);

		HARMONY_INFO("Loading configuration from file: {}", filePath.string());
		HARMONY_ASSERT(!filePath.empty(), "Configuration file path cannot be empty");

		try {
			configuration_.load(filePath);
			HARMONY_INFO("Configuration loaded successfully from: {}", filePath.string());
		}
		catch (const Exceptions::ConfigurationException& e) {
			HARMONY_ERROR("Failed to load configuration from '{}': {}", filePath.string(), e.what());
			throw;
		}
	}

	void ConfigurationHandler::debugPrint() const {
		std::lock_guard lock(mutex_);
		HARMONY_DEBUG("Printing ConfigurationHandler contents:");
		configuration_.debugPrint();
	}

	const Configuration& ConfigurationHandler::getConfiguration() const {
		std::lock_guard lock(mutex_);
		return configuration_;
	}

	Configuration ConfigurationHandler::getConfigurationCopy() const {
		std::lock_guard lock(mutex_);
		return configuration_;
	}

} // namespace Harmony::Internals
