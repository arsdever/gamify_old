#pragma once

#include <array>

#include "common/concepts.hpp"

namespace g::common
{

template <is_scalar T>
class vector3
{
#pragma region static

public:
    static vector3 zero() { return { 0, 0, 0 }; }

    static vector3 forward() { return { 0, 0, 1 }; }

    static vector3 up() { return { 0, 1, 0 }; }

    static vector3 right() { return { 1, 0, 0 }; }

#pragma endregion

public:
    vector3() = default;
    vector3(vector3 const& other) = default;
    vector3(vector3&& other) = default;
    vector3& operator=(vector3 const& other) = default;
    vector3& operator=(vector3&& other) = default;
    vector3(T x, T y, T z)
        : _data { x, y, z }
    {
    }
    vector3(std::array<T, 3> const& data)
        : _data(data)
    {
    }
    vector3(std::array<T, 3>&& data)
        : _data(std::move(data))
    {
    }

    T& x() { return _data[ 0 ]; }
    T& y() { return _data[ 1 ]; }
    T& z() { return _data[ 2 ]; }

    const T& x() const { return _data[ 0 ]; }
    const T& y() const { return _data[ 1 ]; }
    const T& z() const { return _data[ 2 ]; }

    void normalize()
    {
        auto l = length();
        x() /= l;
        y() /= l;
        z() /= l;
    }

    template <typename U = T>
    vector3<U> normalized() const
    {
        auto l = length();
        return { static_cast<U>(x() / l),
                 static_cast<U>(y() / l),
                 static_cast<U>(z() / l) };
    }

    T length() const { return std::sqrt(x() * x() + y() * y() + z() * z()); }

    T& operator[](int index) { return _data[ index ]; }
    const T& operator[](int index) const { return _data[ index ]; }

    std::array<T, 3>& as_array() { return _data; }
    const std::array<T, 3>& as_array() const { return _data; }

    template <is_scalar U>
    vector3<T>& operator+=(vector3<U> const& other)
    {
        x() += other.x();
        y() += other.y();
        z() += other.z();
        return *this;
    }

    template <is_scalar U>
    vector3<T> operator+(vector3<U> const& other) const
    {
        vector3<T> copy(*this);
        copy += other;
        return copy;
    }

    template <is_scalar U>
    vector3<T>& operator-=(vector3<U> const& other)
    {
        x() -= other.x();
        y() -= other.y();
        z() -= other.z();
        return *this;
    }

    vector3<T> operator-() const { return { -x(), -y(), -z() }; }

    template <is_scalar U>
    vector3<T> operator-(vector3<U> const& other) const
    {
        vector3<T> copy(*this);
        copy -= other;
        return copy;
    }

    template <is_scalar U>
    vector3<T>& operator*=(U scalar)
    {
        x() *= scalar;
        y() *= scalar;
        z() *= scalar;
        return *this;
    }

    template <is_scalar U>
    vector3<T> operator*(U scalar) const
    {
        vector3<T> copy(*this);
        copy *= scalar;
        return copy;
    }

    template <is_scalar U>
    vector3<T>& operator/=(U scalar)
    {
        x() /= scalar;
        y() /= scalar;
        z() /= scalar;
        return *this;
    }

    template <is_scalar U>
    vector3<T> operator/(U scalar) const
    {
        vector3<T> copy(*this);
        copy /= scalar;
        return copy;
    }

    template <is_scalar U>
    float dot(vector3<U> const& other) const
    {
        return x() * other.x() + y() * other.y() + z() * other.z();
    }

    template <is_scalar U>
    vector3<T> cross(vector3<U> const& other) const
    {
        return vector3<T>(y() * other.z() - z() * other.y(),
                          z() * other.x() - x() * other.z(),
                          x() * other.y() - y() * other.x());
    }

    template <is_scalar U>
    vector3<U> as_vector() const
    {
        return { static_cast<U>(x()),
                 static_cast<U>(y()),
                 static_cast<U>(z()) };
    }

private:
    std::array<T, 3> _data;
};

using vector3f = vector3<float>;
using vector3d = vector3<double>;

struct vector2
{
    float x;
    float y;
};

} // namespace g::common
