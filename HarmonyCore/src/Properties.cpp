#include "internal/json_config.h"
#include "Harmony/Properties.h"
#include "Harmony/Exceptions.h"
#include "Harmony/Assert.h"

#include <nlohmann/json.hpp>
#include <fmt/ranges.h>
#include <type_traits>
#include <fstream>

namespace Harmony {

    struct Properties::Internal {
        nlohmann::json data;
    };

    Properties::Properties()
        : internal_(std::make_unique<Internal>()) {
    }

    Properties::~Properties() = default;

    Properties::Properties(const Properties& other) {
        std::lock_guard lock(other.m_mutex);
        internal_ = std::make_unique<Internal>();
        internal_->data = other.internal_->data;
    }

    Properties& Properties::operator=(const Properties& other) {
        if (this != &other) {
            std::scoped_lock lock(m_mutex, other.m_mutex);
            if (!internal_) {
                internal_ = std::make_unique<Internal>();
            }
            internal_->data = other.internal_->data;
        }
        return *this;
    }

    void Properties::merge(const Properties& configuration) {
        std::scoped_lock lock(m_mutex, configuration.m_mutex);
        internal_->data.merge_patch(configuration.internal_->data);
    }

    void Properties::save(const std::filesystem::path& filePath) {
        std::lock_guard lock(m_mutex);

        HARMONY_ASSERT(!filePath.empty(), "Properties file path cannot be empty");
        HARMONY_ASSERT_NOT_NULL(internal_.get(), "Properties internal data is null");

        std::ofstream file(filePath);

		if (!file) HARMONY_ERROR("Failed to open properties file for writing: {}", filePath.string());

        file << internal_->data.dump(1);
        HARMONY_INFO("Properties saved to {}", filePath.string());
    }

    void Properties::load(const std::filesystem::path& filePath) {
        std::lock_guard lock(m_mutex);

        HARMONY_ASSERT(!filePath.empty(), "Properties file path cannot be empty");
        HARMONY_ASSERT_NOT_NULL(internal_.get(), "Properties internal data is null");

        std::ifstream file(filePath);

        if (!file) HARMONY_ERROR("Failed to open properties file for writing: {}", filePath.string());

        try {
            file >> internal_->data;
            HARMONY_INFO("Properties loaded from {}", filePath.string());
        }
        catch (const nlohmann::json::parse_error& e) {
			HARMONY_THROW("Failed to parse configuration file {}: {}", filePath.string(), e.what());
        }
    }

    void Properties::clear() {
        std::lock_guard lock(m_mutex);
        internal_->data.clear();
    }

    void Properties::debugPrint() const
    {
        std::lock_guard lock(m_mutex);
        std::string data = internal_->data.dump(2);
        HARMONY_INFO("Properties Data:\n{}", data);
    }

    // Helpers
    namespace {
        const nlohmann::json* findNode(const nlohmann::json& root, const std::vector<std::string>& keys) {
            const nlohmann::json* node = &root;
            for (const auto& key : keys) {
                if (!node->contains(key)) return nullptr;
                node = &(*node)[key];
            }
            return node;
        }

        nlohmann::json* findOrCreateNode(nlohmann::json& root, const std::vector<std::string>& keys) {
            nlohmann::json* node = &root;
            for (const auto& key : keys)
                node = &(*node)[key];
            return node;
        }
    }

    template<typename Type>
    std::optional<Type> Properties::get(const std::vector<std::string>& keys) const {
        std::lock_guard lock(m_mutex);
        const auto* node = findNode(internal_->data, keys);
        if (!node) {
            HARMONY_WARN("Properties::get - Key path not found: {}",
                fmt::format("[{}]", fmt::join(keys, ".")));
            return std::nullopt;
        }

        try {
            auto value = node->get<Type>();
            HARMONY_DEBUG("Properties::get - Retrieved {} = {}",
                fmt::format("[{}]", fmt::join(keys, ".")), value);
            return value;
        }
        catch (const nlohmann::json::type_error& e) {
            HARMONY_ERROR("Properties::get - Type mismatch at {}: {}",
                fmt::format("[{}]", fmt::join(keys, ".")), e.what());
            return std::nullopt;
        }
    }

    template<typename Type>
    void Properties::set(const std::vector<std::string>& keys, const Type& value) {
        std::lock_guard lock(m_mutex);
        auto* node = findOrCreateNode(internal_->data, keys);

        bool existed = !node->is_null();
        *node = value;

        auto formatValueToString = [&](const Type& val) -> std::string {
            if constexpr (std::is_same_v<Type, std::vector<bool>>) {
                std::string s = "[";
                for (size_t i = 0; i < val.size(); ++i) {
                    s += (val[i] ? "true" : "false");
                    if (i < val.size() - 1) s += ", ";
                }
                s += "]";
                return s;
            }
            else {
                return fmt::format("{}", val);
            }
            };

        std::string valueStr = formatValueToString(value);
        std::string keyStr = fmt::format("[{}]", fmt::join(keys, "."));

        if (existed) {
            HARMONY_INFO("Properties::set - Overriding {} with new value: {}", keyStr, valueStr);
        }
        else {
            HARMONY_INFO("Properties::set - Created {} = {}", keyStr, valueStr);
        }
    }

