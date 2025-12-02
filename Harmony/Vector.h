#pragma once

namespace Harmony
{
    template<typename Type>
    struct Vector2
    {
        inline Vector2(Type X = 0, Type Y = 0);

        template<typename U>
        inline Vector2(const Vector2<U>& vector);

        inline Vector2 operator+(const Vector2& other) const;
        inline Vector2 operator-(const Vector2& other) const;

        inline Vector2 operator*(Type scalar) const;
        inline Vector2 operator/(Type scalar) const;

        inline Vector2& operator+=(const Vector2& other);
        inline Vector2& operator-=(const Vector2& other);

        inline Vector2& operator*=(Type scalar);
        inline Vector2& operator/=(Type scalar);

        Type x;
        Type y;
    };

    // Equality operators should also be templated on Type
    template<typename Type>
    inline bool operator==(const Vector2<Type>& left, const Vector2<Type>& right);

    template<typename Type>
    inline bool operator!=(const Vector2<Type>& left, const Vector2<Type>& right);

    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int>;
    using Vector2d = Vector2<double>;
    using Vector2l = Vector2<long>;
}

#include "Vector.inl"