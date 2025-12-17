#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace Harmony {

    class Properties {
    public:
        Properties();
        ~Properties();

        Properties(const Properties& other);
        Properties& operator=(const Properties& other);

        void merge(const Properties& configuration);
        void save(const std::filesystem::path& filePath);
        void load(const std::filesystem::path& filePath);
        void clear();
        void debugPrint() const;

        std::optional<Properties> subsection(const std::vector<std::string>& keys) const;
        std::vector<std::string> extractKeys(const std::vector<std::string>& keys) const;
        std::optional<Properties> operator[](const std::string& key);

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

    Properties operator+(const Properties& left, const Properties& right);
    Properties& operator+=(Properties& left, const Properties& right);

} // namespace Harmony