    template<typename Type>
    std::optional<Type> Properties::operator[](const std::string& key) {
        return Properties::get<Type>({ key });
    }

    std::optional<Properties> Properties::subsection(const std::vector<std::string>& keys) const {
        std::lock_guard lock(m_mutex);
        const auto* node = findNode(internal_->data, keys);
        if (node) {
            Properties configuration;
            configuration.internal_->data = *node;
            HARMONY_DEBUG("Properties::subsection - Extracted subsection {}", fmt::format("[{}]", fmt::join(keys, ".")));
            return configuration;
        }
        HARMONY_WARN("Properties::subsection - Subsection not found: {}", fmt::format("[{}]", fmt::join(keys, ".")));
        return std::nullopt;
    }

    std::vector<std::string> Properties::extractKeys(const std::vector<std::string>& keys) const {
        std::lock_guard lock(m_mutex);
        const auto* node = findNode(internal_->data, keys);
        if (!node) {
            HARMONY_WARN("Properties::extractKeys - Node not found: {}", fmt::format("[{}]", fmt::join(keys, ".")));
            return {};
        }
        if (!node->is_object()) {
            HARMONY_WARN("Properties::extractKeys - Node at {} is not an object", fmt::format("[{}]", fmt::join(keys, ".")));
            return {};
        }

        std::vector<std::string> rKeys;
        rKeys.reserve(node->size());
        for (const auto& [key, _] : node->items())
            rKeys.push_back(key);

        HARMONY_INFO("Properties::extractKeys - Extracted {} keys from {}",
            rKeys.size(), fmt::format("[{}]", fmt::join(keys, ".")));
        return rKeys;
    }

    std::optional<Properties> Properties::operator[](const std::string& key)
    {
        return Properties::subsection({ key });
    }

    Properties operator+(const Properties& left, const Properties& right)
    {
        Properties result = left;
        result += right;
        return result;
    }

    Properties& operator+=(Properties& left, const Properties& right)
    {
        left.merge(right);
        return left;
    }

    // =====================================================================================
    // 1. SCALARS - GET
    // =====================================================================================

    // Boolean & String
    template std::optional<bool>        Properties::get<bool>(const std::vector<std::string>&) const;
    template std::optional<std::string> Properties::get<std::string>(const std::vector<std::string>&) const;

    // Char Types (8-bit)
    template std::optional<char>           Properties::get<char>(const std::vector<std::string>&) const;
    template std::optional<signed char>    Properties::get<signed char>(const std::vector<std::string>&) const;   // int8_t
    template std::optional<unsigned char>  Properties::get<unsigned char>(const std::vector<std::string>&) const; // uint8_t

    // Short Types (16-bit)
    template std::optional<short>          Properties::get<short>(const std::vector<std::string>&) const;          // int16_t
    template std::optional<unsigned short> Properties::get<unsigned short>(const std::vector<std::string>&) const; // uint16_t

    // Integer Types (32-bit usually)
    template std::optional<int>            Properties::get<int>(const std::vector<std::string>&) const;            // int32_t
    template std::optional<unsigned int>   Properties::get<unsigned int>(const std::vector<std::string>&) const;   // uint32_t

    // Long Types (32-bit or 64-bit depending on OS)
    template std::optional<long>           Properties::get<long>(const std::vector<std::string>&) const;
    template std::optional<unsigned long>  Properties::get<unsigned long>(const std::vector<std::string>&) const;

    // Long Long Types (64-bit)
    template std::optional<long long>          Properties::get<long long>(const std::vector<std::string>&) const;          // int64_t
    template std::optional<unsigned long long> Properties::get<unsigned long long>(const std::vector<std::string>&) const; // uint64_t

    // Floating Point Types
    template std::optional<float>       Properties::get<float>(const std::vector<std::string>&) const;
    template std::optional<double>      Properties::get<double>(const std::vector<std::string>&) const;
    template std::optional<long double> Properties::get<long double>(const std::vector<std::string>&) const;


    // =====================================================================================
    // 2. SCALARS - SET
    // =====================================================================================

    // Boolean & String
    template void Properties::set<bool>(const std::vector<std::string>&, const bool&);
    template void Properties::set<std::string>(const std::vector<std::string>&, const std::string&);

    // Char Types
    template void Properties::set<char>(const std::vector<std::string>&, const char&);
    template void Properties::set<signed char>(const std::vector<std::string>&, const signed char&);
    template void Properties::set<unsigned char>(const std::vector<std::string>&, const unsigned char&);

