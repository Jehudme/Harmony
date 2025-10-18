#include "pch.h"
#include "Configuration.h"

#include <nlohmann/json.hpp>

namespace Harmony::Utilities
{
    // Internal data holder for the Configuration class.
    // Encapsulates the JSON object used for storing configuration values.
    struct Configuration::Internal
    {
        nlohmann::json data;
    };

    // Constructor: initializes the internal dataset.
    Configuration::Configuration()
        : internal_(std::make_unique<Internal>()) {}


    Configuration::~Configuration() = default;

	// Copy constructor: creates a deep copy of another Configuration's data.
    Configuration::Configuration(const Configuration& other) 
    {
        internal_ = std::make_unique<Internal>();
		internal_->data = other.internal_->data;
    }

	// Copy assignment operator: assigns another Configuration's data to this one.
    Configuration& Configuration::operator=(const Configuration& other) 
    {
        if (this != &other) 
        {
            internal_ = std::make_unique<Internal>();
            internal_->data = other.internal_->data;
        }

		return *this;
    }

    // Merges another Configuration's data into this one using JSON merge_patch.
    // This performs a shallow merge: keys in the source override those in the target.
    void Configuration::merge(const Configuration& configuration) 
    {
        std::lock_guard<std::mutex> lock(mutex_);

        internal_->data.merge_patch(configuration.internal_->data);
    }

    // Saves the current configuration to a file in pretty-printed JSON format.
    void Configuration::save(const std::filesystem::path& filePath) 
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::ofstream file(filePath);
        if (!file)
            throw std::runtime_error("Failed to open configuration file: " + filePath.string());

        file << internal_->data.dump(4); // Indent with 4 spaces for readability.
    }

    // Loads configuration from a file, parsing its JSON content.
    void Configuration::load(const std::filesystem::path& filePath) {
        std::lock_guard<std::mutex> lock(mutex_);

        std::ifstream file(filePath);
        if (!file)
            throw std::runtime_error("Failed to open configuration file: " + filePath.string());

        try 
        {
            file >> internal_->data;
        }
        catch (const nlohmann::json::parse_error& e) 
        {
            throw std::runtime_error("Failed to parse configuration file: " + std::string(e.what()));
        }
    }

    namespace {
        // Traverses the JSON tree using a vector of keys.
        // Returns a pointer to the node if found, or nullptr otherwise.
        const nlohmann::json* findNode(const nlohmann::json& root, const std::vector<std::string>& keys) 
        {
            const nlohmann::json* node = &root;
            for (const auto& key : keys) 
            {
                if (!node->contains(key)) return nullptr;
                node = &(*node)[key];
            }

            return node;
        }

        // Traverses or creates the JSON path specified by keys.
        // Returns a pointer to the final node, creating intermediate objects as needed.
        nlohmann::json* findOrCreateNode(nlohmann::json& root, const std::vector<std::string>& keys) 
        {
            nlohmann::json* node = &root;
            for (const auto& key : keys)
                node = &(*node)[key];

            return node;
        }
    }

    // Retrieves a value of the specified type from the configuration.
    // Returns std::nullopt if the path doesn't exist.
    template<typename Type>
    std::optional<Type> Configuration::get(const std::vector<std::string>& keys) const 
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto* node = findNode(internal_->data, keys);
        if (!node) return std::nullopt;
        return node->get<Type>();
    }

    // Sets a value at the specified path in the configuration.
    // Intermediate nodes are created if they don't exist.
    template<typename Type>
    void Configuration::set(const std::vector<std::string>& keys, const Type& value) 
    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto* node = findOrCreateNode(internal_->data, keys);
        *node = value;
    }

	// Extracts a subsection of the configuration based on the provided keys.
	// Returns an optional Configuration object containing the subsection if found.
    std::optional<Configuration> Configuration::subsection(const std::vector<std::string>& keys) const 
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto* node = findNode(internal_->data, keys);
        if (node) 
        {
            Configuration configuration;
            configuration.internal_->data = *node;
			return configuration;
        }

		return std::nullopt;
    }

    // Extracts all top-level keys in the configuration.
    std::vector<std::string> Configuration::extractKeys(const std::vector<std::string>& keys) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto* node = findNode(internal_->data, keys);
        if (!node || !node->is_object())
            return {}; // empty vector

        std::vector<std::string> rKeys;
        rKeys.reserve(node->size()); // avoid reallocations

        for (const auto& [key, _] : node->items())
            rKeys.push_back(key);

        return rKeys;
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

    template std::optional<std::vector<int>> Configuration::get<std::vector<int>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<unsigned int>> Configuration::get<std::vector<unsigned int>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<int64_t>> Configuration::get<std::vector<int64_t>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<uint64_t>> Configuration::get<std::vector<uint64_t>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<float>> Configuration::get<std::vector<float>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<double>>  Configuration::get<std::vector<double>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<bool>> Configuration::get<std::vector<bool>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<std::string>> Configuration::get<std::vector<std::string>>(const std::vector<std::string>&) const;
    
    template void Configuration::set<std::vector<int>>(const std::vector<std::string>&, const std::vector<int>&);
    template void Configuration::set<std::vector<unsigned int>>(const std::vector<std::string>&, const std::vector<unsigned int>&);
    template void Configuration::set<std::vector<int64_t>>(const std::vector<std::string>&, const std::vector<int64_t>&);
    template void Configuration::set<std::vector<uint64_t>>(const std::vector<std::string>&, const std::vector<uint64_t>&);
    template void Configuration::set<std::vector<float>>(const std::vector<std::string>&, const std::vector<float>&);
    template void Configuration::set<std::vector<double>>(const std::vector<std::string>&, const std::vector<double>&);
    template void Configuration::set<std::vector<bool>>(const std::vector<std::string>&, const std::vector<bool>&);
    template void Configuration::set<std::vector<std::string>>(const std::vector<std::string>&, const std::vector<std::string>&);

} // namespace Harmony