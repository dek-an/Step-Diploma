#ifndef GEOMETRY_MATRIX_H_
#define GEOMETRY_MATRIX_H_

#include <memory>

#include "Vector2D.h"

namespace core
{

template <typename T = float>
class Matrix
{
public:
    Matrix(int rows, int cols, T* data = 0);
    ~Matrix();

public:
    int rows() const;
    int cols() const;
    void setZero();
    void setRowValue(int row, T val);
    void setColValue(int col, T val);

public:
    T operator()(int row, int col) const;
    T operator()(Vector2D<int> pos) const;
    void set(int row, int col, T val);

private:
    Matrix(const Matrix&);
    Matrix& operator=(const Matrix&);

private:
    int     m_rows;
    int     m_cols;
    bool    m_dataOwner;
    T*      m_data;
};

// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T* data/* = 0*/)
    : m_rows(rows)
    , m_cols(cols)
    , m_dataOwner(false)
    , m_data(data)
{
    if (!data)
        m_data = new T[rows * cols];
    m_dataOwner = true;
}

template <typename T>
Matrix<T>::~Matrix()
{
    if (m_dataOwner)
        delete[] m_data;
}


template <typename T>
int Matrix<T>::rows() const
{
    return m_rows;
}

template <typename T>
int Matrix<T>::cols() const
{
    return m_cols;
}

template <typename T>
void Matrix<T>::setZero()
{
    memset(m_data, 0, m_rows * m_cols);
}


template <typename T>
T Matrix<T>::operator()(int row, int col) const
{
    return *(m_data + row * m_cols + col);
}

template <typename T>
T Matrix<T>::operator()(Vector2D<int> pos) const
{
    return this->operator()(pos.x(), pos.y());
}

template <typename T>
void Matrix<T>::set(int row, int col, T val)
{
    *(m_data + row * m_cols + col) = val;
}


template <typename T>
void Matrix<T>::setRowValue(int row, T val)
{
    T* dst = m_data + row * m_cols;
    memset(dst, val, m_cols * sizeof(T));
}

template <typename T>
void Matrix<T>::setColValue(int col, T val)
{
}

// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------

typedef Matrix<float>   MatrixFloat;
typedef Matrix<int>     MatrixInt;

} // namespace core

#endif // GEOMETRY_MATRIX_H_