    // Short Types
    template void Properties::set<short>(const std::vector<std::string>&, const short&);
    template void Properties::set<unsigned short>(const std::vector<std::string>&, const unsigned short&);

    // Integer Types
    template void Properties::set<int>(const std::vector<std::string>&, const int&);
    template void Properties::set<unsigned int>(const std::vector<std::string>&, const unsigned int&);

    // Long Types
    template void Properties::set<long>(const std::vector<std::string>&, const long&);
    template void Properties::set<unsigned long>(const std::vector<std::string>&, const unsigned long&);

    // Long Long Types
    template void Properties::set<long long>(const std::vector<std::string>&, const long long&);
    template void Properties::set<unsigned long long>(const std::vector<std::string>&, const unsigned long long&);

    // Floating Point Types
    template void Properties::set<float>(const std::vector<std::string>&, const float&);
    template void Properties::set<double>(const std::vector<std::string>&, const double&);
    template void Properties::set<long double>(const std::vector<std::string>&, const long double&);


    // =====================================================================================
    // 3. VECTORS - GET
    // =====================================================================================

    // Boolean & String
    template std::optional<std::vector<bool>>        Properties::get<std::vector<bool>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<std::string>> Properties::get<std::vector<std::string>>(const std::vector<std::string>&) const;

    // Char Types
    template std::optional<std::vector<char>>           Properties::get<std::vector<char>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<signed char>>    Properties::get<std::vector<signed char>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<unsigned char>>  Properties::get<std::vector<unsigned char>>(const std::vector<std::string>&) const;

    // Short Types
    template std::optional<std::vector<short>>          Properties::get<std::vector<short>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<unsigned short>> Properties::get<std::vector<unsigned short>>(const std::vector<std::string>&) const;

    // Integer Types
    template std::optional<std::vector<int>>            Properties::get<std::vector<int>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<unsigned int>>   Properties::get<std::vector<unsigned int>>(const std::vector<std::string>&) const;

    // Long Types
    template std::optional<std::vector<long>>           Properties::get<std::vector<long>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<unsigned long>>  Properties::get<std::vector<unsigned long>>(const std::vector<std::string>&) const;

    // Long Long Types
    template std::optional<std::vector<long long>>          Properties::get<std::vector<long long>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<unsigned long long>> Properties::get<std::vector<unsigned long long>>(const std::vector<std::string>&) const;

    // Floating Point Types
    template std::optional<std::vector<float>>       Properties::get<std::vector<float>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<double>>      Properties::get<std::vector<double>>(const std::vector<std::string>&) const;
    template std::optional<std::vector<long double>> Properties::get<std::vector<long double>>(const std::vector<std::string>&) const;


    // =====================================================================================
    // 4. VECTORS - SET
    // =====================================================================================

    // Boolean & String
    template void Properties::set<std::vector<bool>>(const std::vector<std::string>&, const std::vector<bool>&);
    template void Properties::set<std::vector<std::string>>(const std::vector<std::string>&, const std::vector<std::string>&);

    // Char Types
    template void Properties::set<std::vector<char>>(const std::vector<std::string>&, const std::vector<char>&);
    template void Properties::set<std::vector<signed char>>(const std::vector<std::string>&, const std::vector<signed char>&);
    template void Properties::set<std::vector<unsigned char>>(const std::vector<std::string>&, const std::vector<unsigned char>&);

    // Short Types
    template void Properties::set<std::vector<short>>(const std::vector<std::string>&, const std::vector<short>&);
    template void Properties::set<std::vector<unsigned short>>(const std::vector<std::string>&, const std::vector<unsigned short>&);

    // Integer Types
    template void Properties::set<std::vector<int>>(const std::vector<std::string>&, const std::vector<int>&);
    template void Properties::set<std::vector<unsigned int>>(const std::vector<std::string>&, const std::vector<unsigned int>&);

    // Long Types
    template void Properties::set<std::vector<long>>(const std::vector<std::string>&, const std::vector<long>&);
    template void Properties::set<std::vector<unsigned long>>(const std::vector<std::string>&, const std::vector<unsigned long>&);

    // Long Long Types
    template void Properties::set<std::vector<long long>>(const std::vector<std::string>&, const std::vector<long long>&);
    template void Properties::set<std::vector<unsigned long long>>(const std::vector<std::string>&, const std::vector<unsigned long long>&);

    // Floating Point Types
    template void Properties::set<std::vector<float>>(const std::vector<std::string>&, const std::vector<float>&);
    template void Properties::set<std::vector<double>>(const std::vector<std::string>&, const std::vector<double>&);
    template void Properties::set<std::vector<long double>>(const std::vector<std::string>&, const std::vector<long double>&);
}