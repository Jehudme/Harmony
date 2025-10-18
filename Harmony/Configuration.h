#pragma once

namespace Harmony::Errors {
    class ConfigurationError : public std::runtime_error {
    public:
        explicit ConfigurationError(const std::string& msg);
    };

}

namespace Harmony::Utilities 
{
    class Configuration 
    {
    public:
        Configuration();
		~Configuration();

        Configuration(const Configuration& other);
        Configuration& operator=(const Configuration& other);

        void merge(const Configuration& configuration);
        void save(const std::filesystem::path& filePath);
        void load(const std::filesystem::path& filePath);

        std::optional<Configuration> subsection(const std::vector<std::string>& keys) const;
        std::vector<std::string> extractKeys(const std::vector<std::string>& keys) const;

        template<typename Type>
        std::optional<Type> get(const std::vector<std::string>& keys) const;
        template<typename Type>
        void set(const std::vector<std::string>& keys, const Type& value);

    private:
        struct Internal;
        mutable std::mutex mutex_;
        std::unique_ptr<Internal> internal_;
    };

} // namespace Harmony
