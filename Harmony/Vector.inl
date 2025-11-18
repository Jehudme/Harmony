#include "Vector.h"
#include "Assert.h"

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
		HARMONY_ASSERT_WARN(scalar != static_cast<Type>(0), "Division by zero in Vector2");
		if (scalar == static_cast<Type>(0)) {
			return { static_cast<Type>(0), static_cast<Type>(0) };
		}
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
		HARMONY_ASSERT_WARN(scalar != static_cast<Type>(0), "Division by zero in Vector2");
		if (scalar == static_cast<Type>(0)) {
			x = y = static_cast<Type>(0);
			return *this;
		}
		x /= scalar;
		y /= scalar;
		return *this;
	}

	bool operator==(const Vector2<float>& left, const Vector2<float>& right){
		return left.x == right.x && left.y == right.y;
	}

	bool operator!=(const Vector2<float>& left, const Vector2<float>& right){
		return !(left == right);
	}
}