#include "pch.h"
#include "Configuration.h"
#include <fstream>
#include <variant>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace Harmony
{

    struct Configuration::Dataset {
        nlohmann::json data;
    };

    Configuration::Configuration() :
        dataset_(std::make_unique<Dataset>())
    {
    }

    void Configuration::merge(const Configuration& configuration)
    {
        dataset_->data.merge_patch(configuration.dataset_->data);
    }

    void Configuration::save(const std::string& filePath)
    {
        std::ofstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open configuration file for writing: " + filePath);
        }

        file << dataset_->data.dump(4);
        file.close();
    }

    void Configuration::load(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open configuration file for reading: " + filePath);
        }

        try
        {
            file >> dataset_->data;
        }
        catch (const nlohmann::json::parse_error& e)
        {
            throw std::runtime_error("Failed to parse configuration file: " + std::string(e.what()));
        }

        file.close();
    }

    inline const nlohmann::json* findNode(const nlohmann::json& root, const std::vector<std::string>& keys)
    {
        const nlohmann::json* node = &root;
        for (const auto& key : keys)
        {
            if (!node->contains(key)) return nullptr;
            node = &(*node)[key];
        }
        return node;
    }

    inline nlohmann::json* findOrCreateNode(nlohmann::json& root, const std::vector<std::string>& keys)
    {
        nlohmann::json* node = &root;
        for (const auto& key : keys)
        {
            node = &(*node)[key];
        }
        return node;
    }

    std::optional<Configuration::Value> Configuration::get(const std::vector<std::string>& keys) const
    {
        const auto* node = findNode(dataset_->data, keys);
        if (!node) return std::nullopt;

        if (node->is_number_integer())      return Value(node->get<int>());
        if (node->is_number_float())        return Value(node->get<float>());
        if (node->is_boolean())             return Value(node->get<bool>());
        if (node->is_string())              return Value(node->get<std::string>());

        return std::nullopt;
    }

    void Configuration::set(const std::vector<std::string>& keys, const Value& value)
    {
        auto* node = findOrCreateNode(dataset_->data, keys);

        if (value.is<int>())                *node = value.as<int>();
        else if (value.is<float>())         *node = value.as<float>();
        else if (value.is<bool>())          *node = value.as<bool>();
        else if (value.is<std::string>())   *node = value.as<std::string>();
        else throw std::runtime_error("Unsupported value type in Configuration::set");
    }

    Configuration Configuration::subsection(const std::vector<std::string>& keys) const
    {
        Configuration subsection;

        // Find the JSON node at the given path
        const auto* node = findNode(dataset_->data, keys);
        if (!node)
        {
            // Return an empty config if the path doesn't exist
            return subsection;
        }

        // Copy the subtree into the new Configuration's dataset
        subsection.dataset_->data = *node;

        return subsection;
    }
}