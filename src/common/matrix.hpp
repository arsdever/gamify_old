#pragma once

#include <array>

#include "common/concepts.hpp"
#include "common/quaternion.hpp"
#include "common/vector.hpp"

namespace g::common
{

template <is_scalar T>
class matrix4x4
{
#pragma region static

public:
    /**
     * @brief Get the identity matrix.
     *
     * @return matrix4x4 the identity matrix.
     */
    static matrix4x4<T> identity();

    /**
     * @brief Get the zero matrix.
     *
     * @return matrix4x4 the zero matrix.
     */
    static matrix4x4<T> zero();

    /**
     * @brief Get a matrix for perspective projection.
     *
     * @tparam fov_t the type of the field of view
     * @tparam aspect_t the type of the aspect ratio
     * @tparam near_plane_t the type of the near plane
     * @tparam far_plane_t the type of the far plane
     * @param fov the field of view
     * @param aspect the aspect ratio
     * @param near the near plane
     * @param far the far plane
     * @return matrix4x4<T> the matrix
     */
    template <typename fov_t,
              typename aspect_t,
              typename near_plane_t,
              typename far_plane_t>
    static matrix4x4<T> perspective(fov_t fov,
                                    aspect_t aspect,
                                    near_plane_t near_plane,
                                    far_plane_t far_plane);

    template <typename dimension_t, typename near_plane_t, typename far_plane_t>
    static matrix4x4<T> orthographic(dimension_t width,
                                     dimension_t height,
                                     near_plane_t near_plane,
                                     far_plane_t far_plane);

    /**
     * @brief Get the matrix from raw data.
     *
     * @param data the raw data
     * @return matrix4x4 the matrix
     */
    static matrix4x4<T> from_raw_data(const T* data);

    template <is_scalar U>
    static matrix4x4<T>
    from_look(vector3<U> from, vector3<U> to, vector3<U> up);

    /**
     * @brief Get the matrix from a quaternion.
     *
     * @param quat the quaternion
     * @return matrix4x4<T> the matrix of the quaternion
     */
    static matrix4x4<T> from_quaternion(quaternion quat);

#pragma endregion

public:
    matrix4x4()
        : _data { 0 }
    {
    }

    matrix4x4(const matrix4x4<T>& other)
    {
        std::copy(other._data.begin(), other._data.end(), _data.begin());
    }

    matrix4x4(matrix4x4<T>&& other)
    {
        std::copy(other._data.begin(), other._data.end(), _data.begin());
    }

    matrix4x4& operator=(const matrix4x4<T>& other)
    {
        std::copy(other._data.begin(), other._data.end(), _data.begin());
        return *this;
    }

    matrix4x4& operator=(matrix4x4<T>&& other)
    {
        std::copy(other._data.begin(), other._data.end(), _data.begin());
        return *this;
    }

    /**
     * @brief Get the data of the matrix.
     *
     * @return const T* the data
     */
    const T* raw_data() const
    {
        return reinterpret_cast<const T*>(_data.data());
    }

    /**
     * @brief Get column from matrix.
     *
     * @param index the column index to get
     * @return std::array<T, 4>& the column
     */
    std::array<T, 4>& operator[](int index)
    {
        return *reinterpret_cast<std::array<T, 4>*>(&_data[ index * 4 ]);
    }

    /**
     * @brief Get column from matrix.
     *
     * @param index the column index to get
     * @return const std::array<T, 4>& the column
     */
    const std::array<T, 4>& operator[](int index) const
    {
        return *reinterpret_cast<const std::array<T, 4>*>(&_data[ index * 4 ]);
    }

    /**
     * @brief Get element from matrix.
     *
     * @param row the row index to get
     * @param column the column index to get
     * @return T& the element
     */
    T& operator()(int row, int column) { return (*this)[ column ][ row ]; }

    /**
     * @brief Get element from matrix.
     *
     * @param row the row index to get
     * @param column the column index to get
     * @return T the element
     */
    const T& operator()(int row, int column) const
    {
        return (*this)[ column ][ row ];
    }

    template <is_scalar V>
    matrix4x4<T>& operator+=(V scalar)
    {
        std::for_each(_data.begin(),
                      _data.end(),
                      [ &scalar ](T& value) { value += scalar; });
        return *this;
    }

    template <is_scalar V>
    matrix4x4<T>& operator-=(V scalar)
    {
        std::for_each(_data.begin(),
                      _data.end(),
                      [ &scalar ](T& value) { value -= scalar; });
        return *this;
    }

