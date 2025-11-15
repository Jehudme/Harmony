#pragma once

#include <optional>
#include <functional>
#include <stdexcept>

namespace Harmony::Utilities {

    template<typename Type>
    class OptionalReference : public std::optional<std::reference_wrapper<Type>>
    {
        using Base = std::optional<std::reference_wrapper<Type>>;

    public:
        using Base::Base;

        inline Type& get() {
            if (!this->has_value()) {
                throw std::runtime_error("Attempted to access value of empty OptionalReference");
            }
            return this->value().get();
        }

        inline const Type& get() const {
            if (!this->has_value()) {
                throw std::runtime_error("Attempted to access value of empty OptionalReference");
            }
            return this->value().get();
        }

        inline Type* operator->() {
            return &get();
        }

        inline const Type* operator->() const {
            return &get();
        }
    };

}