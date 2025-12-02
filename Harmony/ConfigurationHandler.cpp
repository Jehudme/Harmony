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
            HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration reference cannot be null");
            HARMONY_DEBUG("ConfigurationHandler initialized with configuration");
        }
        catch (const Exceptions::HarmonyException& e) {
            std::string errorMessage = e.what();
            HARMONY_CRITICAL("ConfigurationHandler initialization failed with HarmonyException: {}", errorMessage);
            throw Exceptions::ConfigurationHandlerInitializationException(errorMessage);
        }
        catch (const std::exception& e) {
            std::string errorMessage = e.what();
            HARMONY_CRITICAL("ConfigurationHandler initialization failed with exception: {}", errorMessage);
            throw Exceptions::ConfigurationHandlerInitializationException(errorMessage);
        }

        HARMONY_INFO("ConfigurationHandler initialized successfully");
    }

    ConfigurationHandler::~ConfigurationHandler() {
        HARMONY_INFO("Destroying ConfigurationHandler");
    }

    std::optional<Configuration> ConfigurationHandler::subsection(const std::vector<std::string>& keys) const {
        std::lock_guard lock(mutex_);

        bool keysEmpty = keys.empty();
        HARMONY_ASSERT(!keysEmpty, "Configuration subsection keys cannot be empty");
        
        std::string joinedKeys = fmt::format("{}", fmt::join(keys, "."));
        HARMONY_TRACE("Getting configuration subsection: {}", joinedKeys);

        try {
            return configuration_.subsection(keys);
        }
        catch (const Exceptions::ConfigurationException& e) {
            std::string errorMessage = e.what();
            std::string logMessage = fmt::format("{} : {}", joinedKeys, errorMessage);
            HARMONY_WARN("Failed to get configuration subsection '{}': {}", logMessage);
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
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to merge configuration: {}", errorMessage);
            throw;
        }
    }

    void ConfigurationHandler::save(const std::filesystem::path& filePath) {
        std::lock_guard lock(mutex_);

        std::string pathString = filePath.string();
        HARMONY_INFO("Saving configuration to file: {}", pathString);
        
        bool pathEmpty = filePath.empty();
        HARMONY_ASSERT(!pathEmpty, "Configuration file path cannot be empty");

        try {
            configuration_.save(filePath);
            HARMONY_INFO("Configuration saved successfully to: {}", pathString);
        }
        catch (const Exceptions::ConfigurationException& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to save configuration to '{}': {}", pathString, errorMessage);
            throw;
        }
    }

    void ConfigurationHandler::load(const std::filesystem::path& filePath) {
        std::lock_guard lock(mutex_);

        std::string pathString = filePath.string();
        HARMONY_INFO("Loading configuration from file: {}", pathString);
        
        bool pathEmpty = filePath.empty();
        HARMONY_ASSERT(!pathEmpty, "Configuration file path cannot be empty");

        try {
            configuration_.load(filePath);
            HARMONY_INFO("Configuration loaded successfully from: {}", pathString);
        }
        catch (const Exceptions::ConfigurationException& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to load configuration from '{}': {}", pathString, errorMessage);
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
