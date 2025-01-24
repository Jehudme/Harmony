#include "pch.h"
#include "configuration.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include "Object.h"
#include <nlohmann/json_fwd.hpp>

namespace harmony
{
    std::shared_ptr<Configuration> configuration = utilities::create<Configuration>(nlohmann::json(), "config.json");

    Configuration::Configuration(const nlohmann::json& data, const std::string& path, const uint64_t& uniqueId)
        : core::Object(uniqueId), path(path), data(data)
    {
    }

    void Configuration::load()
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open configuration file: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        try
        {
            data = nlohmann::json::parse(buffer.str());
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Failed to parse JSON: " + std::string(e.what()));
        }
    }

    void Configuration::save()
    {
        std::ofstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open configuration file: " + path);
        }

        file << data.dump(4);
    }

    std::optional<nlohmann::json> Configuration::getData(const std::initializer_list<Key> keys) const
    {
        auto current = data;
        for (const auto& key : keys)
        {
            if (!current.contains(key))
            {
                return std::nullopt;
            }
            current = current[key];
        }
        return current;
    }

    bool Configuration::contain(const std::initializer_list<Key> keys) const
    {
        auto current = data;
        for (const auto& key : keys)
        {
            if (current.find(key) == current.end())
            {
                return false;
            }
            current = current[key];
        }
        return true;
    }
}
