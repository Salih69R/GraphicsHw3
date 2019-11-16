#ifndef MAT3_H
#define MAT3_H

#include <ostream>

#include "Vec3.h"

class Mat3
{
public:
    Mat3(const float &a11 = 0.0f, const float &a12 = 0.0f, const float &a13 = 0.0f,
         const float &a21 = 0.0f, const float &a22 = 0.0f, const float &a23 = 0.0f,
         const float &a31 = 0.0f, const float &a32 = 0.0f, const float &a33 = 0.0f);
    Mat3(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3);
    static Mat3 identity();
    static Mat3 RotatedX(const float &angle_deg);
    static Mat3 RotatedY(const float &angle_deg);
    static Mat3 RotatedZ(const float &angle_deg);
    Mat3 &operator+=(const Mat3 &other);
    Mat3 &operator-=(const Mat3 &other);
    Mat3 &operator*=(const float &scalar);
    Mat3 &operator*=(const Mat3 &other);
    Mat3 operator+(const Mat3 &other);
    Mat3 operator-(const Mat3 &other);
    Mat3 operator*(const Mat3 &other);
    Vec3 operator*(const Vec3 &vec);

    friend std::ostream &operator<<(std::ostream &out, const Mat3 &mat)
    {
        out << mat._v1.getX() << " " << mat._v2.getX() << " " << mat._v3.getX() << std::endl;
        out << mat._v1.getY() << " " << mat._v2.getY() << " " << mat._v3.getY() << std::endl;
        out << mat._v1.getZ() << " " << mat._v2.getZ() << " " << mat._v3.getZ() << std::endl;

        return out;
    }

private:
    Vec3 _v1;
    Vec3 _v2;
    Vec3 _v3;
};

#endif // MAT3_H
