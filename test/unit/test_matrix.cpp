#include <QMatrix4x4>
#include <QRandomGenerator>

#include "common/matrix.hpp"
#include "common/quaternion.hpp"
#include <gtest/gtest.h>

using namespace g::common;

template <typename matrix1_type, typename matrix2_type>
void test_matrix_equality(matrix1_type mat1, matrix2_type mat2)
{
    EXPECT_FLOAT_EQ(mat1(0, 0), mat2(0, 0));
    EXPECT_FLOAT_EQ(mat1(0, 1), mat2(0, 1));
    EXPECT_FLOAT_EQ(mat1(0, 2), mat2(0, 2));
    EXPECT_FLOAT_EQ(mat1(0, 3), mat2(0, 3));
    EXPECT_FLOAT_EQ(mat1(1, 0), mat2(1, 0));
    EXPECT_FLOAT_EQ(mat1(1, 1), mat2(1, 1));
    EXPECT_FLOAT_EQ(mat1(1, 2), mat2(1, 2));
    EXPECT_FLOAT_EQ(mat1(1, 3), mat2(1, 3));
    EXPECT_FLOAT_EQ(mat1(2, 0), mat2(2, 0));
    EXPECT_FLOAT_EQ(mat1(2, 1), mat2(2, 1));
    EXPECT_FLOAT_EQ(mat1(2, 2), mat2(2, 2));
    EXPECT_FLOAT_EQ(mat1(2, 3), mat2(2, 3));
    EXPECT_FLOAT_EQ(mat1(3, 0), mat2(3, 0));
    EXPECT_FLOAT_EQ(mat1(3, 1), mat2(3, 1));
    EXPECT_FLOAT_EQ(mat1(3, 2), mat2(3, 2));
    EXPECT_FLOAT_EQ(mat1(3, 3), mat2(3, 3));
}

TEST(matrix, copy)
{
    matrix4x4f matrix;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix(i, j) = QRandomGenerator::global()->generateDouble();
        }
    }

    matrix4x4f copy = matrix;

    test_matrix_equality(copy, matrix);
}

TEST(matrix, zero)
{
    matrix4x4f matrix = matrix4x4f::zero();

    EXPECT_FLOAT_EQ(matrix(0, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(0, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(0, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(0, 3), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 3), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 3), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 3), 0.0f);
}

TEST(matrix, identity)
{
    matrix4x4f matrix = matrix4x4f::identity();

    EXPECT_FLOAT_EQ(matrix(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(matrix(0, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(0, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(0, 3), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 1), 1.0f);
    EXPECT_FLOAT_EQ(matrix(1, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(1, 3), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(2, 2), 1.0f);
    EXPECT_FLOAT_EQ(matrix(2, 3), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 0), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 1), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 2), 0.0f);
    EXPECT_FLOAT_EQ(matrix(3, 3), 1.0f);
}

TEST(matrix, addition)
{
    matrix4x4f matrix1, matrix2;
    QMatrix4x4 qmatrix1, qmatrix2;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix1(i, j) = qmatrix1(i, j) =
                QRandomGenerator::global()->generateDouble();
            matrix2(i, j) = qmatrix2(i, j) =
                QRandomGenerator::global()->generateDouble();
        }
    }

    matrix4x4f result = matrix1 + matrix2;
    QMatrix4x4 qresult = qmatrix1 + qmatrix2;

    test_matrix_equality(result, qresult);
}

TEST(matrix, subtraction)
{
    matrix4x4f matrix1, matrix2;
    QMatrix4x4 qmatrix1, qmatrix2;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix1(i, j) = qmatrix1(i, j) =
                QRandomGenerator::global()->generateDouble();
            matrix2(i, j) = qmatrix2(i, j) =
                QRandomGenerator::global()->generateDouble();
        }
    }

    matrix4x4f result = matrix1 - matrix2;
    QMatrix4x4 qresult = qmatrix1 - qmatrix2;

    test_matrix_equality(result, qresult);
}

