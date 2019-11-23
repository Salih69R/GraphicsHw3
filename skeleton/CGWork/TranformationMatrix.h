#ifndef TRANFORMATIONMATRIX_H
#define TRANFORMATIONMATRIX_H

#include "Matrix.h"
#include "Vector.h"

static constexpr uint TRANSFORMATION_SIZE = 4;
static constexpr uint ROTATION_SIZE = 3;
static constexpr uint TRANSLATION_SIZE = 3;

template<typename T>
class TransformationMatrix : public Matrix<T, TRANSFORMATION_SIZE, TRANSFORMATION_SIZE>
{
public:
    template<typename ...Args>
    explicit TransformationMatrix(const T &first, Args... tail);

    TransformationMatrix();
    TransformationMatrix(const Matrix<T, ROTATION_SIZE, ROTATION_SIZE> &rotation,
                         Vector<T, TRANSLATION_SIZE> &translation);

    TransformationMatrix &rotateX(const T &angle_deg);
    TransformationMatrix &rotateY(const T &angle_deg);
    TransformationMatrix &rotateZ(const T &angle_deg);
    TransformationMatrix operator+(const TransformationMatrix &other);
    TransformationMatrix operator-(const TransformationMatrix &other);
    TransformationMatrix operator*(const TransformationMatrix &other);

    Vector<T, TRANSLATION_SIZE> getTranslation() const;
    Matrix<T, ROTATION_SIZE, ROTATION_SIZE> getRotation() const;
    TransformationMatrix &setTranslation(const Vector<T, TRANSLATION_SIZE> &new_translation);
    TransformationMatrix &setRotation(const Matrix<T, ROTATION_SIZE, ROTATION_SIZE> &new_rotation);
};





template<typename T>
TransformationMatrix<T>::TransformationMatrix() :
    Matrix<T, TRANSFORMATION_SIZE, TRANSFORMATION_SIZE> ()
{
    for(uint i = 0; i < TRANSFORMATION_SIZE; i++)
    {
        this->_data[i][i] = 1;
    }
}

template<typename T>
TransformationMatrix<T>::TransformationMatrix(const Matrix<T, ROTATION_SIZE, ROTATION_SIZE> &rotation,
                                              Vector<T, TRANSLATION_SIZE> &translation) :
    Matrix<T, TRANSFORMATION_SIZE, TRANSFORMATION_SIZE> ()
{
    for(uint row = 0; row < ROTATION_SIZE; row++)
    {
        for(uint col = 0; col < ROTATION_SIZE; col++)
        {
            this->_data[row][col] = rotation(row, col);
        }
    }

    for(uint i = 0; i < TRANSLATION_SIZE; i++)
    {
        this->_data[i][TRANSLATION_SIZE] = translation(i);
    }

    this->_data[TRANSFORMATION_SIZE - 1][TRANSFORMATION_SIZE - 1] = 1;
}

template<typename T>
TransformationMatrix<T> TransformationMatrix<T>::operator+(const TransformationMatrix &other)
{
    TransformationMatrix res;

    for(uint row = 0; row < TRANSFORMATION_SIZE; row++)
    {
        for(uint col = 0; col < TRANSFORMATION_SIZE; col++)
        {
            res._data[row][col] = (*this)(row, col) + other(row, col);
        }
    }

    return res;
}

template<typename T>
TransformationMatrix<T> TransformationMatrix<T>::operator-(const TransformationMatrix &other)
{
    TransformationMatrix res;

    for(uint row = 0; row < TRANSFORMATION_SIZE; row++)
    {
        for(uint col = 0; col < TRANSFORMATION_SIZE; col++)
        {
            res._data[row][col] = (*this)(row, col) - other(row, col);
        }
    }

    return res;
}

template<typename T>
TransformationMatrix<T> TransformationMatrix<T>::operator*(const TransformationMatrix &other)
{
    TransformationMatrix res;

    for (uint row = 0; row < TRANSFORMATION_SIZE; row++ )
    {
        for(uint col = 0; col < TRANSFORMATION_SIZE; col++)
        {
            for(uint i = 0; i < TRANSFORMATION_SIZE; i++)
            {
                res(row,col) += (*this)(row, i) * other(i, col);
            }
        }
    }

    return res;
}

template<typename T>
template<typename ...Args>
TransformationMatrix<T>::TransformationMatrix(const T &first, Args... tail) :
    Matrix<T, TRANSFORMATION_SIZE, TRANSFORMATION_SIZE>(first, tail...)
{

}

//template<typename T>
//TransformationMatrix<T> &TransformationMatrix<T>::rotateX(const T &angle_deg)
//{
//    T angle_rad = angle_deg * M_PI / 180.0;
//    auto m = *this;

//    auto asaf = m * TransformationMatrix(1, 0, 1, 0,
//                                0, cos(angle_rad), sin(angle_rad), 0,
//                                0, -sin(angle_rad), cos(angle_rad), 0,
//                                0, 0, 0 ,1);

//    *this = asaf;
//    return *this;
//}

//template<typename T>
//TransformationMatrix<T> &TransformationMatrix<T>::rotateY(const T &angle_deg)
//{
//    T angle_rad = angle_deg * M_PI / 180.0;

//    *this = *this * TransformationMatrix(cos(angle_rad), 0, -sin(angle_rad), 0,
//                                0, 1, 0, 0,
//                                sin(angle_rad), 0, cos(angle_rad), 0,
//                                0, 0, 0 ,1);

//    return *this;
//}

//template<typename T>
//TransformationMatrix<T> &TransformationMatrix<T>::rotateZ(const T &angle_deg)
//{
//    T angle_rad = angle_deg * M_PI / 180.0;

//    *this = *this * TransformationMatrix(cos(angle_rad), sin(angle_rad), 0, 0,
//                                -sin(angle_rad), cos(angle_rad), 0, 0,
//                                0, 0, 1, 0,
//                                0, 0, 0 ,1);

//    return *this;
//}

template<typename T>
Matrix<T, ROTATION_SIZE, ROTATION_SIZE> TransformationMatrix<T>::getRotation() const
{
    Matrix<T, ROTATION_SIZE, ROTATION_SIZE> res;

    for(uint row = 0; row < ROTATION_SIZE; row++)
    {
        for(uint col = 0; col < ROTATION_SIZE; col++)
        {
            res(row, col) = this->_data[row][col];
        }
    }

    return res;
}

template<typename T>
TransformationMatrix<T> &TransformationMatrix<T>::setRotation(const Matrix<T, ROTATION_SIZE, ROTATION_SIZE> &new_rotation)
{
    for(uint row = 0; row < ROTATION_SIZE; row++)
    {
        for(uint col = 0; col < ROTATION_SIZE; col++)
        {
            this->_data[row][col] = new_rotation(row, col);
        }

    }

    return *this;
}

template<typename T>
TransformationMatrix<T> &TransformationMatrix<T>::setTranslation(const Vector<T, TRANSLATION_SIZE> &new_translation)
{
    for(uint i = 0; i < TRANSLATION_SIZE; i++)
    {
        this->_data[i][TRANSLATION_SIZE] = new_translation(i);
    }

    return *this;
}

template<typename T>
Vector<T, TRANSLATION_SIZE> TransformationMatrix<T>::getTranslation() const
{
    return Vector<T, TRANSLATION_SIZE>(this->_data[0][TRANSLATION_SIZE],
                                       this->_data[1][TRANSLATION_SIZE],
                                       this->_data[2][TRANSLATION_SIZE]);
}




#endif // TRANFORMATIONMATRIX_H
