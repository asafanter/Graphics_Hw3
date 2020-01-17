#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

#include "Matrix.h"

template<typename T, uint elements>
class Vector : public Matrix<T, elements, 1>
{
public:
    Vector() = default;
    Vector(const Matrix<T, elements, 1> & other);

    template<typename ...Args>
    explicit Vector(const T &first, Args... tail);

    template<typename ...Args>
    Vector &scale(const T &first, Args... tail);

    template<typename ...Args>
    Vector &translate(const T &first, Args... tail);

	Vector<T, 4> toHomogeneous() const;
    T &operator()(const uint &index);
    const T &operator()(const uint &index) const;
    T dot(const Vector &other) const;
    T norm() const;
	Vector multiplyElements(const Vector &other);
    Vector &normalize();
    Vector cross(const Vector &other) const;
    Vector operator+(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    Vector operator-() const;
	Vector operator*(const T& scalar) const;
	bool operator==(const Vector &other) const;

private: //methods
    template<typename ...Args>
    void scaleElements(const T& first, Args... tail);
    void scaleElements(const T &first);

    template<typename ...Args>
    void translateElements(const T& first, Args... tail);
    void translateElements(const T &first);
};





template<typename T, uint elements>
template<typename ...Args>
Vector<T, elements>::Vector(const T &first, Args... tail)
    : Matrix<T, elements, 1>(first, tail...)
{

}

template<typename T, uint elements>
Vector<T, elements>::Vector(const Matrix<T, elements, 1> &other) :
    Matrix<T, elements, 1> (other)
{

}

template<typename T, uint elements>
T &Vector<T, elements>::operator()(const uint &index)
{
    return this->_data[index][0];
}

template<typename T, uint elements>
const T &Vector<T, elements>::operator()(const uint &index) const
{
    return this->_data[index][0];
}

template<typename T, uint elements>
T Vector<T, elements>::dot(const Vector &other) const
{
    T res = 0;

    for(uint i = 0; i < elements; i++)
    {
        res += this->_data[i][0] * other(i);
    }

    return res;
}

template<typename T, uint elements>
T Vector<T, elements>::norm() const
{
    T res = 0;

    for(uint i = 0; i < elements; i++)
    {
        res += pow(this->_data[i][0], 2);
    }

    return sqrt(res);
}

template<typename T, uint elements>
Vector<T, elements> Vector<T, elements>::multiplyElements(const Vector &other)
{
	Vector<T, elements> res;

	for (int i = 0; i < elements; i++)
	{
		res(i) = this->_data[i][0] * other(i);
	}

	return res;
}

template<typename T, uint elements>
Vector<T, elements> Vector<T, elements>::operator*(const T &scalar) const
{
	auto res = this->Matrix<T, elements, 1>::operator*(scalar);

	return Vector(res);
}

template<typename T, uint elements>
Vector<T, elements> Vector<T, elements>::operator+(const Vector &other) const
{
    auto res = this->Matrix<T, elements, 1>::operator+(other);

    return Vector(res);
}

template<typename T, uint elements>
Vector<T, elements> Vector<T, elements>::operator-(const Vector &other) const
{
    auto res = this->Matrix<T, elements, 1>::operator-(other);

    return Vector(res);
}

template<typename T, uint elements>
Vector<T, elements> Vector<T, elements>::operator-() const
{
    auto res = this->Matrix<T, elements, 1>::operator-();

    return Vector(res);
}

template<typename T, uint elements>
void Vector<T, elements>::scaleElements(const T &first)
{
    this->_data[elements - 1][0] *= first;
}

template<typename T, uint elements>
void Vector<T, elements>::translateElements(const T &first)
{
    this->_data[elements - 1][0] += first;
}

template<typename T, uint elements>
Vector<T, elements> Vector<T, elements>::cross(const Vector &other) const
{
    if(elements != 3)
    {
        std::cerr << "Vector: cross product must contain vector with 3 elemetns not " << elements << std::endl;
        return Vector();
    }

    return Vector(this->_data[1][0] * other(2) - other(1) * this->_data[2][0],
                  other(0) * this->_data[2][0] - this->_data[0][0] * other(2),
                  this->_data[0][0] * other(1) - other(0) * this->_data[1][0]);
}

template<typename T, uint elements>
Vector<T, elements> &Vector<T, elements>::normalize()
{
    *this /= norm();

    return *this;
}

template<typename T, uint elements>
template<typename ...Args>
Vector<T, elements> &Vector<T, elements>::scale(const T &first, Args... tail)
{
    if(sizeof... (tail) + 1 == 1)
    {
        *this *= first;
        return *this;
    }

    if(sizeof... (tail) + 1 != elements)
    {
        std::cerr << "Vector: number of scale elements must be same as number of vector elements" << std::endl;
        return *this;
    }

    scaleElements(first, tail...);

    return *this;
}

template<typename T, uint elements>
template<typename ...Args>
void Vector<T, elements>::scaleElements(const T &first, Args... tail)
{
    uint curr_element = elements - sizeof... (tail) - 1;

    this->_data[curr_element][0] *= first;
    scaleElements(tail...);
}

template<typename T, uint elements>
template<typename ...Args>
Vector<T, elements> &Vector<T, elements>::translate(const T &first, Args... tail)
{
    if(sizeof... (tail) + 1 != elements)
    {
        std::cerr << "Vector: number of translate elements must be same as number of vector elements" << std::endl;
        return *this;
    }

   translateElements(first, tail...);

    return *this;
}

template<typename T, uint elements>
template<typename ...Args>
void Vector<T, elements>::translateElements(const T &first, Args... tail)
{
    uint curr_element = elements - sizeof... (tail) - 1;

    this->_data[curr_element][0] += first;
    translateElements(tail...);
}

template<typename T, uint elements>
bool Vector<T, elements>::operator==(const Vector &other) const
{
	for (uint i = 0; i < elements; i++)
	{
		if (this->_data[i][0] != other(i))
		{
			return false;
		}
	}

	return true;
}

template<typename T, uint elements>
Vector<T, 4> Vector<T, elements>::toHomogeneous() const
{
	return Vec4d(_data[0][0], _data[1][0], _data[2][0], static_cast<T>(1.0));
}

#endif // VECTOR_H


