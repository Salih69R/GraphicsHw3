#include <cmath>

#include "Mat3.h"

Mat3::Mat3(const float &a11, const float &a12, const float &a13,
           const float &a21, const float &a22, const float &a23,
           const float &a31, const float &a32, const float &a33) :
    _v1(a11, a21, a31),
    _v2(a12, a22, a32),
    _v3(a13, a23, a33)
{

}

Mat3::Mat3(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3) :
    _v1(v1),
    _v2(v2),
    _v3(v3)
{

}

Mat3 Mat3::identity()
{
    return Mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat3 Mat3::RotatedX(const float &angle_deg)
{
    float angle_rad = angle_deg * static_cast<float>(M_PI) / 180.0f;
    float cos_angle = cosf(angle_rad);
    float sin_angle = sinf(angle_rad);

    return Mat3(1.0f, 0.0f, 0.0f,
                0.0f, cos_angle, sin_angle,
                0.0f, -sin_angle, cos_angle);
}

Mat3 Mat3::RotatedY(const float &angle_deg)
{
    float angle_rad = angle_deg * static_cast<float>(M_PI) / 180.0f;
    float cos_angle = cosf(angle_rad);
    float sin_angle = sinf(angle_rad);

    return Mat3(cos_angle, 0.0f, -sin_angle,
                0.0f, 1.0f, 0.0f,
                sin_angle, 0.0f, cos_angle);
}

Mat3 Mat3::RotatedZ(const float &angle_deg)
{
    float angle_rad = angle_deg * static_cast<float>(M_PI) / 180.0f;
    float cos_angle = cosf(angle_rad);
    float sin_angle = sinf(angle_rad);

    return Mat3(cos_angle, sin_angle, 0.0f,
                -sin_angle, cos_angle, 0.0f,
                0.0f, 0.0f, 1.0f);
}

Mat3 &Mat3::operator+=(const Mat3 &other)
{
    _v1 += other._v1;
    _v2 += other._v2;
    _v3 += other._v3;

    return *this;
}

Mat3 &Mat3::operator-=(const Mat3 &other)
{
    _v1 -= other._v1;
    _v2 -= other._v2;
    _v3 -= other._v3;

    return *this;
}

Mat3 &Mat3::operator*=(const float &scalar)
{
    _v1 *= scalar;
    _v2 *= scalar;
    _v3 *= scalar;

    return *this;
}

Mat3 &Mat3::operator*=(const Mat3 &other)
{
    Vec3 row1(_v1.getX(), _v2.getX(), _v3.getX());
    Vec3 row2(_v1.getY(), _v2.getY(), _v3.getY());
    Vec3 row3(_v1.getZ(), _v2.getZ(), _v3.getZ());

    *this = Mat3(row1 * other._v1, row1 * other._v2, row1 * other._v3,
                 row2 * other._v1, row2 * other._v2, row2 * other._v3,
                 row3 * other._v1, row3 * other._v2, row3 * other._v3);

    return *this;

}

Mat3 Mat3::operator+(const Mat3 &other)
{
    return Mat3(_v1 + other._v1, _v2 + other._v2, _v3 + other._v3);
}

Mat3 Mat3::operator-(const Mat3 &other)
{
    return Mat3(_v1 - other._v1, _v2 - other._v2, _v3 - other._v3);
}

Mat3 Mat3::operator*(const Mat3 &other)
{
    Vec3 row1(_v1.getX(), _v2.getX(), _v3.getX());
    Vec3 row2(_v1.getY(), _v2.getY(), _v3.getY());
    Vec3 row3(_v1.getZ(), _v2.getZ(), _v3.getZ());

    return Mat3(row1 * other._v1, row1 * other._v2, row1 * other._v3,
                row2 * other._v1, row2 * other._v2, row2 * other._v3,
                row3 * other._v1, row3 * other._v2, row3 * other._v3);
}

Vec3 Mat3::operator*(const Vec3 &vec)
{
    Vec3 row1(_v1.getX(), _v2.getX(), _v3.getX());
    Vec3 row2(_v1.getY(), _v2.getY(), _v3.getY());
    Vec3 row3(_v1.getZ(), _v2.getZ(), _v3.getZ());

    return Vec3(row1 * vec, row2 * vec, row3 * vec);
}
