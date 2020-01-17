#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <memory>

using uint = unsigned int;

template<typename T, uint rows, uint cols>
class Matrix
{
public:
    Matrix();

    template<typename ...Args>
    explicit Matrix(const T &first, Args... tail);

    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator*=(const T &scalar);
    Matrix &operator/=(const T &scalar);
    T &operator()(const uint &row, const uint &col);
    const T &operator()(const uint &row, const uint &col) const;
    Matrix transpose() const;
    uint getNumOfRows() const {return rows;}
    uint getNumOfCols() const {return cols;}

    template<uint num_of_cols>
    Matrix<T, rows, num_of_cols> operator*(const Matrix<T, cols, num_of_cols> &other) const;
    Matrix<T, rows, cols> operator*(const T &scalar) const;
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator-() const;

    void print();

    virtual ~Matrix() = default;

private: //methods
    void init();
    void fillElements(const T &first);

    template<typename ...Args>
    void fillElements(const T &first, Args... tail);


protected: //members
    std::vector<std::vector<T>> _data;
};










template<typename T,uint rows, uint cols>
Matrix<T, rows, cols>::Matrix() :
    _data()
{
    init();
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> &Matrix<T, rows, cols>::operator+=(const Matrix<T, rows, cols> &other)
{
    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            _data[row][col] += other._data[row][col];
        }
    }

    return *this;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> &Matrix<T, rows, cols>::operator-=(const Matrix<T, rows, cols> &other)
{
    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            _data[row][col] -= other._data[row][col];
        }
    }

    return *this;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> &Matrix<T, rows, cols>::operator*=(const T &scalar)
{
    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            _data[row][col] *= scalar;
        }
    }

    return *this;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> &Matrix<T, rows, cols>::operator/=(const T &scalar)
{
    if(scalar == 0)
    {
        std::cerr << "Matrix: cannot devide with zero" << std::endl;
    }

    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            _data[row][col] /= scalar;
        }
    }

    return *this;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator+(const Matrix<T, rows, cols> &other) const
{
    Matrix<T, rows, cols> res;

    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            res._data[row][col] = _data[row][col] + other._data[row][col];
        }
    }

    return res;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator-(const Matrix<T, rows, cols> &other) const
{
    Matrix<T, rows, cols> res;

    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            res._data[row][col] = _data[row][col] - other._data[row][col];
        }
    }

    return res;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator-() const
{
    Matrix<T, rows, cols> res;

    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            res._data[row][col] = -1 * (*this)(row, col);
        }
    }

    return res;
}

template<typename T, uint rows, uint cols>
T &Matrix<T, rows, cols>::operator()(const uint &row, const uint &col)
{
    if(row >= rows || col >= cols)
    {
        std::cerr << "Matrix: invalid index" << std::endl;
        return _data[0][0];
    }

    return _data[row][col];
}

template<typename T, uint rows, uint cols>
const T &Matrix<T, rows, cols>::operator()(const uint &row, const uint &col) const
{
    if(row >= rows || col >= cols)
    {
        std::cerr << "Matrix: invalid index" << std::endl;
        return _data[0][0];
    }

    return _data[row][col];
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator*(const T &scalar) const
{
    Matrix<T, rows, cols> res;

    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            res(row, col) = (*this)(row, col) * scalar;
        }
    }

    return res;
}

template<typename T, uint rows, uint cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::transpose() const
{
    Matrix<T, rows, cols> res;

    for(uint row = 0; row < rows; row++)
    {
        for(uint col = 0; col < cols; col++)
        {
            res(row, col) = (*this)(col, row);
        }
    }

    return res;
}

template<typename T, uint rows, uint cols>
void Matrix<T, rows,cols>::print()
{
    for(auto &row : _data)
    {
        for(auto &col : row)
        {
            std::cout << col << " ";
        }

        std::cout << std::endl;
    }
}

template<typename T, uint rows, uint cols>
void Matrix<T, rows, cols>::init()
{
    _data.resize(rows);

    for(auto &row :_data)
    {
        row.resize(cols);
    }
}

template<typename T, uint rows, uint cols>
void Matrix<T, rows, cols>::fillElements(const T &first)
{
    _data[rows - 1][cols - 1] = first;
}

template<typename T, uint rows, uint cols>
template<typename ...Args>
Matrix<T, rows, cols>::Matrix(const T& first, Args... tail)
{
    if(sizeof... (tail) + 1 != rows * cols)
    {
        std::cout << "Matrix: matrix with a size of " << rows << " X " << cols << " has been initizlized with " <<
                     sizeof... (tail) + 1 << " elements instead of " << rows * cols << " elements" << std::endl;
        return;
    }

    init();
    fillElements(first, tail...);
}

template<typename T, uint rows, uint cols>
template<typename ...Args>
void Matrix<T, rows, cols>::fillElements(const T & first, Args... tail)
{
    uint count = rows * cols - sizeof... (tail) - 1;

    uint row = count / cols;
    uint col = count % cols;

    _data[row][col] = first;
    fillElements(tail...);
}

template<typename T, uint rows, uint cols>
template<uint num_of_cols>
Matrix<T, rows, num_of_cols> Matrix<T, rows, cols>::operator*(const Matrix<T, cols, num_of_cols> &other) const
{
    Matrix<T, rows, num_of_cols> res;

    for (uint row = 0; row < rows; row++ )
    {
        for(uint col = 0; col < num_of_cols; col++)
        {
            for(uint i = 0; i < cols; i++)
            {
                res(row,col) += _data[row][i] * other(i, col);
            }
        }
    }

    return res;
}

#endif // MATRIX_H


