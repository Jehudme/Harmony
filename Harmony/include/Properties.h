#pragma once
//
// Configuration API for Harmony Engine
// 
// IMPLEMENTATION NOTE: This class uses nlohmann/json internally.
// When implementing .cpp files, always include "json_config.h" BEFORE
// including nlohmann/json.hpp to ensure proper configuration.
// See Harmony/src/README_json_config.md for details.
//

#include <filesystem>
#include <memory>
#include <mutex>

namespace Harmony {

    class Configuration {
    public:
        Configuration();
        ~Configuration();

        Configuration(const Configuration& other);
        Configuration& operator=(const Configuration& other);

        void merge(const Configuration& configuration);
        void save(const std::filesystem::path& filePath);
        void load(const std::filesystem::path& filePath);
        void clear();
        void debugPrint() const;

        std::optional<Configuration> subsection(const std::vector<std::string>& keys) const;
        std::vector<std::string> extractKeys(const std::vector<std::string>& keys) const;
        std::optional<Configuration> operator[](const std::string& key);


        template<typename Type>
        inline std::optional<Type> get(const std::vector<std::string>& keys) const;
        template<typename Type>
        inline void set(const std::vector<std::string>& keys, const Type& value);
        template<typename Type>
        inline std::optional<Type> operator[](const std::string& key);

    private:
        struct Internal;
        mutable std::mutex m_mutex;
        std::unique_ptr<Internal> internal_;
    };

    Configuration operator+(const Configuration& left, const Configuration& right);
    Configuration& operator+=(Configuration& left, const Configuration& right);

} // namespace Harmony