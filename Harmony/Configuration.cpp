#include "pch.h"
#include "configuration.h"
#include <fstream>

namespace harmony
{
    std::shared_ptr<Configuration> configuration = utilities::onEnter<Configuration>("config.json");

    Configuration::Configuration(const std::string path, const uint64_t& uniqueId)
        : core::Object(uniqueId), path(path)
    {
    }

    void Configuration::load()
    {
        LOG_INFO(Logger::core, "Loading configuration from path: {}", path);
        std::ifstream file(path);
        if (file.is_open())
        {
            std::stringstream buffer;
            buffer << file.rdbuf(); // Read the whole file into a string stream
            std::string content = buffer.str();

            LOG_INFO(Logger::core, "Configuration file content: {}", content);

            try
            {
                this->data = nlohmann::json::parse(content); // Parse the JSON content
                LOG_TRACE(Logger::core, "Configuration loaded successfully");
            }
            catch (const std::exception& e)
            {
                LOG_ERROR(Logger::core, "Failed to parse JSON: {}", e.what());
            }
        }
        else
        {
            LOG_ERROR(Logger::core, "Unable to open configuration file: {}", path);
            throw std::runtime_error("Unable to open configuration file: " + path);
        }
    }

    void Configuration::save()
    {
        LOG_INFO(Logger::core, "Saving configuration to path: {}", path);
        std::ofstream file(path);
        if (file.is_open())
        {
            file << data.dump(4);
            file.close();
            LOG_TRACE(Logger::core, "Configuration saved successfully");
        }
        else
        {
            LOG_ERROR(Logger::core, "Unable to open configuration file: {}", path);
            throw std::runtime_error("Unable to open configuration file: " + path);
        }
    }

    std::optional<nlohmann::json> Configuration::getData(const std::initializer_list<Key> keys)
    {
        LOG_TRACE(Logger::core, "Getting data with keys");
        auto current = this->data;
        for (auto key : keys)
        {
            if (!current.contains(key))
            {
                return std::nullopt;
            }
            current = current[key];
        }
        LOG_TRACE(Logger::core, "Data retrieved successfully");
        return std::make_optional<nlohmann::json>(current);
    }

    bool Configuration::contain(const std::initializer_list<Key> keys)
    {
        auto& current = this->data;
        for (auto key : keys)
        {
            if (current.find(key) == current.end())
                return false;

            current = current[key];   
        }
        return true;
    }
}
