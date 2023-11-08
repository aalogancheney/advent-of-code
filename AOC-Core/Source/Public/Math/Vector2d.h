#pragma once

#include "pch.h"

#include "Types.h"

namespace Core::Math
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	template<Arithmetic T>
	struct Vector2D
	{
		T x{ };
		T y{ };

		static const Vector2D<T> zero;
		static const Vector2D<T> up;
		static const Vector2D<T> down;
		static const Vector2D<T> left;
		static const Vector2D<T> right;

		bool operator==(const Vector2D<T>&) const = default;
		auto operator<=>(const Vector2D<T>&) const = default;

		template<Arithmetic U> friend std::ostream& operator<<(std::ostream& output, const Vector2D<T>& v);

		// Addition
		template<Arithmetic U> friend Vector2D<T> operator+(const Vector2D<T>& a, const Vector2D<T>& b);
		Vector2D<T>& operator+=(const Vector2D<T>& other);

		// Subtraction
		template<Arithmetic U> friend Vector2D<T> operator-(const Vector2D<T>& a, const Vector2D<T>& b);
		Vector2D<T>& operator-=(const Vector2D<T>& other);

		// Multiplication
		template<Arithmetic U> friend Vector2D<T> operator*(const Vector2D<T>& a, const Vector2D<T>& b);
		template<Arithmetic U> friend Vector2D<T> operator*(const Vector2D<T>& v, T s);
		template<Arithmetic U> friend Vector2D<T> operator*(T s, const Vector2D<T>& v);
		Vector2D<T>& operator*=(T s);

		// Division
		template<Arithmetic U> friend Vector2D<T> operator/(const Vector2D<T>& a, const Vector2D<T>& b);
		template<Arithmetic U> friend Vector2D<T> operator/(const Vector2D<T>& v, T s);
		template<Arithmetic U> friend Vector2D<T> operator/(T s, const Vector2D<T>& v);
		Vector2D<T>& operator/=(T s);

		T LengthSquared() const;
		T Length() const;
		Vector2D<T>& Normalize();
	};

	template<Arithmetic T>
	/*static*/ const Vector2D<T> Vector2D<T>::zero{ 0, 0 };

	template<Arithmetic T>
	/*static*/ const Vector2D<T> Vector2D<T>::up{ 0, 1 };

	template<Arithmetic T>
	/*static*/ const Vector2D<T> Vector2D<T>::down{ 0, -1 };

	template<Arithmetic T>
	/*static*/ const Vector2D<T> Vector2D<T>::left{ -1, 0 };

	template<Arithmetic T>
	/*static*/ const Vector2D<T> Vector2D<T>::right{ 1, 0 };

	template<Arithmetic T>
	std::ostream& operator<<(std::ostream& output, const Vector2D<T>& v)
	{
		output << "(" << v.x << "," << v.y << ")" << std::endl;
		return output;
	}

	template<Arithmetic T>
	Vector2D<T> operator+(const Vector2D<T>& a, const Vector2D<T>& b) { return { a.x + b.x, a.y + b.y }; }

	template<Arithmetic T>
	Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template<Arithmetic T>
	Vector2D<T> operator-(const Vector2D<T>& a, const Vector2D<T>& b) { return { a.x - b.x, a.y - b.y }; }

	template<Arithmetic T>
	Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<Arithmetic T>
	Vector2D<T> operator*(const Vector2D<T>& a, const Vector2D<T>& b) { return { a.x * b.x, a.y * b.y }; }

	template<Arithmetic T>
	Vector2D<T> operator*(const Vector2D<T>& v, T s) { return Vector2D<T>{ v.x * s, v.y * s }; }

	template<Arithmetic T>
	Vector2D<T> operator*(T s, const Vector2D<T>& v) { return Vector2D<T>{ v.x * s, v.y * s }; }

	template<Arithmetic T>
	Vector2D<T>& Vector2D<T>::operator*=(T s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	template<Arithmetic T>
	Vector2D<T> operator/(const Vector2D<T>& a, const Vector2D<T>& b) { return { a.x / b.x, a.y / b.y }; }

	template<Arithmetic T>
	Vector2D<T> operator/(const Vector2D<T>& v, T s) { return Vector2D<T>{ v.x / s, v.y / s }; }

	template<Arithmetic T>
	Vector2D<T> operator/(T s, const Vector2D<T>& v) { return Vector2D<T>{ v.x / s, v.y / s }; }

	template<Arithmetic T>
	Vector2D<T>& Vector2D<T>::operator/=(T s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	template<Arithmetic T>
	T Vector2D<T>::LengthSquared() const
	{
		return x * x + y * y;
	}

	template<Arithmetic T>
	T Vector2D<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSquared()));
	}

	template<Arithmetic T>
	Vector2D<T>& Vector2D<T>::Normalize()
	{
		const auto length{ Length() };
		this->operator/=(length);
		return *this;
	}

	using IVector2D = Core::Math::Vector2D<int32>;
	using FVector2D = Core::Math::Vector2D<float>;
}
