#include "pch.h"
#include "Configuration.h"

#include <fstream>
#include <variant>
#include <stdexcept>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace Harmony {

    struct Configuration::Dataset {
        nlohmann::json data;
    };

    Configuration::Configuration()
        : dataset_(std::make_unique<Dataset>()) {
    }

    void Configuration::merge(const Configuration& configuration) {
        dataset_->data.merge_patch(configuration.dataset_->data);
    }

    void Configuration::save(const std::filesystem::path& filePath) {
        std::ofstream file(filePath);
        if (!file) {
            throw std::runtime_error("Failed to open configuration file: " + filePath.string());
        }
        file << dataset_->data.dump(4);
    }

    void Configuration::load(const std::filesystem::path& filePath) {
        std::ifstream file(filePath);
        if (!file) {
            throw std::runtime_error("Failed to open configuration file: " + filePath.string());
        }

        try {
            file >> dataset_->data;
        }
        catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("Failed to parse configuration file: " + std::string(e.what()));
        }
    }

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
            for (const auto& key : keys) {
                node = &(*node)[key];
            }
            return node;
        }
    }

    template<typename Type>
    std::optional<Type> Configuration::get(const std::vector<std::string>& keys) const {
        const auto* node = findNode(dataset_->data, keys);
        if (!node) return std::nullopt;
        return node->get<Type>();
    }

    template<typename Type>
    void Configuration::set(const std::vector<std::string>& keys, const Type& value) {
        auto* node = findOrCreateNode(dataset_->data, keys);
        *node = value;
    }

    Configuration Configuration::subsection(const std::vector<std::string>& keys) const {
        Configuration subsection;
        const auto* node = findNode(dataset_->data, keys);
        if (node) {
            subsection.dataset_->data = *node;
        }
        return subsection;
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


} // namespace Harmony