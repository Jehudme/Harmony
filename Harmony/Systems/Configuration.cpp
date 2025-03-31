/***
#include "pch.h"
#include "Configuration.h"
#include <fstream>

namespace Harmony
{
    Configuration::Configuration(const uint64_t& uniqueID)
        : Object(uniqueID)
    {
    }

    void Configuration::setFilePath(const std::string& path)
    {
        filePath_ = path;
    }

    const std::string& Configuration::getFilePath() const
    {
        return filePath_;
    }

    void Configuration::importFileData()
    {
        std::ifstream file(filePath_);
        if (file.is_open())
        {
            file >> jsonInstance_;
            file.close();
        }
    }

    void Configuration::exportFileData()
    {
        std::ofstream file(filePath_);
        if (file.is_open())
        {
            file << jsonInstance_.dump(4);
            file.close();
        }
    }

    std::shared_ptr<Configuration> Configuration::get(std::initializer_list<std::string> keys)
    {
        const nlohmann::json* current = &jsonInstance_;
        for (const auto& key : keys)
        {
            if (!current->contains(key))
                return std::nullopt;
            current = &((*current)[key]);
        }
        return current->is_null() ? std::nullopt : std::optional<Type>(current->get<Type>());
    }
}
***/