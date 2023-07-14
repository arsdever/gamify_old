#pragma once

#include <array>
#include <concepts>

namespace g::common
{

template <typename T>
concept is_number = std::is_arithmetic_v<T>;

template <unsigned N>
struct vector
{
public:
    template <unsigned M>
    vector<N>& operator=(vector<M> const& other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] = other.data[ i ];
        return *this;
    }

    template <unsigned M>
    vector<std::max(N, M)> operator+(vector<M> const& other) const
    {
        vector<std::max(N, M)> result;
        for (unsigned i = 0; i < std::min(N, M); ++i)
            result.data[ i ] = data[ i ] + other.data[ i ];
        return result;
    }

    template <unsigned M>
    vector<std::max(N, M)> operator-(vector<M> const& other) const
    {
        vector<std::max(N, M)> result;
        for (unsigned i = 0; i < std::min(N, M); ++i)
            result.data[ i ] = data[ i ] - other.data[ i ];
        return result;
    }

    template <unsigned M>
    vector<std::max(N, M)> operator*(vector<M> const& other) const
    {
        vector<std::max(N, M)> result;
        for (unsigned i = 0; i < std::min(N, M); ++i)
            result.data[ i ] = data[ i ] * other.data[ i ];
        return result;
    }

    template <unsigned M>
    vector<std::max(N, M)> operator/(vector<M> const& other) const
    {
        vector<std::max(N, M)> result;
        for (unsigned i = 0; i < std::min(N, M); ++i)
            result.data[ i ] = data[ i ] / other.data[ i ];
        return result;
    }

    template <is_number numeric>
    vector<N> operator+(numeric other) const
    {
        vector<N> result;
        for (unsigned i = 0; i < N; ++i)
            result.data[ i ] = data[ i ] + other;
        return result;
    }

    template <is_number numeric>
    vector<N> operator-(numeric other) const
    {
        vector<N> result;
        for (unsigned i = 0; i < N; ++i)
            result.data[ i ] = data[ i ] - other;
        return result;
    }

    template <is_number numeric>
    vector<N> operator*(numeric other) const
    {
        vector<N> result;
        for (unsigned i = 0; i < N; ++i)
            result.data[ i ] = data[ i ] * other;
        return result;
    }

    template <is_number numeric>
    vector<N> operator/(numeric other) const
    {
        vector<N> result;
        for (unsigned i = 0; i < N; ++i)
            result.data[ i ] = data[ i ] / other;
        return result;
    }

    vector<N>& operator+=(vector<N> const& other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] += other.data[ i ];
        return *this;
    }

    vector<N>& operator-=(vector<N> const& other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] -= other.data[ i ];
        return *this;
    }

    vector<N>& operator*=(vector<N> const& other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] *= other.data[ i ];
        return *this;
    }

    vector<N>& operator/=(vector<N> const& other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] /= other.data[ i ];
        return *this;
    }

    template <is_number numeric>
    vector<N>& operator+=(numeric other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] += other;
        return *this;
    }

    template <is_number numeric>
    vector<N>& operator-=(numeric other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] -= other;
        return *this;
    }

    template <is_number numeric>
    vector<N>& operator*=(numeric other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] *= other;
        return *this;
    }

    template <is_number numeric>
    vector<N>& operator/=(numeric other)
    {
        for (unsigned i = 0; i < N; ++i)
            data[ i ] /= other;
        return *this;
    }

    float& operator[](unsigned i) { return data[ i ]; }
    float const& operator[](unsigned i) const { return data[ i ]; }

    float* begin() { return data.begin(); }
    float const* begin() const { return data.begin(); }

    float* end() { return data.end(); }
    float const* end() const { return data.end(); }

    float* data() { return data.data(); }
    float const* data() const { return data.data(); }

    size_t size() const { return data.size(); }

private:
    std::array<float, N> data;
};

struct vector2 : public vector<2>
{
    vector2(float x, float y)
        : vector<2> { { x, y } }
    {
    }

    float x() const { return data[ 0 ]; }
    float y() const { return data[ 1 ]; }

    void set_x(float x) { data[ 0 ] = x; }
    void set_y(float y) { data[ 1 ] = y; }

    void* raw() { return data.data(); }
    size_t size() const { return data.size() * sizeof(float); }
};

struct vector3 : private vector<3>
{
    vector3(float x, float y, float z)
        : vector<3> { { x, y, z } }
    {
    }

    float x() const { return data[ 0 ]; }
    float y() const { return data[ 1 ]; }
    float z() const { return data[ 2 ]; }

    void set_x(float x) { data[ 0 ] = x; }
    void set_y(float y) { data[ 1 ] = y; }
    void set_z(float z) { data[ 2 ] = z; }

    void* raw() { return data.data(); }
    size_t size() const { return data.size() * sizeof(float); }
};

} // namespace g::common
