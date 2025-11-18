#include "Vector.h"
namespace Harmony
{
	template<typename Type>
	inline Vector2<Type>::Vector2(Type X, Type Y) :
		x(X),
		y(Y)
	{
	}

	template <typename Type>
	template <typename U>
	inline Vector2<Type>::Vector2(const Vector2<U>& vector) :
		x(static_cast<Type>(vector.x)),
		y(static_cast<Type>(vector.y))
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

	inline bool operator==(const Vector2<float>& left, const Vector2<float>& right){
		return left.x == right.x && left.y == right.y;
	}

	inline bool operator!=(const Vector2<float>& left, const Vector2<float>& right){
		return !(left == right);
	}
}