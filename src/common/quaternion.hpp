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

    static quaternion look_rotation(vector3 from, vector3 to)
    {
        vector3 forward = from - to;
        forward.normalize();

        float dot = vector3 { 0, 0, 1 } * forward;

        if (std::abs(dot - (-1.0f)) < 0.000001f)
        {
            return { 0, 1, 0, 3.1415926535897932f };
        }
        if (std::abs(dot - (1.0f)) < 0.000001f)
        {
            return { 0, 0, 0, 1 };
        }

        float rotAngle = (float)std::acos(dot);
        vector3 rotAxis = vector3 { 0, 0, 1 }.cross(forward);
        rotAxis.normalize();
        return from_axis_angle(rotAxis, rotAngle);
    }

    static quaternion from_axis_angle(vector3 axis, float angle)
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
