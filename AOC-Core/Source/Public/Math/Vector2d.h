#pragma once

#include "pch.h"

#include "Types.h"
#include "Hashing.h"

namespace Core::Math
{
    template<typename T>
    concept Arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template<Arithmetic T>
    struct Vector2d
    {
        T x{ };
        T y{ };

        Vector2d() : x{ }, y{ } { }
        explicit Vector2d(T inX, T inY) : x{ inX }, y{ inY } { }

        static const Vector2d<T> zero;
        static const Vector2d<T> up;
        static const Vector2d<T> down;
        static const Vector2d<T> left;
        static const Vector2d<T> right;

        bool operator==(const Vector2d<T>&) const = default;
        auto operator<=>(const Vector2d<T>&) const = default;

        template<Arithmetic U> friend std::ostream& operator<<(std::ostream& output, const Vector2d<T>& v);

        // Addition
        template<Arithmetic U> friend Vector2d<T> operator+(const Vector2d<T>& a, const Vector2d<T>& b);
        Vector2d<T>& operator+=(const Vector2d<T>& other);

        // Subtraction
        template<Arithmetic U> friend Vector2d<T> operator-(const Vector2d<T>& a, const Vector2d<T>& b);
        Vector2d<T>& operator-=(const Vector2d<T>& other);

        // Multiplication
        template<Arithmetic U> friend Vector2d<T> operator*(const Vector2d<T>& a, const Vector2d<T>& b);
        template<Arithmetic U> friend Vector2d<T> operator*(const Vector2d<T>& v, T s);
        template<Arithmetic U> friend Vector2d<T> operator*(T s, const Vector2d<T>& v);
        Vector2d<T>& operator*=(T s);

        // Division
        template<Arithmetic U> friend Vector2d<T> operator/(const Vector2d<T>& a, const Vector2d<T>& b);
        template<Arithmetic U> friend Vector2d<T> operator/(const Vector2d<T>& v, T s);
        template<Arithmetic U> friend Vector2d<T> operator/(T s, const Vector2d<T>& v);
        Vector2d<T>& operator/=(T s);

        T LengthSquared() const;
        T Length() const;
        Vector2d<T>& Normalize();
    };

    template<Arithmetic T>
    /*static*/ const Vector2d<T> Vector2d<T>::zero{ 0, 0 };

    template<Arithmetic T>
    /*static*/ const Vector2d<T> Vector2d<T>::up{ 0, 1 };

    template<Arithmetic T>
    /*static*/ const Vector2d<T> Vector2d<T>::down{ 0, -1 };

    template<Arithmetic T>
    /*static*/ const Vector2d<T> Vector2d<T>::left{ -1, 0 };

    template<Arithmetic T>
    /*static*/ const Vector2d<T> Vector2d<T>::right{ 1, 0 };

    template<Arithmetic T>
    std::ostream& operator<<(std::ostream& output, const Vector2d<T>& v)
    {
        output << "(" << v.x << "," << v.y << ")" << std::endl;
        return output;
    }

    template<Arithmetic T>
    Vector2d<T> operator+(const Vector2d<T>& a, const Vector2d<T>& b) { return { a.x + b.x, a.y + b.y }; }

    template<Arithmetic T>
    Vector2d<T>& Vector2d<T>::operator+=(const Vector2d<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<Arithmetic T>
    Vector2d<T> operator-(const Vector2d<T>& a, const Vector2d<T>& b) { return { a.x - b.x, a.y - b.y }; }

    template<Arithmetic T>
    Vector2d<T>& Vector2d<T>::operator-=(const Vector2d<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template<Arithmetic T>
    Vector2d<T> operator*(const Vector2d<T>& a, const Vector2d<T>& b) { return { a.x * b.x, a.y * b.y }; }

    template<Arithmetic T>
    Vector2d<T> operator*(const Vector2d<T>& v, T s) { return Vector2d<T>{ v.x * s, v.y * s }; }

    template<Arithmetic T>
    Vector2d<T> operator*(T s, const Vector2d<T>& v) { return Vector2d<T>{ v.x * s, v.y * s }; }

    template<Arithmetic T>
    Vector2d<T>& Vector2d<T>::operator*=(T s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    template<Arithmetic T>
    Vector2d<T> operator/(const Vector2d<T>& a, const Vector2d<T>& b) { return { a.x / b.x, a.y / b.y }; }

    template<Arithmetic T>
    Vector2d<T> operator/(const Vector2d<T>& v, T s) { return Vector2d<T>{ v.x / s, v.y / s }; }

    template<Arithmetic T>
    Vector2d<T> operator/(T s, const Vector2d<T>& v) { return Vector2d<T>{ v.x / s, v.y / s }; }

    template<Arithmetic T>
    Vector2d<T>& Vector2d<T>::operator/=(T s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    template<Arithmetic T>
    T Vector2d<T>::LengthSquared() const
    {
        return x * x + y * y;
    }

    template<Arithmetic T>
    T Vector2d<T>::Length() const
    {
        return static_cast<T>(std::sqrt(LengthSquared()));
    }

    template<Arithmetic T>
    Vector2d<T>& Vector2d<T>::Normalize()
    {
        const auto length{ Length() };
        this->operator/=(length);
        return *this;
    }
}

#define DECL_VECTOR2D(name, type) \
    using name = Core::Math::Vector2d<type>; \
    IMPL_HASH(name, value.x, value.y); \

DECL_VECTOR2D(Vector2d32, int32);
DECL_VECTOR2D(Vector2d64, int64);
DECL_VECTOR2D(Vector2df, float);
DECL_VECTOR2D(Vector2dd, double);