    template <is_scalar V>
    matrix4x4<T>& operator*=(V scalar)
    {
        std::for_each(_data.begin(),
                      _data.end(),
                      [ &scalar ](T& value) { value *= scalar; });
        return *this;
    }

    template <is_scalar V>
    matrix4x4<T>& operator/=(V scalar)
    {
        std::for_each(_data.begin(),
                      _data.end(),
                      [ &scalar ](T& value) { value /= scalar; });
        return *this;
    }

    template <is_scalar U>
    matrix4x4<T> operator+(U scalar) const
    {
        matrix4x4<T> result = *this;
        result += scalar;
        return result;
    }

    template <is_scalar U>
    matrix4x4<T> operator-(U scalar) const
    {
        matrix4x4<T> result = *this;
        result -= scalar;
        return result;
    }

    template <is_scalar U>
    matrix4x4<T> operator*(U scalar) const
    {
        matrix4x4<T> result = *this;
        result *= scalar;
        return result;
    }

    template <is_scalar U>
    matrix4x4<T> operator/(U scalar) const
    {
        matrix4x4<T> result = *this;
        result /= scalar;
        return result;
    }

    template <typename U>
    matrix4x4<T>& operator+=(const matrix4x4<U>& other)
    {
        for (auto i = 0; i < 4; ++i)
        {
            for (auto j = 0; j < 4; ++j)
            {
                (*this)[ i ][ j ] += other[ i ][ j ];
            }
        }
        return *this;
    }

    template <typename U>
    matrix4x4<T>& operator-=(const matrix4x4<U>& other)
    {
        for (auto i = 0; i < 4; ++i)
        {
            for (auto j = 0; j < 4; ++j)
            {
                (*this)[ i ][ j ] -= other[ i ][ j ];
            }
        }
        return *this;
    }

