#pragma once

namespace Harmony::Utilities
{
	template<typename Type>
	struct Vector2
	{
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;

		Vector2 operator*(Type scalar) const;
		Vector2 operator/(Type scalar) const;

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);

		Vector2& operator*=(Type scalar);
		Vector2& operator/=(Type scalar);

		Type x{ 0 };
		Type y{ 0 };
	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector2d = Vector2<double>;
	using Vector2l = Vector2<long>;
}

#include "Vector.inl"