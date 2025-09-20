#pragma once
#include <variant>
#include <string>

namespace Harmony::Utilities
{
    template<typename... Types>
    class Value
    {
    public:
        using VariantType = std::variant<Types...>;

        Value(const VariantType& data) : data_(data) {}
        Value(VariantType&& data) : data_(std::move(data)) {}

        template<typename Type>
        bool is() const {
            return std::holds_alternative<Type>(data_);
        }

        template<typename Type>
        Type as() const {
            return std::get<Type>(data_);
        }

    private:
        VariantType data_;
    };
}