TEST(matrix, multiplication)
{
    matrix4x4f matrix1, matrix2;
    QMatrix4x4 qmatrix1, qmatrix2;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix1(i, j) = qmatrix1(i, j) =
                QRandomGenerator::global()->generateDouble();
            matrix2(i, j) = qmatrix2(i, j) =
                QRandomGenerator::global()->generateDouble();
        }
    }

    matrix4x4f result = matrix1 * matrix2;
    QMatrix4x4 qresult = qmatrix1 * qmatrix2;

    test_matrix_equality(result, qresult);
}

TEST(matrix, scalar_addition)
{
    matrix4x4f matrix;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix(i, j) = QRandomGenerator::global()->generateDouble();
        }
    }

    float scalar = QRandomGenerator::global()->generateDouble();

    matrix4x4f result = matrix + scalar;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            EXPECT_FLOAT_EQ(result(i, j), matrix(i, j) + scalar);
        }
    }
}

TEST(matrix, scalar_subtraction)
{
    matrix4x4f matrix;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix(i, j) = QRandomGenerator::global()->generateDouble();
        }
    }

    float scalar = QRandomGenerator::global()->generateDouble();

    matrix4x4f result = matrix - scalar;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            EXPECT_FLOAT_EQ(result(i, j), matrix(i, j) - scalar);
        }
    }
}

TEST(matrix, scalar_multiplication)
{
    matrix4x4f matrix;
    QMatrix4x4 qmatrix;
    float scalar = QRandomGenerator::global()->generateDouble();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix(i, j) = qmatrix(i, j) =
                QRandomGenerator::global()->generateDouble();
        }
    }

    matrix4x4f result = matrix * scalar;
    QMatrix4x4 qresult = qmatrix * scalar;

    test_matrix_equality(result, qresult);
}

TEST(matrix, scalar_division)
{
    matrix4x4f matrix;
    QMatrix4x4 qmatrix;
    float scalar = QRandomGenerator::global()->generateDouble();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix(i, j) = qmatrix(i, j) =
                QRandomGenerator::global()->generateDouble();
        }
    }

    matrix4x4f result = matrix / scalar;
    QMatrix4x4 qresult = qmatrix / scalar;

    test_matrix_equality(result, qresult);
}

TEST(matrix, matrix_vector_multiplication)
{
    matrix4x4f matrix;
    vector3f vector;
    QMatrix4x4 qmatrix;
    QVector3D qvector;
    for (int i = 0; i < 3; ++i)
    {
        vector.as_array()[ i ] = qvector[ i ] =
            QRandomGenerator::global()->generateDouble();
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix(i, j) = qmatrix(i, j) =
                QRandomGenerator::global()->generateDouble();
        }
    }

    vector3f result = matrix * vector.as_array();
    QVector3D qresult = qmatrix.map(qvector);

    EXPECT_FLOAT_EQ(result.as_array()[ 0 ], qresult[ 0 ]);
    EXPECT_FLOAT_EQ(result.as_array()[ 1 ], qresult[ 1 ]);
    EXPECT_FLOAT_EQ(result.as_array()[ 2 ], qresult[ 2 ]);
}

TEST(matrix, from_look)
{
    std::array<float, 3> from, to, up;
    for (int i = 0; i < 3; ++i)
    {
        from[ i ] = QRandomGenerator::global()->generateDouble();
        to[ i ] = QRandomGenerator::global()->generateDouble();
        up[ i ] = QRandomGenerator::global()->generateDouble();
    }
    matrix4x4f matrix =
        matrix4x4f::from_look(vector3f { from[ 0 ], from[ 1 ], from[ 2 ] },
                              vector3f { to[ 0 ], to[ 1 ], to[ 2 ] },
                              vector3f { up[ 0 ], up[ 1 ], up[ 2 ] });
    QMatrix4x4 qmatrix;
    qmatrix.lookAt({ from[ 0 ], from[ 1 ], from[ 2 ] },
                   { to[ 0 ], to[ 1 ], to[ 2 ] },
                   { up[ 0 ], up[ 1 ], up[ 2 ] });

    test_matrix_equality(matrix, qmatrix);
}
