#include "Exceptions.h"

namespace Harmony::Utilities {

    template<typename Type>
    Type& OptionalReference<Type>::get() {
        if (!this->has_value()) {
            throw Exceptions::NullPointerException("OptionalReference is empty");
        }
        return this->value().get();
    }

    template<typename Type>
    const Type& OptionalReference<Type>::get() const {
        if (!this->has_value()) {
            throw Exceptions::NullPointerException("OptionalReference is empty");
        }
        return this->value().get();
    }

    template<typename Type>
    Type* OptionalReference<Type>::operator->() {
        return &get();
    }

    template<typename Type>
    const Type* OptionalReference<Type>::operator->() const {
        return &get();
    }

    inline bool operator==(const OptionalReference<std::string>& left,
        const OptionalReference<std::string>& right) {
        if (!left && !right) return true;
        if (!left || !right) return false;
        return left.get() == right.get();
    }

    inline bool operator!=(const OptionalReference<std::string>& left,
        const OptionalReference<std::string>& right) {
        return !(left == right);
    }

}