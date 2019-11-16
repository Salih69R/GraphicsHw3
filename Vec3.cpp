#include <iostream>
#include <cmath>

#include "Vec3.h"

Vec3::Vec3(const float &x, const float &y, const float &z) :
    _x(x),
    _y(y),
    _z(z)
{

}

Vec3::Vec3(const Vec3 &other) :
    _x(other._x),
    _y(other._y),
    _z(other._z)
{

}

Vec3 &Vec3::operator+=(const Vec3 &other)
{
    _x += other._x;
    _y += other._y;
    _z += other._z;

    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other)
{
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;

    return *this;
}

Vec3 &Vec3::operator*=(const float &scalar)
{
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;

    return *this;
}

Vec3 &Vec3::operator/=(const float &scalar)
{
    if(scalar == 0.0f)
    {
        std::cerr << "Vec3: cannot divide vector with zero" << std::endl;
        return *this;
    }

    _x /= scalar;
    _y /= scalar;
    _z /= scalar;

    return *this;
}

Vec3 &Vec3::scale(const float &x_scale, const float &y_scale, const float &z_scale)
{
    _x *= x_scale;
    _y *= y_scale;
    _z *= z_scale;

    return *this;
}

Vec3 &Vec3::transalte(const float &x_translate, const float &y_translate, const float &z_translate)
{
    _x += x_translate;
    _y += y_translate;
    _z += z_translate;

    return *this;
}

Vec3 Vec3::elementMultiply(const Vec3 &other)
{
    return Vec3(_x * other._x, _y * other._y, _z * other._z);
}

Vec3 Vec3::elementDevide(const Vec3 &other)
{
    if(other._x == 0.0f || other._y == 0.0f || other._z == 0.0f)
    {
        std::cerr << "Vec3: one of the elements is zero and cannot be divided" << std::endl;
        return Vec3();
    }

    return Vec3(_x / other._x, _y / other._y, _z / other._z);
}

float Vec3::operator*(const Vec3 &other)
{
    return _x * other._x + _y * other._y + _z * other._z;
}

Vec3 Vec3::operator+(const Vec3 &other)
{
    return Vec3(_x + other._x, _y + other._y, _z + other._z);
}

Vec3 Vec3::operator-(const Vec3 &other)
{
    return Vec3(_x - other._x, _y - other._y, _z - other._z);
}

Vec3 operator*(const float &scalar, const Vec3 &other)
{
    return Vec3(other._x * scalar, other._y* scalar, other._z * scalar);
}

Vec3 Vec3::operator*(const float &scalar)
{
    return Vec3(_x * scalar, _y* scalar, _z * scalar);
}

Vec3 Vec3::cross(const Vec3 &other)
{
    return Vec3(_y * other._z - other._y * _z, other._x * _z - _x * other._z, _x * other._y - other._x * _y);
}

float Vec3::norm()
{
    return sqrtf(powf(_x, 2) + powf(_y, 2) + powf(_z, 2));
}

Vec3 &Vec3::normalize()
{
    *this /= norm();

    return *this;
}
