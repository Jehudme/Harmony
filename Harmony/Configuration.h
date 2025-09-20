#pragma once

#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Harmony::Internals {

    /**
     * @brief Represents a hierarchical configuration system for the Harmony engine.
     *
     * This class stores and manages configuration data, typically loaded from or saved to
     * a JSON file (implementation hidden in Internal). It supports nested keys, merging
     * configurations, and retrieving/setting typed values.
     *
     * Example usage:
     * @code
     * Harmony::Configuration config;
     * config.load("settings.json");
     * auto width = config.get<int>({"window", "width"}).value_or(800);
     * config.set({"graphics", "vsync"}, true);
     * config.save("settings.json");
     * @endcode
     */
    class Configuration {
    public:
        /**
         * @brief Constructs an empty configuration object.
         */
        Configuration();

        /**
         * @brief Merge another configuration into this one.
         *
         * Keys from the given configuration will overwrite existing keys in this configuration.
         * Nested structures are merged recursively.
         *
         * @param configuration The configuration to merge into this one.
         */
        void merge(const Configuration& configuration);

        /**
         * @brief Save the configuration to a file.
         *
         * @param filePath Path to the file where the configuration will be saved.
         *                 The format is typically JSON.
         */
        void save(const std::filesystem::path& filePath);

        /**
         * @brief Load configuration data from a file.
         *
         * @param filePath Path to the file to load from.
         *                 Existing configuration values will be replaced.
         */
        void load(const std::filesystem::path& filePath);

        /**
         * @brief Extract a subsection of the configuration.
         *
         * @param keys A vector of keys representing the path to the subsection.
         * @return A new Configuration object containing only the specified subsection.
         */
        Configuration subsection(const std::vector<std::string>& keys) const;

        /**
         * @brief Retrieve a typed value from the configuration.
         *
         * @tparam Type The expected type of the value (e.g., int, std::string, bool).
         * @param keys A vector of keys representing the path to the value.
         * @return std::optional<Type> containing the value if found and convertible,
         *         or std::nullopt if not found or type mismatch.
         */
        template<typename Type>
        std::optional<Type> get(const std::vector<std::string>& keys) const;

        /**
         * @brief Set a typed value in the configuration.
         *
         * If the key path does not exist, it will be created.
         *
         * @tparam Type The type of the value to store.
         * @param keys A vector of keys representing the path to the value.
         * @param value The value to set.
         */
        template<typename Type>
        void set(const std::vector<std::string>& keys, const Type& value);

    private:
        /**
         * @brief Opaque internal implementation details.
         *
         * This struct hides the actual storage and JSON handling logic from the public API.
         */
        struct Internal;

        /// Pointer to the internal implementation.
        std::unique_ptr<Internal> internal_;
    };

} // namespace Harmony
