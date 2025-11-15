#pragma once
#include <string_view>

namespace Harmony::Utilities {

    template <typename T>
    constexpr std::string_view typeName() {
#if defined(__clang__) || defined(__GNUC__)
        return __PRETTY_FUNCTION__; // contains "T = int" etc.
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
        return "Unknow: Unsupported compiler";
#endif
    }

}