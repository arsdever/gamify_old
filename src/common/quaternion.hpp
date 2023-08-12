#pragma once

#include <cmath>

#include "common/floating_point_arithmetics.hpp"
#include "common/vector.hpp"

namespace g::common
{

struct quaternion
{
    float x;
    float y;
    float z;
    float w;

    void normalize()
    {
        float length = sqrtf(x * x + y * y + z * z + w * w);
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    static quaternion identity() { return quaternion { 0, 0, 0, 1 }; }

    bool operator==(quaternion const& other) const
    {
        return fp_equals(x, other.x) && fp_equals(y, other.y) &&
               fp_equals(z, other.z) && fp_equals(w, other.w);
    }

    bool operator!=(quaternion const& other) const { return !(*this == other); }

    friend vector3f operator*(vector3f const& v, quaternion const& q)
    {
        vector3f t = vector3f(q.x, q.y, q.z).cross(v) * 2.0f;
        return v + t * q.w + vector3f(q.x, q.y, q.z).cross(t);
    }

    std::array<float, 9> to_matrix() const
    {
        std::array<float, 9> m;
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        m[ 0 ] = 1 - 2 * (y2 + z2);
        m[ 1 ] = 2 * (xy - wz);
        m[ 2 ] = 2 * (xz + wy);

        m[ 3 ] = 2 * (xy + wz);
        m[ 4 ] = 1 - 2 * (x2 + z2);
        m[ 5 ] = 2 * (yz - wx);

        m[ 6 ] = 2 * (xz - wy);
        m[ 7 ] = 2 * (yz + wx);
        m[ 8 ] = 1 - 2 * (x2 + y2);

        return m;
    }

    static quaternion
    look_rotation(vector3f from, vector3f to, vector3f up = vector3f::up())
    {
        vector3f direction = (to - from).normalized();
        if (direction.x() == 0 && direction.y() == 0 && direction.z() == 0)
        {
            return quaternion::identity();
        }

        float dot = vector3f::forward().dot(direction);
        if (std::abs(dot + 1.f) < 0.000001f)
        {
            // vector3f:forward() and direction point exactly in the opposite
            // direction,
            return quaternion { 0, 1, 0, 0 };
        }
        else if (std::abs(dot - 1.f) < 0.000001f)
        {
            // vector3f::forward() and direction point exactly in the same
            // direction, so we return the identity quaternion
            return quaternion::identity();
        }

        float rotAngle = std::acos(dot);
        vector3f rotAxis = vector3f::forward().cross(direction);

        quaternion q = quaternion::from_axis_angle(rotAxis, rotAngle);
        q.normalize();
        return q;
    }

    static quaternion from_axis_angle(vector3f axis, float angle)
    {
        float halfAngle = angle * .5f;
        float s = (float)std::sin(halfAngle);
        quaternion q;
        q.x = axis.x() * s;
        q.y = axis.y() * s;
        q.z = axis.z() * s;
        q.w = (float)std::cos(halfAngle);
        return q;
    }
};

} // namespace g::common
