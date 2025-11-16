#pragma once

#include <optional>
#include <functional>
#include <stdexcept>
#include <string>

namespace Harmony::Utilities {

    template<typename Type>
    class OptionalReference : public std::optional<std::reference_wrapper<Type>>
    {
    public:
        using Base = std::optional<std::reference_wrapper<Type>>;
        using Base::Base;

        Type& get();
        const Type& get() const;

        Type* operator->();
        const Type* operator->() const;
    };

    bool operator==(const OptionalReference<std::string>& left,
        const OptionalReference<std::string>& right);

    bool operator!=(const OptionalReference<std::string>& left,
        const OptionalReference<std::string>& right);
}

#include "OptionalReference.inl"