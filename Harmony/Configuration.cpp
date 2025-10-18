#include "pch.h"
#include "Configuration.h"
#include "Logger.h"

namespace Harmony::Errors {
    class ConfigurationError;
    ConfigurationError::ConfigurationError(const std::string& msg)
        : std::runtime_error("Configuration error: " + msg) {}
}

namespace Harmony::Utilities {

    struct Configuration::Internal {
        nlohmann::json data;
    };

    Configuration::Configuration()
        : internal_(std::make_unique<Internal>()) {
    }

    Configuration::~Configuration() = default;

    Configuration::Configuration(const Configuration& other) {
        internal_ = std::make_unique<Internal>();
        internal_->data = other.internal_->data;
    }

    Configuration& Configuration::operator=(const Configuration& other) {
        if (this != &other) {
            internal_ = std::make_unique<Internal>();
            internal_->data = other.internal_->data;
        }
        return *this;
    }

    void Configuration::merge(const Configuration& configuration) {
        std::lock_guard lock(mutex_);
        internal_->data.merge_patch(configuration.internal_->data);
    }

    void Configuration::save(const std::filesystem::path& filePath) {
        std::lock_guard lock(mutex_);
        std::ofstream file(filePath);
        if (!file) {
            HARMONY_ERROR("Failed to open configuration file for saving: {}", filePath.string());
            throw Errors::ConfigurationError("Failed to open file: " + filePath.string());
        }
        file << internal_->data.dump(4);
        HARMONY_INFO("Configuration saved to {}", filePath.string());
    }

    void Configuration::load(const std::filesystem::path& filePath) {
        std::lock_guard lock(mutex_);
        std::ifstream file(filePath);
        if (!file) {
            HARMONY_ERROR("Failed to open configuration file for loading: {}", filePath.string());
            throw Errors::ConfigurationError("Failed to open file: " + filePath.string());
        }
        try {
            file >> internal_->data;
            HARMONY_INFO("Configuration loaded from {}", filePath.string());
        }
        catch (const nlohmann::json::parse_error& e) {
            HARMONY_ERROR("Failed to parse configuration file {}: {}", filePath.string(), e.what());
            throw Errors::ConfigurationError("Parse error: " + std::string(e.what()));
        }
    }

    // Helpers
    namespace {
        const nlohmann::json* findNode(const nlohmann::json& root, const std::vector<std::string>& keys) {
            const nlohmann::json* node = &root;
            for (const auto& key : keys) {
                if (!node->contains(key)) return nullptr;
                node = &(*node)[key];
            }
            return node;
        }

        nlohmann::json* findOrCreateNode(nlohmann::json& root, const std::vector<std::string>& keys) {
            nlohmann::json* node = &root;
            for (const auto& key : keys)
                node = &(*node)[key];
            return node;
        }
    }

    template<typename Type>
    std::optional<Type> Configuration::get(const std::vector<std::string>& keys) const {
        std::lock_guard lock(mutex_);
        const auto* node = findNode(internal_->data, keys);
        if (!node) return std::nullopt;
        return node->get<Type>();
    }

    template<typename Type>
    void Configuration::set(const std::vector<std::string>& keys, const Type& value) {
        std::lock_guard lock(mutex_);
        auto* node = findOrCreateNode(internal_->data, keys);
        *node = value;
    }

    std::optional<Configuration> Configuration::subsection(const std::vector<std::string>& keys) const {
        std::lock_guard lock(mutex_);
        const auto* node = findNode(internal_->data, keys);
        if (node) {
            Configuration configuration;
            configuration.internal_->data = *node;
            return configuration;
        }
        return std::nullopt;
    }

    std::vector<std::string> Configuration::extractKeys(const std::vector<std::string>& keys) const {
        std::lock_guard lock(mutex_);
        const auto* node = findNode(internal_->data, keys);
        if (!node || !node->is_object())
            return {};
        std::vector<std::string> rKeys;
        rKeys.reserve(node->size());
        for (const auto& [key, _] : node->items())
            rKeys.push_back(key);
        return rKeys;
    }

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

} // namespace Harmony::Utilities