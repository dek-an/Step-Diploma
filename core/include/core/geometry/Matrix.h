#ifndef GEOMETRY_MATRIX_H_
#define GEOMETRY_MATRIX_H_

namespace core
{

template <typename T = float>
class Matrix
{
public:
    Matrix(int rows, int cols, T* data = 0)
        : m_rows(rows)
        , m_cols(cols)
        , m_dataOwner(false)
        , m_data(data)
    {
        if (!data)
            m_data = new T[rows * cols];
        m_dataOwner = true;
    }

    ~Matrix()
    {
        if (m_dataOwner)
            delete[] m_data;
    }

public:
    int rows() const
    {
        return m_rows;
    }

    int cols() const
    {
        return m_cols;
    }

    void setZero()
    {
        memset(m_data, 0, m_rows * m_cols);
    }

public:
    T operator()(int row, int col) const
    {
        return operator()(row, col);
    }

    T& operator()(int row, int col)
    {
        return *(m_data + row * m_cols + col);
    }

private:
    Matrix(const Matrix&);
    Matrix& operator=(const Matrix&);

private:
    int     m_rows;
    int     m_cols;
    bool    m_dataOwner;
    T*      m_data;
};

} // namespace core

#endif // GEOMETRY_MATRIX_H_