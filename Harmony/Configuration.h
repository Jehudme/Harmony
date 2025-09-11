#pragma once

#include "Object.h"
#include <nlohmann/json.hpp>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <optional>


namespace Harmony {
    class Configuration : public Object {
    public:
        Configuration(const nlohmann::json& data = nlohmann::json(), uint64_t uniqueId = 0);

        void set(const nlohmann::json& data, const std::vector<std::string>& path = {});
        std::optional<nlohmann::json> get(const std::vector<std::string>& path = {}) const;

        void load();
        void unload();

        void setFilePath(const std::string& filePath);
        std::string getFilePath() const;

        template<typename Type>
        void set(const Type& data, const std::initializer_list<const char*>& path = {}) 
        {
            set(nlohmann::json(data), path);
        }

        template<typename Type>
        std::optional<Type> get(const std::vector<std::string>& path = {}) const
        {
            if (auto data = get(path)) {
                return std::make_optional<Type>(data.value().get<Type>());
            }
            return std::nullopt;
        }

    private:
        nlohmann::json data_;
        std::optional<std::string> filePath_;
    };

    static inline std::shared_ptr<Configuration> globalConfig = create<Configuration>();
}