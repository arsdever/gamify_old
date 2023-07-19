#pragma once

#include <array>

namespace g::common
{

struct vector3
{
    vector3() = default;
    vector3(float x, float y, float z)
    {
        _data[ 0 ] = x;
        _data[ 1 ] = y;
        _data[ 2 ] = z;
    }
    vector3(vector3 const& other) = default;
    vector3(vector3&& other) = default;
    vector3& operator=(vector3 const& other) = default;
    vector3& operator=(vector3&& other) = default;
    vector3(std::array<float, 3> const& data)
        : _data(data)
    {
    }
    vector3(std::array<float, 3>&& data)
        : _data(std::move(data))
    {
    }

    std::array<float, 3> _data;
    float& x() { return _data[ 0 ]; }
    float& y() { return _data[ 1 ]; }
    float& z() { return _data[ 2 ]; }

    const float& x() const { return _data[ 0 ]; }
    const float& y() const { return _data[ 1 ]; }
    const float& z() const { return _data[ 2 ]; }

    void normalize()
    {
        float length = sqrtf(x() * x() + y() * y() + z() * z());
        x() /= length;
        y() /= length;
        z() /= length;
    }

    std::array<float, 3> as_array() const { return { x(), y(), z() }; }
};

struct vector2
{
    float x;
    float y;
};

} // namespace g::common
