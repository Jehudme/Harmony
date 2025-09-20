#include "pch.h"
#include "Configuration.h"

#include <nlohmann/json.hpp>

namespace Harmony {

    // Internal data holder for the Configuration class.
    // Encapsulates the JSON object used for storing configuration values.
    struct Configuration::Internal {
        nlohmann::json data;
    };

    // Constructor: initializes the internal dataset.
    Configuration::Configuration()
        : internal_(std::make_unique<Internal>()) {
    }

    // Merges another Configuration's data into this one using JSON merge_patch.
    // This performs a shallow merge: keys in the source override those in the target.
    void Configuration::merge(const Configuration& configuration) {
        internal_->data.merge_patch(configuration.internal_->data);
    }

    // Saves the current configuration to a file in pretty-printed JSON format.
    void Configuration::save(const std::filesystem::path& filePath) {
        std::ofstream file(filePath);
        if (!file) {
            throw std::runtime_error("Failed to open configuration file: " + filePath.string());
        }
        file << internal_->data.dump(4); // Indent with 4 spaces for readability.
    }

    // Loads configuration from a file, parsing its JSON content.
    void Configuration::load(const std::filesystem::path& filePath) {
        std::ifstream file(filePath);
        if (!file) {
            throw std::runtime_error("Failed to open configuration file: " + filePath.string());
        }

        try {
            file >> internal_->data;
        }
        catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("Failed to parse configuration file: " + std::string(e.what()));
        }
    }

    namespace {
        // Traverses the JSON tree using a vector of keys.
        // Returns a pointer to the node if found, or nullptr otherwise.
        const nlohmann::json* findNode(const nlohmann::json& root, const std::vector<std::string>& keys) {
            const nlohmann::json* node = &root;
            for (const auto& key : keys) {
                if (!node->contains(key)) return nullptr;
                node = &(*node)[key];
            }
            return node;
        }

        // Traverses or creates the JSON path specified by keys.
        // Returns a pointer to the final node, creating intermediate objects as needed.
        nlohmann::json* findOrCreateNode(nlohmann::json& root, const std::vector<std::string>& keys) {
            nlohmann::json* node = &root;
            for (const auto& key : keys) {
                node = &(*node)[key];
            }
            return node;
        }
    }

    // Retrieves a value of the specified type from the configuration.
    // Returns std::nullopt if the path doesn't exist.
    template<typename Type>
    std::optional<Type> Configuration::get(const std::vector<std::string>& keys) const {
        const auto* node = findNode(internal_->data, keys);
        if (!node) return std::nullopt;
        return node->get<Type>();
    }

    // Sets a value at the specified path in the configuration.
    // Intermediate nodes are created if they don't exist.
    template<typename Type>
    void Configuration::set(const std::vector<std::string>& keys, const Type& value) {
        auto* node = findOrCreateNode(internal_->data, keys);
        *node = value;
    }

    // Extracts a subsection of the configuration as a new Configuration object.
    // If the path doesn't exist, returns an empty Configuration.
    Configuration Configuration::subsection(const std::vector<std::string>& keys) const {
        Configuration subsection;
        const auto* node = findNode(internal_->data, keys);
        if (node) {
            subsection.internal_->data = *node;
        }
        return subsection;
    }

    // Explicit template instantiations for supported types.
    // These ensure the compiler generates code for these types.
    template std::optional<int> Configuration::get<int>(const std::vector<std::string>&) const;
    template std::optional<unsigned int> Configuration::get<unsigned int>(const std::vector<std::string>&) const;
    template std::optional<int64_t> Configuration::get<int64_t>(const std::vector<std::string>&) const;
    template std::optional<uint64_t> Configuration::get<uint64_t>(const std::vector<std::string>&) const;
    template std::optional<float> Configuration::get<float>(const std::vector<std::string>&) const;
    template std::optional<double> Configuration::get<double>(const std::vector<std::string>&) const;
    template std::optional<bool> Configuration::get<bool>(const std::vector<std::string>&) const;
    template std::optional<std::string> Configuration::get<std::string>(const std::vector<std::string>&) const;

    template void Configuration::set<int>(const std::vector<std::string>&, const int&);
    template void Configuration::set<unsigned int>(const std::vector<std::string>&, const unsigned int&);
    template void Configuration::set<int64_t>(const std::vector<std::string>&, const int64_t&);
    template void Configuration::set<uint64_t>(const std::vector<std::string>&, const uint64_t&);
    template void Configuration::set<float>(const std::vector<std::string>&, const float&);
    template void Configuration::set<double>(const std::vector<std::string>&, const double&);
    template void Configuration::set<bool>(const std::vector<std::string>&, const bool&);
    template void Configuration::set<std::string>(const std::vector<std::string>&, const std::string&);

} // namespace Harmony