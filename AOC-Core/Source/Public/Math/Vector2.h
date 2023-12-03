#pragma once

#include "pch.h"

#include "Types.h"
#include "Hashing.h"

namespace Core::Math
{
    template<typename T>
    concept Arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template<Arithmetic T>
    struct Vector2
    {
        T x{ };
        T y{ };

        Vector2() : x{ }, y{ } { }
        Vector2(T inX, T inY) : x{ inX }, y{ inY } { }

        static const Vector2<T> zero;
        static const Vector2<T> up;
        static const Vector2<T> down;
        static const Vector2<T> left;
        static const Vector2<T> right;

        bool operator==(const Vector2<T>&) const = default;
        auto operator<=>(const Vector2<T>&) const = default;

        template<Arithmetic U> friend std::ostream& operator<<(std::ostream& output, const Vector2<T>& v);

        // Addition
        template<Arithmetic U> friend Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b);
        Vector2<T>& operator+=(const Vector2<T>& other);

        // Subtraction
        template<Arithmetic U> friend Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b);
        Vector2<T>& operator-=(const Vector2<T>& other);

        // Multiplication
        template<Arithmetic U> friend Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b);
        template<Arithmetic U> friend Vector2<T> operator*(const Vector2<T>& v, T s);
        template<Arithmetic U> friend Vector2<T> operator*(T s, const Vector2<T>& v);
        Vector2<T>& operator*=(T s);

        // Division
        template<Arithmetic U> friend Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b);
        template<Arithmetic U> friend Vector2<T> operator/(const Vector2<T>& v, T s);
        template<Arithmetic U> friend Vector2<T> operator/(T s, const Vector2<T>& v);
        Vector2<T>& operator/=(T s);

        T LengthSquared() const;
        T Length() const;
        Vector2<T>& Normalize();
    };

    template<Arithmetic T>
    /*static*/ const Vector2<T> Vector2<T>::zero{ static_cast<T>(0), static_cast<T>(0) };

    template<Arithmetic T>
    /*static*/ const Vector2<T> Vector2<T>::up{ static_cast<T>(0), static_cast<T>(1) };

    template<Arithmetic T>
    /*static*/ const Vector2<T> Vector2<T>::down{ static_cast<T>(0), static_cast<T>(-1) };

    template<Arithmetic T>
    /*static*/ const Vector2<T> Vector2<T>::left{ static_cast<T>(-1), static_cast<T>(0) };

    template<Arithmetic T>
    /*static*/ const Vector2<T> Vector2<T>::right{ static_cast<T>(1), static_cast<T>(0) };

    template<Arithmetic T>
    std::ostream& operator<<(std::ostream& output, const Vector2<T>& v)
    {
        output << "(" << v.x << "," << v.y << ")" << std::endl;
        return output;
    }

    template<Arithmetic T>
    Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) { return { a.x + b.x, a.y + b.y }; }

    template<Arithmetic T>
    Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<Arithmetic T>
    Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) { return { a.x - b.x, a.y - b.y }; }

    template<Arithmetic T>
    Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template<Arithmetic T>
    Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b) { return { a.x * b.x, a.y * b.y }; }

    template<Arithmetic T>
    Vector2<T> operator*(const Vector2<T>& v, T s) { return Vector2<T>{ v.x * s, v.y * s }; }

    template<Arithmetic T>
    Vector2<T> operator*(T s, const Vector2<T>& v) { return Vector2<T>{ v.x * s, v.y * s }; }

    template<Arithmetic T>
    Vector2<T>& Vector2<T>::operator*=(T s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    template<Arithmetic T>
    Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b) { return { a.x / b.x, a.y / b.y }; }

    template<Arithmetic T>
    Vector2<T> operator/(const Vector2<T>& v, T s) { return Vector2<T>{ v.x / s, v.y / s }; }

    template<Arithmetic T>
    Vector2<T> operator/(T s, const Vector2<T>& v) { return Vector2<T>{ v.x / s, v.y / s }; }

    template<Arithmetic T>
    Vector2<T>& Vector2<T>::operator/=(T s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    template<Arithmetic T>
    T Vector2<T>::LengthSquared() const
    {
        return x * x + y * y;
    }

    template<Arithmetic T>
    T Vector2<T>::Length() const
    {
        return static_cast<T>(std::sqrt(LengthSquared()));
    }

    template<Arithmetic T>
    Vector2<T>& Vector2<T>::Normalize()
    {
        const auto length{ Length() };
        this->operator/=(length);
        return *this;
    }
}

#define DECL_Vector2(name, type) \
    using name = Core::Math::Vector2<type>; \
    IMPL_HASH(name, value.x, value.y); \

DECL_Vector2(Vector2_32, int32);
DECL_Vector2(Vector2_64, int64);
DECL_Vector2(Vector2_f, float);
DECL_Vector2(Vector2_d, double);
