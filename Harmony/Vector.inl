#include "Vector.h"
namespace Harmony::Utilities
{
	template<typename Type>
	inline Vector2<Type>::Vector2(Type X, Type Y) :
		x(X),
		y(Y)
	{
	}

	template <typename T>
	template <typename U>
	inline Vector2<T>::Vector2(const Vector2<U>& vector) :
		x(static_cast<T>(vector.x)),
		y(static_cast<T>(vector.y))
	{
	}

	template<typename Type>
	inline Vector2<Type> Vector2<Type>::operator+(const Vector2& other) const
	{
		return { x + other.x, y + other.y };
	}
	template<typename Type>
	inline Vector2<Type> Vector2<Type>::operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}
	template<typename Type>
	inline Vector2<Type> Vector2<Type>::operator*(Type scalar) const
	{
		return { x * scalar, y * scalar };
	}
	template<typename Type>
	inline Vector2<Type> Vector2<Type>::operator/(Type scalar) const
	{
		return { x / scalar, y / scalar };
	}
	template<typename Type>
	inline Vector2<Type>& Vector2<Type>::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	template<typename Type>
	inline Vector2<Type>& Vector2<Type>::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	template<typename Type>
	inline Vector2<Type>& Vector2<Type>::operator*=(Type scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	template<typename Type>
	inline Vector2<Type>& Vector2<Type>::operator/=(Type scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}
}