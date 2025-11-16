namespace Harmony::Utilities
{
	template<typename Type>
	Vector2<Type> Vector2<Type>::operator+(const Vector2& other) const
	{
		return { x + other.x, y + other.y };
	}
	template<typename Type>
	Vector2<Type> Vector2<Type>::operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}
	template<typename Type>
	Vector2<Type> Vector2<Type>::operator*(Type scalar) const
	{
		return { x * scalar, y * scalar };
	}
	template<typename Type>
	Vector2<Type> Vector2<Type>::operator/(Type scalar) const
	{
		return { x / scalar, y / scalar };
	}
	template<typename Type>
	Vector2<Type>& Vector2<Type>::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	template<typename Type>
	Vector2<Type>& Vector2<Type>::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	template<typename Type>
	Vector2<Type>& Vector2<Type>::operator*=(Type scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	template<typename Type>
	Vector2<Type>& Vector2<Type>::operator/=(Type scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}
}