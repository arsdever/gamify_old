#include <QRandomGenerator>
#include <QVector3D>

#include "common/vector.hpp"
#include <gtest/gtest.h>

TEST(vector, copy)
{
    g::common::vector3f vector {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };

    g::common::vector3f copy = vector;

    EXPECT_FLOAT_EQ(copy.x(), vector.x());
    EXPECT_FLOAT_EQ(copy.y(), vector.y());
    EXPECT_FLOAT_EQ(copy.z(), vector.z());
}

TEST(vector, zero)
{
    g::common::vector3f vector = g::common::vector3f::zero();

    EXPECT_FLOAT_EQ(vector.x(), 0.0f);
    EXPECT_FLOAT_EQ(vector.y(), 0.0f);
    EXPECT_FLOAT_EQ(vector.z(), 0.0f);
}

TEST(vector, cross)
{
    g::common::vector3f vector1, vector2;
    QVector3D qvector1, qvector2;

    for (int i = 0; i < 3; ++i)
    {
        vector1[ i ] = qvector1[ i ] =
            static_cast<float>(QRandomGenerator::global()->generateDouble());
        vector1[ i ] = qvector1[ i ] =
            static_cast<float>(QRandomGenerator::global()->generateDouble());
        vector1[ i ] = qvector1[ i ] =
            static_cast<float>(QRandomGenerator::global()->generateDouble());
        vector2[ i ] = qvector2[ i ] =
            static_cast<float>(QRandomGenerator::global()->generateDouble());
        vector2[ i ] = qvector2[ i ] =
            static_cast<float>(QRandomGenerator::global()->generateDouble());
        vector2[ i ] = qvector2[ i ] =
            static_cast<float>(QRandomGenerator::global()->generateDouble());
    };

    g::common::vector3f result = vector1.cross(vector2);
    QVector3D qresult = QVector3D::crossProduct(qvector1, qvector2);

    EXPECT_FLOAT_EQ(result.x(), qresult.x());
    EXPECT_FLOAT_EQ(result.y(), qresult.y());
    EXPECT_FLOAT_EQ(result.z(), qresult.z());
}

TEST(vector, negate)
{
    g::common::vector3f vector {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };

    auto neg_vector = -vector;

    EXPECT_FLOAT_EQ(neg_vector.x(), -vector.x());
    EXPECT_FLOAT_EQ(neg_vector.y(), -vector.y());
    EXPECT_FLOAT_EQ(neg_vector.z(), -vector.z());
}

TEST(vector, add)
{
    g::common::vector3f vector1 {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };
    g::common::vector3f vector2 {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };

    auto result = vector1 + vector2;

    EXPECT_FLOAT_EQ(result.x(), vector1.x() + vector2.x());
    EXPECT_FLOAT_EQ(result.y(), vector1.y() + vector2.y());
    EXPECT_FLOAT_EQ(result.z(), vector1.z() + vector2.z());
}

TEST(vector, subtract)
{
    g::common::vector3f vector1 {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };
    g::common::vector3f vector2 {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };

    auto result = vector1 - vector2;

    EXPECT_FLOAT_EQ(result.x(), vector1.x() - vector2.x());
    EXPECT_FLOAT_EQ(result.y(), vector1.y() - vector2.y());
    EXPECT_FLOAT_EQ(result.z(), vector1.z() - vector2.z());
}

TEST(vector, multiply)
{
    g::common::vector3f vector {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };
    float scalar =
        static_cast<float>(QRandomGenerator::global()->generateDouble());

    auto result = vector * scalar;

    EXPECT_FLOAT_EQ(result.x(), vector.x() * scalar);
    EXPECT_FLOAT_EQ(result.y(), vector.y() * scalar);
    EXPECT_FLOAT_EQ(result.z(), vector.z() * scalar);
}

TEST(vector, divide)
{
    g::common::vector3f vector {
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble()),
        static_cast<float>(QRandomGenerator::global()->generateDouble())
    };
    float scalar =
        static_cast<float>(QRandomGenerator::global()->generateDouble());

    auto result = vector / scalar;

    EXPECT_FLOAT_EQ(result.x(), vector.x() / scalar);
    EXPECT_FLOAT_EQ(result.y(), vector.y() / scalar);
    EXPECT_FLOAT_EQ(result.z(), vector.z() / scalar);
}

TEST(vector, dot)
{
    g::common::vector3f vector1 { 1, 2, 3 };
    g::common::vector3f vector2 { 4, 5, 6 };

    auto result = vector1.dot(vector2);

    EXPECT_FLOAT_EQ(result, 32.0f);
}

TEST(vector, length)
{
    g::common::vector3f vector { 1, 2, 3 };

    auto result = vector.length();

    EXPECT_FLOAT_EQ(result, std::sqrt(14.0f));
}

TEST(vector, normalize)
{
    g::common::vector3f vector { 1, 2, 3 };

    auto result = vector.normalized();

    EXPECT_FLOAT_EQ(result.x(), 1.0f / std::sqrt(14.0f));
    EXPECT_FLOAT_EQ(result.y(), 2.0f / std::sqrt(14.0f));
    EXPECT_FLOAT_EQ(result.z(), 3.0f / std::sqrt(14.0f));
    EXPECT_FLOAT_EQ(result.length(), 1.0f);
}

TEST(vector, as_vector)
{
    g::common::vector3f vector { 1, 2, 3 };
    g::common::vector3d result = vector.as_vector<double>();

    EXPECT_FLOAT_EQ(result.x(), 1.0);
    EXPECT_FLOAT_EQ(result.y(), 2.0);
    EXPECT_FLOAT_EQ(result.z(), 3.0);
}
