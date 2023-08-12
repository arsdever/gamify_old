#pragma once

#include <limits>

namespace g::common
{

template <typename T, typename U>
inline bool fp_equals(T n1, U n2);

template <>
inline bool fp_equals(float n1, float n2)
{
    return std::abs(n1 - n2) < std::numeric_limits<float>::epsilon();
}

template <>
inline bool fp_equals(double n1, double n2)
{
    return std::abs(n1 - n2) < std::numeric_limits<double>::epsilon();
}

template <>
inline bool fp_equals(float n1, double n2)
{
    return static_cast<double>(n1) == n2;
}

template <>
inline bool fp_equals(double n1, float n2)
{
    return n1 == static_cast<double>(n2);
}

} // namespace g::common
