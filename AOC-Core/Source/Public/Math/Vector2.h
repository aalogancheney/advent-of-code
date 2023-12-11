#pragma once

#include "pch.h"

#include "Types.h"
#include "Hashing.h"
#include "Concepts.h"

namespace Core::Math
{
    template<SignedArithmetic T>
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
        static const Vector2<T> rightUp;
        static const Vector2<T> leftUp;
        static const Vector2<T> rightDown;
        static const Vector2<T> leftDown;
        
        template<SignedArithmetic U>
        using NeighboringDirectionCollection = std::unordered_set<Vector2<U>>;

        static const NeighboringDirectionCollection<T> cardinalNeighboringDirections;
        static const NeighboringDirectionCollection<T> cardinalNeighboringDirectionsWithDiagonals;

        bool operator==(const Vector2<T>&) const = default;
        auto operator<=>(const Vector2<T>&) const = default;

        template<SignedArithmetic U> friend std::ostream& operator<<(std::ostream& output, const Vector2<T>& v);

        // Addition
        template<SignedArithmetic U> friend Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b);
        Vector2<T>& operator+=(const Vector2<T>& other);

        // Subtraction
        template<SignedArithmetic U> friend Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b);
        Vector2<T>& operator-=(const Vector2<T>& other);

        // Multiplication
        template<SignedArithmetic U> friend Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b);
        template<SignedArithmetic U> friend Vector2<T> operator*(const Vector2<T>& v, T s);
        template<SignedArithmetic U> friend Vector2<T> operator*(T s, const Vector2<T>& v);
        Vector2<T>& operator*=(T s);

        // Division
        template<SignedArithmetic U> friend Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b);
        template<SignedArithmetic U> friend Vector2<T> operator/(const Vector2<T>& v, T s);
        template<SignedArithmetic U> friend Vector2<T> operator/(T s, const Vector2<T>& v);
        Vector2<T>& operator/=(T s);

        T LengthSquared() const;
        T Length() const;
        Vector2<T>& Normalize();
    };

    template<SignedArithmetic T> const Vector2<T> Vector2<T>::zero      {  0,  0 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::up        {  0,  1 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::down      {  0, -1 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::left      { -1,  0 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::right     {  1,  0 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::rightUp   {  1,  1 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::leftUp    { -1,  1 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::rightDown {  1, -1 };
    template<SignedArithmetic T> const Vector2<T> Vector2<T>::leftDown  { -1, -1 };

    template<SignedArithmetic T>
    const Vector2<T>::NeighboringDirectionCollection<T> Vector2<T>::cardinalNeighboringDirections
    {
        // Static initialization ordering means the named variables can't be used here.
        {  0,  1 },
        {  0, -1 },
        { -1,  0 },
        {  1,  0 },
    };

    template<SignedArithmetic T>
    const Vector2<T>::NeighboringDirectionCollection<T> Vector2<T>::cardinalNeighboringDirectionsWithDiagonals
    {
        // Static initialization ordering means the named variables can't be used here.
        {  0,  1 },
        {  0, -1 },
        { -1,  0 },
        {  1,  0 },
        {  1,  1 },
        { -1,  1 },
        {  1, -1 },
        { -1, -1 },
    };

    template<SignedArithmetic T>
    std::ostream& operator<<(std::ostream& output, const Vector2<T>& v)
    {
        output << "(" << v.x << "," << v.y << ")" << std::endl;
        return output;
    }

    template<SignedArithmetic T>
    Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) { return { a.x + b.x, a.y + b.y }; }

    template<SignedArithmetic T>
    Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<SignedArithmetic T>
    Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) { return { a.x - b.x, a.y - b.y }; }

    template<SignedArithmetic T>
    Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template<SignedArithmetic T>
    Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b) { return { a.x * b.x, a.y * b.y }; }

    template<SignedArithmetic T>
    Vector2<T> operator*(const Vector2<T>& v, T s) { return Vector2<T>{ v.x * s, v.y * s }; }

    template<SignedArithmetic T>
    Vector2<T> operator*(T s, const Vector2<T>& v) { return Vector2<T>{ v.x * s, v.y * s }; }

    template<SignedArithmetic T>
    Vector2<T>& Vector2<T>::operator*=(T s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    template<SignedArithmetic T>
    Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b) { return { a.x / b.x, a.y / b.y }; }

    template<SignedArithmetic T>
    Vector2<T> operator/(const Vector2<T>& v, T s) { return Vector2<T>{ v.x / s, v.y / s }; }

    template<SignedArithmetic T>
    Vector2<T> operator/(T s, const Vector2<T>& v) { return Vector2<T>{ v.x / s, v.y / s }; }

    template<SignedArithmetic T>
    Vector2<T>& Vector2<T>::operator/=(T s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    template<SignedArithmetic T>
    T Vector2<T>::LengthSquared() const
    {
        return x * x + y * y;
    }

    template<SignedArithmetic T>
    T Vector2<T>::Length() const
    {
        return static_cast<T>(std::sqrt(LengthSquared()));
    }

    template<SignedArithmetic T>
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
