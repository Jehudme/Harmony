#include "Harmony/Properties.h"
#include "Harmony/Assert.h"
#include "Harmony/Logger.h"

#include <filesystem>
#include <fmt/ranges.h>
#include <fstream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <optional>
#include <type_traits>

namespace Harmony
{
// ========================================================
// Properties Internal Structure
// ========================================================

struct Properties::Internal
{
    nlohmann::json data;
};

// ========================================================
// Properties Constructors and Destructor
// ========================================================

Properties::Properties() : internal_(std::make_unique<Internal>()) {}

Properties::~Properties() = default;

Properties::Properties(const Properties& other)
{
    std::lock_guard lock(other.m_mutex);
    internal_ = std::make_unique<Internal>();
    internal_->data = other.internal_->data;
}

Properties& Properties::operator=(const Properties& other)
{
    if (this != &other)
    {
        std::scoped_lock lock(m_mutex, other.m_mutex);

        if (!internal_)
        {
            internal_ = std::make_unique<Internal>();
        }
        internal_->data = other.internal_->data;
    }
    return *this;
}

// ========================================================
// Properties Public Methods
// ========================================================

void Properties::Merge(const Properties& configuration)
{
    std::scoped_lock lock(m_mutex, configuration.m_mutex);
    internal_->data.merge_patch(configuration.internal_->data);
}

void Properties::Save(const std::filesystem::path& filePath)
{
    std::lock_guard lock(m_mutex);

    HARMONY_ASSERT(!filePath.empty(), "Properties file path cannot be empty");
    HARMONY_ASSERT_NOT_NULL(internal_.get(), "Properties internal data is null");

    std::ofstream file(filePath);

    if (!file)
    {
        HARMONY_ERROR("Failed to open properties file for writing: {}", filePath.string());
        return;
    }

    file << internal_->data.dump(1);
    HARMONY_INFO("Properties saved to {}", filePath.string());
}

void Properties::Load(const std::filesystem::path& filePath)
{
    std::lock_guard lock(m_mutex);

    HARMONY_ASSERT(!filePath.empty(), "Properties file path cannot be empty");
    HARMONY_ASSERT_NOT_NULL(internal_.get(), "Properties internal data is null");

    std::ifstream file(filePath);

    if (!file)
    {
        HARMONY_ERROR("Failed to open properties file for reading: {}", filePath.string());
        return;
    }

    internal_->data = nlohmann::json::parse(file, nullptr, false);

    if (internal_->data.is_discarded())
    {
        HARMONY_ERROR("Failed to parse configuration file: {}", filePath.string());
        internal_->data = nlohmann::json::object();
        return;
    }

    HARMONY_INFO("Properties loaded from {}", filePath.string());
}

void Properties::Clear()
{
    std::lock_guard lock(m_mutex);
    internal_->data.clear();
}

void Properties::DebugPrint() const
{
    std::lock_guard lock(m_mutex);
    std::string data = internal_->data.dump(2);
    HARMONY_INFO("Properties Data:\n{}", data);
}

// ========================================================
// Properties Helper Functions
// ========================================================

namespace
{
const nlohmann::json* FindNode(const nlohmann::json& root, const std::vector<std::string>& keys)
{
    const nlohmann::json* node = &root;
    for (const auto& key : keys)
    {
        if (!node->contains(key))
        {
            return nullptr;
        }
        node = &(*node)[key];
    }
    return node;
}

nlohmann::json* FindOrCreateNode(nlohmann::json& root, const std::vector<std::string>& keys)
{
    nlohmann::json* node = &root;
    for (const auto& key : keys)
    {
        node = &(*node)[key];
    }
    return node;
}
} // namespace

// ========================================================
// Properties Template Implementations
// ========================================================

template <typename Type>
std::optional<Type> Properties::Get(const std::vector<std::string>& keys) const
{
    std::lock_guard lock(m_mutex);
    const auto* node = FindNode(internal_->data, keys);

    if (!node)
    {
        HARMONY_WARN("Properties::Get - Key path not found: {}",
                     fmt::format("[{}]", fmt::join(keys, ".")));
        return std::nullopt;
    }

    Type value = node->template get<Type>();
    HARMONY_DEBUG("Properties::Get - Retrieved {} = {}", fmt::format("[{}]", fmt::join(keys, ".")),
                  value);
    return value;
}

template <typename Type>
void Properties::Set(const std::vector<std::string>& keys, const Type& value)
{
    std::lock_guard lock(m_mutex);
    auto* node = FindOrCreateNode(internal_->data, keys);

    bool existed = !node->is_null();
    *node = value;

    auto formatValueToString = [&](const Type& val) -> std::string
    {
        if constexpr (std::is_same_v<Type, std::vector<bool>>)
        {
            std::string stringValue = "[";
            for (size_t index = 0; index < val.size(); ++index)
            {
                stringValue += (val[index] ? "true" : "false");
                if (index < val.size() - 1)
                {
                    stringValue += ", ";
                }
            }
            stringValue += "]";
            return stringValue;
        }
        else
        {
            return fmt::format("{}", val);
        }
    };

    std::string valueString = formatValueToString(value);
    std::string keyString = fmt::format("[{}]", fmt::join(keys, "."));

    if (existed)
    {
        HARMONY_INFO("Properties::Set - Overriding {} with new value: {}", keyString, valueString);
    }
    else
    {
        HARMONY_INFO("Properties::Set - Created {} = {}", keyString, valueString);
    }
}

std::optional<Properties> Properties::Subsection(const std::vector<std::string>& keys) const
{
    std::lock_guard lock(m_mutex);
    const auto* node = FindNode(internal_->data, keys);

    if (node)
    {
        Properties configuration;
        configuration.internal_->data = *node;
        HARMONY_DEBUG("Properties::Subsection - Extracted subsection {}",
                      fmt::format("[{}]", fmt::join(keys, ".")));
        return configuration;
    }

    HARMONY_WARN("Properties::Subsection - Subsection not found: {}",
                 fmt::format("[{}]", fmt::join(keys, ".")));
    return std::nullopt;
}

std::vector<std::string> Properties::ExtractKeys(const std::vector<std::string>& keys) const
{
    std::lock_guard lock(m_mutex);
    const auto* node = FindNode(internal_->data, keys);

    if (!node)
    {
        HARMONY_WARN("Properties::ExtractKeys - Node not found: {}",
                     fmt::format("[{}]", fmt::join(keys, ".")));
        return {};
    }

    if (!node->is_object())
    {
        HARMONY_WARN("Properties::ExtractKeys - Node at {} is not an object",
                     fmt::format("[{}]", fmt::join(keys, ".")));
        return {};
    }

    std::vector<std::string> extractedKeys;
    extractedKeys.reserve(node->size());

    for (const auto& [key, _] : node->items())
    {
        extractedKeys.push_back(key);
    }

    HARMONY_INFO("Properties::ExtractKeys - Extracted {} keys from {}", extractedKeys.size(),
                 fmt::format("[{}]", fmt::join(keys, ".")));
    return extractedKeys;
}

Properties Properties::operator[](const std::string& key) const
{
    if (auto subsectionOptional = Subsection({key}); subsectionOptional.has_value())
    {
        return subsectionOptional.value();
    }

    HARMONY_WARN("Properties::operator[] - Subsection '{}' not found, returning empty Properties",
                 key);
    return Properties();
}

// ========================================================
// Properties Global Operators
// ========================================================

Properties operator+(const Properties& left, const Properties& right)
{
    Properties result = left;
    result += right;
    return result;
}

Properties& operator+=(Properties& left, const Properties& right)
{
    left.Merge(right);
    return left;
}

// ========================================================
// Template Instantiations - Scalar Types (Get)
// ========================================================

template std::optional<bool> Properties::Get<bool>(const std::vector<std::string>&) const;
template std::optional<std::string>
Properties::Get<std::string>(const std::vector<std::string>&) const;

template std::optional<char> Properties::Get<char>(const std::vector<std::string>&) const;
template std::optional<signed char>
Properties::Get<signed char>(const std::vector<std::string>&) const;
template std::optional<unsigned char>
Properties::Get<unsigned char>(const std::vector<std::string>&) const;

template std::optional<short> Properties::Get<short>(const std::vector<std::string>&) const;
template std::optional<unsigned short>
Properties::Get<unsigned short>(const std::vector<std::string>&) const;

template std::optional<int> Properties::Get<int>(const std::vector<std::string>&) const;
template std::optional<unsigned int>
Properties::Get<unsigned int>(const std::vector<std::string>&) const;

template std::optional<long> Properties::Get<long>(const std::vector<std::string>&) const;
template std::optional<unsigned long>
Properties::Get<unsigned long>(const std::vector<std::string>&) const;

template std::optional<long long> Properties::Get<long long>(const std::vector<std::string>&) const;
template std::optional<unsigned long long>
Properties::Get<unsigned long long>(const std::vector<std::string>&) const;

template std::optional<float> Properties::Get<float>(const std::vector<std::string>&) const;
template std::optional<double> Properties::Get<double>(const std::vector<std::string>&) const;
template std::optional<long double>
Properties::Get<long double>(const std::vector<std::string>&) const;

// ========================================================
// Template Instantiations - Scalar Types (Set)
// ========================================================

template void Properties::Set<bool>(const std::vector<std::string>&, const bool&);
template void Properties::Set<std::string>(const std::vector<std::string>&, const std::string&);

template void Properties::Set<char>(const std::vector<std::string>&, const char&);
template void Properties::Set<signed char>(const std::vector<std::string>&, const signed char&);
template void Properties::Set<unsigned char>(const std::vector<std::string>&, const unsigned char&);

template void Properties::Set<short>(const std::vector<std::string>&, const short&);
template void Properties::Set<unsigned short>(const std::vector<std::string>&,
                                              const unsigned short&);

template void Properties::Set<int>(const std::vector<std::string>&, const int&);
template void Properties::Set<unsigned int>(const std::vector<std::string>&, const unsigned int&);

template void Properties::Set<long>(const std::vector<std::string>&, const long&);
template void Properties::Set<unsigned long>(const std::vector<std::string>&, const unsigned long&);

template void Properties::Set<long long>(const std::vector<std::string>&, const long long&);
template void Properties::Set<unsigned long long>(const std::vector<std::string>&,
                                                  const unsigned long long&);

template void Properties::Set<float>(const std::vector<std::string>&, const float&);
template void Properties::Set<double>(const std::vector<std::string>&, const double&);
template void Properties::Set<long double>(const std::vector<std::string>&, const long double&);

// ========================================================
// Template Instantiations - Vector Types (Get)
// ========================================================

template std::optional<std::vector<bool>>
Properties::Get<std::vector<bool>>(const std::vector<std::string>&) const;
template std::optional<std::vector<std::string>>
Properties::Get<std::vector<std::string>>(const std::vector<std::string>&) const;

template std::optional<std::vector<char>>
Properties::Get<std::vector<char>>(const std::vector<std::string>&) const;
template std::optional<std::vector<signed char>>
Properties::Get<std::vector<signed char>>(const std::vector<std::string>&) const;
template std::optional<std::vector<unsigned char>>
Properties::Get<std::vector<unsigned char>>(const std::vector<std::string>&) const;

template std::optional<std::vector<short>>
Properties::Get<std::vector<short>>(const std::vector<std::string>&) const;
template std::optional<std::vector<unsigned short>>
Properties::Get<std::vector<unsigned short>>(const std::vector<std::string>&) const;

template std::optional<std::vector<int>>
Properties::Get<std::vector<int>>(const std::vector<std::string>&) const;
template std::optional<std::vector<unsigned int>>
Properties::Get<std::vector<unsigned int>>(const std::vector<std::string>&) const;

template std::optional<std::vector<long>>
Properties::Get<std::vector<long>>(const std::vector<std::string>&) const;
template std::optional<std::vector<unsigned long>>
Properties::Get<std::vector<unsigned long>>(const std::vector<std::string>&) const;

template std::optional<std::vector<long long>>
Properties::Get<std::vector<long long>>(const std::vector<std::string>&) const;
template std::optional<std::vector<unsigned long long>>
Properties::Get<std::vector<unsigned long long>>(const std::vector<std::string>&) const;

template std::optional<std::vector<float>>
Properties::Get<std::vector<float>>(const std::vector<std::string>&) const;
template std::optional<std::vector<double>>
Properties::Get<std::vector<double>>(const std::vector<std::string>&) const;
template std::optional<std::vector<long double>>
Properties::Get<std::vector<long double>>(const std::vector<std::string>&) const;

// ========================================================
// Template Instantiations - Vector Types (Set)
// ========================================================

template void Properties::Set<std::vector<bool>>(const std::vector<std::string>&,
                                                 const std::vector<bool>&);
template void Properties::Set<std::vector<std::string>>(const std::vector<std::string>&,
                                                        const std::vector<std::string>&);

template void Properties::Set<std::vector<char>>(const std::vector<std::string>&,
                                                 const std::vector<char>&);
template void Properties::Set<std::vector<signed char>>(const std::vector<std::string>&,
                                                        const std::vector<signed char>&);
template void Properties::Set<std::vector<unsigned char>>(const std::vector<std::string>&,
                                                          const std::vector<unsigned char>&);

template void Properties::Set<std::vector<short>>(const std::vector<std::string>&,
                                                  const std::vector<short>&);
template void Properties::Set<std::vector<unsigned short>>(const std::vector<std::string>&,
                                                           const std::vector<unsigned short>&);

template void Properties::Set<std::vector<int>>(const std::vector<std::string>&,
                                                const std::vector<int>&);
template void Properties::Set<std::vector<unsigned int>>(const std::vector<std::string>&,
                                                         const std::vector<unsigned int>&);

template void Properties::Set<std::vector<long>>(const std::vector<std::string>&,
                                                 const std::vector<long>&);
template void Properties::Set<std::vector<unsigned long>>(const std::vector<std::string>&,
                                                          const std::vector<unsigned long>&);

template void Properties::Set<std::vector<long long>>(const std::vector<std::string>&,
                                                      const std::vector<long long>&);
template void
Properties::Set<std::vector<unsigned long long>>(const std::vector<std::string>&,
                                                 const std::vector<unsigned long long>&);

template void Properties::Set<std::vector<float>>(const std::vector<std::string>&,
                                                  const std::vector<float>&);
template void Properties::Set<std::vector<double>>(const std::vector<std::string>&,
                                                   const std::vector<double>&);
template void Properties::Set<std::vector<long double>>(const std::vector<std::string>&,
                                                        const std::vector<long double>&);

} // namespace Harmony