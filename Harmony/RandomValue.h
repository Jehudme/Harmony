#pragma once

#include <iostream>
#include <random>
#include <type_traits>
#include <limits>
#include "Assert.h"

namespace Harmony::Utilities
{
    template<typename Type>
    Type random_value(Type min, Type max) {
        static_assert(std::is_integral<Type>::value, "Type must be an integral type");

        HARMONY_ASSERT(min <= max, "min must be less than or equal to max in random_value");

        static std::random_device rd;
        thread_local std::mt19937 gen(rd());

        thread_local std::uniform_int_distribution<Type> distrib;
        distrib.param(typename std::uniform_int_distribution<Type>::param_type{ min, max });
        return distrib(gen);
    }

    // Overload: no bounds provided � use full range
    template<typename Type>
    Type random_value() {
        return random_value<Type>(
            std::numeric_limits<Type>::min(),
            std::numeric_limits<Type>::max()
        );
    }

    // Overload: only min provided � max is max of type
    template<typename Type>
    Type random_value(Type min) {
        return random_value<Type>(min, std::numeric_limits<Type>::max());
    }
}