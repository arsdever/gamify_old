#pragma once

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
};

} // namespace g::common
