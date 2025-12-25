#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace Harmony 
{
    // ========================================================
    // Properties Configuration Class
    // ========================================================

    class Properties 
    {
    public:
        Properties();
        ~Properties();

        Properties(const Properties& other);
        Properties& operator=(const Properties& other);

        void Merge(const Properties& configuration);
        void Save(const std::filesystem::path& filePath);
        void Load(const std::filesystem::path& filePath);
        void Clear();
        void DebugPrint() const;

        std::optional<Properties> Subsection(const std::vector<std::string>& keys) const;
        std::vector<std::string> ExtractKeys(const std::vector<std::string>& keys) const;
        Properties operator[](const std::string& key) const;

        template<typename Type>
        inline std::optional<Type> Get(const std::vector<std::string>& keys) const;
        
        template<typename Type>
        inline void Set(const std::vector<std::string>& keys, const Type& value);

    private:
        struct Internal;

        mutable std::mutex m_mutex;
        std::unique_ptr<Internal> internal_;
    };

    Properties operator+(const Properties& left, const Properties& right);
    Properties& operator+=(Properties& left, const Properties& right);

} // namespace Harmony