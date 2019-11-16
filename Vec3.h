#ifndef VEC3_H
#define VEC3_H


class Vec3
{
public:
    Vec3(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f);
    Vec3(const Vec3 &other);
    Vec3 &operator+=(const Vec3 &other);
    Vec3 &operator-=(const Vec3 &other);
    Vec3 &operator*=(const float &scalar);
    Vec3 &operator/=(const float &scalar);
    float operator*(const Vec3 &other);
    Vec3 operator*(const float &scalar);
    friend Vec3 operator*(const float &scalar, const Vec3 &other);
    Vec3 operator+(const Vec3 &other);
    Vec3 operator-(const Vec3 &other);
    Vec3 &scale(const float &x_scale, const float &y_scale, const float &z_scale);
    Vec3 &transalte(const float &x_translate, const float &y_translate, const float &z_translate);
    Vec3 elementMultiply(const Vec3 &other);
    Vec3 elementDevide(const Vec3 &other);
    Vec3 cross(const Vec3 &other);
    float norm();
    Vec3 &normalize();
    float getX() const {return _x;}
    float getY() const {return _y;}
    float getZ() const {return _z;}

private:
    float _x;
    float _y;
    float _z;
};

#endif // VEC3_H
