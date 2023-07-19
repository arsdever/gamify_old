#pragma once

#include <cmath>

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

    static quaternion look_rotation(vector3f from, vector3f to)
    {
        vector3f forward = from - to;
        forward.normalize();

        float dot = vector3f { 0, 0, 1 }.dot(forward);

        if (std::abs(dot - (-1.0f)) < 0.000001f)
        {
            return { 0, 1, 0, 3.1415926535897932f };
        }
        if (std::abs(dot - (1.0f)) < 0.000001f)
        {
            return { 0, 0, 0, 1 };
        }

        float rotAngle = (float)std::acos(dot);
        vector3f rotAxis = vector3f { 0, 0, 1 }.cross(forward);
        rotAxis.normalize();
        return from_axis_angle(rotAxis, rotAngle);
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