    template <typename U>
    matrix4x4<T>& operator*=(const matrix4x4<U>& other)
    {
        matrix4x4<T> result;
        for (auto i = 0; i < 4; ++i)
        {
            for (auto j = 0; j < 4; ++j)
            {
                T sum = 0;
                for (auto k = 0; k < 4; ++k)
                {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        *this = result;
        return *this;
    }

    template <typename U>
    matrix4x4<T> operator+(const matrix4x4<U>& other) const
    {
        matrix4x4<T> result = *this;
        result += other;
        return result;
    }

    template <typename U>
    matrix4x4<T> operator-(const matrix4x4<U>& other) const
    {
        matrix4x4<T> result = *this;
        result -= other;
        return result;
    }

    template <typename U>
    matrix4x4<T> operator*(const matrix4x4<U>& other) const
    {
        matrix4x4<T> result = *this;
        result *= other;
        return result;
    }

    template <typename U>
    std::array<T, 3> operator*(const std::array<U, 3>& vector) const
    {
        std::array<T, 4> result { 0 };

        result[ 0 ] = (*this)(0, 0) * vector[ 0 ] +
                      (*this)(0, 1) * vector[ 1 ] +
                      (*this)(0, 2) * vector[ 2 ] + (*this)(0, 3);
        result[ 1 ] = (*this)(1, 0) * vector[ 0 ] +
                      (*this)(1, 1) * vector[ 1 ] +
                      (*this)(1, 2) * vector[ 2 ] + (*this)(1, 3);
        result[ 2 ] = (*this)(2, 0) * vector[ 0 ] +
                      (*this)(2, 1) * vector[ 1 ] +
                      (*this)(2, 2) * vector[ 2 ] + (*this)(2, 3);
        result[ 3 ] = (*this)(3, 0) * vector[ 0 ] +
                      (*this)(3, 1) * vector[ 1 ] +
                      (*this)(3, 2) * vector[ 2 ] + (*this)(3, 3);

        return { result[ 0 ] / result[ 3 ],
                 result[ 1 ] / result[ 3 ],
                 result[ 2 ] / result[ 3 ] };
    }

    template <is_scalar U>
    friend matrix4x4<T> operator+(U scalar, matrix4x4<T> matrix);

    template <is_scalar U>
    friend matrix4x4<T> operator-(U scalar, matrix4x4<T> matrix);

    template <is_scalar U>
    friend matrix4x4<T> operator*(U scalar, matrix4x4<T> matrix);

    template <is_scalar U>
    friend matrix4x4<T> operator/(U scalar, matrix4x4<T> matrix);

    /**
     * @brief Convert the matrix to a matrix of another type.
     *
     * Constructs a new matrix of type V applying @c static_cast to each
     * element.
     *
     * @tparam V the type to convert to
     * @return matrix4x4<V> the converted matrix
     */
    template <typename V>
    matrix4x4<V> convert_to_matrix() const
    {
        matrix4x4<V> result;
        for (int i = 0; i < 16; ++i)
        {
            result.data[ i ] = static_cast<V>(_data[ i ]);
        }
        return result;
    }

#pragma region Transform operations

public:
    void set_position(vector3<T> position)
    {
        (*this)(0, 3) = position.x();
        (*this)(1, 3) = position.y();
        (*this)(2, 3) = position.z();
    }

    void translate(vector3<T> translation)
    {
        (*this)(0, 3) += translation.x();
        (*this)(1, 3) += translation.y();
        (*this)(2, 3) += translation.z();
    }

    void set_rotation(quaternion rotation)
    {
        *this = from_quaternion(rotation);
    }

    void rotate(quaternion rotation)
    {
        *this *= from_quaternion(rotation);
    }

    void set_scale(vector3<T> scale)
    {
        (*this)(0, 0) = scale.x();
        (*this)(1, 1) = scale.y();
        (*this)(2, 2) = scale.z();
    }

    void scale(vector3<T> scale)
    {
        (*this)(0, 0) *= scale.x();
        (*this)(1, 1) *= scale.y();
        (*this)(2, 2) *= scale.z();
    }

#pragma endregion

private:
    std::array<T, 16> _data;
};

template <is_scalar T>
matrix4x4<T> matrix4x4<T>::identity()
{
    matrix4x4<T> result;
    result[ 0 ][ 0 ] = 1;
    result[ 1 ][ 1 ] = 1;
    result[ 2 ][ 2 ] = 1;
    result[ 3 ][ 3 ] = 1;
    return result;
}

template <is_scalar T>
matrix4x4<T> matrix4x4<T>::zero()
{
    return {};
}

template <is_scalar T>
template <typename fov_t,
          typename aspect_t,
          typename near_plane_t,
          typename far_plane_t>
matrix4x4<T> matrix4x4<T>::perspective(fov_t fov,
                                       aspect_t aspect,
                                       near_plane_t near_plane,
                                       far_plane_t far_plane)
{
    matrix4x4<T> result;
    result[ 0 ][ 0 ] = static_cast<T>(1 / (std::tan(fov / 2)));
    result[ 1 ][ 1 ] = static_cast<T>(aspect / std::tan(fov / 2));
    // if far_plane is infinite
    if (far_plane == std::numeric_limits<T>::infinity())
    {
        result[ 2 ][ 2 ] = static_cast<T>(-1);
        result[ 2 ][ 3 ] = static_cast<T>(-2 * near_plane);
    }
    else
    {
        result[ 2 ][ 2 ] =
            static_cast<T>((near_plane + far_plane) / (near_plane - far_plane));
        result[ 2 ][ 3 ] = static_cast<T>(2 * near_plane * far_plane /
                                          (near_plane - far_plane));
    }
    result[ 3 ][ 2 ] = static_cast<T>(-1);
    return result;
}

template <is_scalar T>
template <typename dimension_t, typename near_plane_t, typename far_plane_t>
matrix4x4<T> matrix4x4<T>::orthographic(dimension_t width,
                                        dimension_t height,
                                        near_plane_t near_plane,
                                        far_plane_t far_plane)
{
    matrix4x4<T> result;
    result[ 0 ][ 0 ] = static_cast<T>(2 / width);
    result[ 1 ][ 1 ] = static_cast<T>(2 / height);
    result[ 2 ][ 2 ] = static_cast<T>(
        2 / (near_plane - far_plane)); // if far_plane is infinite
    if (far_plane == std::numeric_limits<T>::infinity())
    {
        result[ 2 ][ 3 ] = static_cast<T>(-1);
    }
    else
    {
        result[ 2 ][ 3 ] =
            static_cast<T>((near_plane + far_plane) / (near_plane - far_plane));
    }
    result[ 3 ][ 3 ] = static_cast<T>(1);
    return result;
}

template <is_scalar T>
template <is_scalar U>
matrix4x4<T>
matrix4x4<T>::from_look(vector3<U> from, vector3<U> to, vector3<U> up)
{
    vector3 forward = to - from;
    // TODO: if forward is null return

    forward.normalize();
    vector3 side = forward.cross(up);
    side.normalize();
    vector3 upVector = side.cross(forward);

    matrix4x4<T> result;
    result[ 0 ][ 0 ] = side.x();
    result[ 1 ][ 0 ] = side.y();
    result[ 2 ][ 0 ] = side.z();
    result[ 3 ][ 0 ] = 0.0f;
    result[ 0 ][ 1 ] = upVector.x();
    result[ 1 ][ 1 ] = upVector.y();
    result[ 2 ][ 1 ] = upVector.z();
    result[ 3 ][ 1 ] = 0.0f;
    result[ 0 ][ 2 ] = -forward.x();
    result[ 1 ][ 2 ] = -forward.y();
    result[ 2 ][ 2 ] = -forward.z();
    result[ 3 ][ 2 ] = 0.0f;
    result[ 0 ][ 3 ] = 0.0f;
    result[ 1 ][ 3 ] = 0.0f;
    result[ 2 ][ 3 ] = 0.0f;
    result[ 3 ][ 3 ] = 1.0f;

    result[ 3 ][ 0 ] += result[ 0 ][ 0 ] * (-from.x()) +
                        result[ 1 ][ 0 ] * (-from.y()) +
                        result[ 2 ][ 0 ] * (-from.z());
    result[ 3 ][ 1 ] += result[ 0 ][ 1 ] * (-from.x()) +
                        result[ 1 ][ 1 ] * (-from.y()) +
                        result[ 2 ][ 1 ] * (-from.z());
    result[ 3 ][ 2 ] += result[ 0 ][ 2 ] * (-from.x()) +
                        result[ 1 ][ 2 ] * (-from.y()) +
                        result[ 2 ][ 2 ] * (-from.z());
    result[ 3 ][ 3 ] += result[ 0 ][ 3 ] * (-from.x()) +
                        result[ 1 ][ 3 ] * (-from.y()) +
                        result[ 2 ][ 3 ] * (-from.z());

    return result;
}

template <is_scalar T>
matrix4x4<T> matrix4x4<T>::from_quaternion(quaternion quat)
{
    matrix4x4<T> result;

    result(0, 0) = 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);
    result(1, 0) = 2.0f * (quat.x * quat.y + quat.z * quat.w);
    result(2, 0) = 2.0f * (quat.x * quat.z - quat.y * quat.w);
    result(3, 0) = 0.0f;
    result(0, 1) = 2.0f * (quat.x * quat.y - quat.z * quat.w);
    result(1, 1) = 1.0f - 2.0f * (quat.x * quat.x + quat.z * quat.z);
    result(2, 1) = 2.0f * (quat.z * quat.y + quat.x * quat.w);
    result(3, 1) = 0.0f;
    result(0, 2) = 2.0f * (quat.x * quat.z + quat.y * quat.w);
    result(1, 2) = 2.0f * (quat.y * quat.z - quat.x * quat.w);
    result(2, 2) = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
    result(3, 2) = 0.0f;
    result(0, 3) = 0.0f;
    result(1, 3) = 0.0f;
    result(2, 3) = 0.0f;
    result(3, 3) = 1.0f;

    return result;
}

template <is_scalar T>
matrix4x4<T> matrix4x4<T>::from_raw_data(const T* data)
{
    matrix4x4<T> result;
    std::copy(data, data + 16, result._data.begin());
    return result;
}

template <is_scalar T, is_scalar U>
matrix4x4<T> operator+(U scalar, matrix4x4<T> matrix)
{
    std::for_each(matrix.begin(),
                  matrix.end(),
                  [ scalar ](T& element) { element += scalar; });
    return matrix;
}

template <is_scalar T, is_scalar U>
matrix4x4<T> operator-(U scalar, matrix4x4<T> matrix)
{
    std::for_each(matrix.begin(),
                  matrix.end(),
                  [ scalar ](T& element) { element -= scalar; });
    return matrix;
}

template <is_scalar T, is_scalar U>
matrix4x4<T> operator*(U scalar, matrix4x4<T> matrix)
{
    std::for_each(matrix.begin(),
                  matrix.end(),
                  [ scalar ](T& element) { element *= scalar; });
    return matrix;
}

template <is_scalar T, is_scalar U>
matrix4x4<T> operator/(U scalar, matrix4x4<T> matrix)
{
    std::for_each(matrix.begin(),
                  matrix.end(),
                  [ scalar ](T& element) { element /= scalar; });
    return matrix;
}

using matrix4x4f = matrix4x4<float>;
using matrix4x4d = matrix4x4<double>;

} // namespace g::common
