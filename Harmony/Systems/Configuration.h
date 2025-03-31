#pragma once

#include "Systems/Object.h"
#include "nlohmann/json.hpp"
#include <optional>
#include <memory>
#include <string>
#include <initializer_list>

namespace Harmony
{
    class Configuration : public Object
    {
    public:
        Configuration(const uint64_t& uniqueID);

        void setFilePath(const std::string& path);
        const std::string& getFilePath() const;

        void importFileData();
        void exportFileData();

        template<typename Type>
        std::optional<Type> get(std::initializer_list<std::string> keys);

        template<typename Type>
        void set(std::initializer_list<std::string> keys, const Type& value);

        std::shared_ptr<Configuration> get(std::initializer_list<std::string> keys);

    private:
        nlohmann::json jsonInstance_;
        std::string filePath_;
    };

    // Template method definitions

    template<typename Type>
    std::optional<Type> Configuration::get(std::initializer_list<std::string> keys)
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

    template<typename Type>
    void Configuration::set(std::initializer_list<std::string> keys, const Type& value)
    {
        nlohmann::json* current = &jsonInstance_;
        for (auto it = keys.begin(); it != keys.end(); ++it)
        {
            // If we're at the last key, set the value.
            if (std::next(it) == keys.end())
            {
                (*current)[*it] = value;
                return;
            }
            current = &((*current)[*it]);
        }
    }
}
