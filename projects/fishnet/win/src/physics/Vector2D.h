#ifndef PHYS_VECTOR2D_H_
#define PHYS_VECTOR2D_H_

#include <cmath>

namespace phys
{

template <typename T = float>
class Vector2D
{
public:
    Vector2D();
    Vector2D(T x, T y);
    explicit Vector2D(const T* data);
    Vector2D(const Vector2D& v);

public:
    inline T x() const;
    inline T y() const;

    T magnitude() const;
    T magnitude2() const;

    Vector2D<T> normalized() const;

    inline void set(T x, T y);
    inline void set(const Vector2D& v);

    Vector2D& operator=(const Vector2D& v);

    inline Vector2D&    operator+=(const Vector2D& v);
    inline Vector2D&    operator-=(const Vector2D& v);
    inline Vector2D&    operator*=(T d);
    inline Vector2D&    operator/=(T d);

    friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
    {
        Vector2D<T> res(v1);
        res += v2;
        return res;
    }

    friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
    {
        Vector2D<T> res(v1);
        res -= v2;
        return res;
    }

    friend Vector2D operator/(const Vector2D& v, T d)
    {
        Vector2D<T> res(v);
        res /= d;
        return res;
    }

    friend Vector2D operator*(const Vector2D& v, T d)
    {
        Vector2D<T> res(v);
        res *= d;
        return res;
    }

    friend Vector2D operator*(T d, const Vector2D& v) { return v * d; }
    friend T operator*(const Vector2D& v1, const Vector2D& v2) { return v1.m_data[0] * v2.m_data[0] + v1.m_data[1] * v2.m_data[1]; }
    friend Vector2D operator-(const Vector2D& v) { return Vector2D<T>(-v.m_data[0], -v.m_data[1]); }

private:
    void swap(Vector2D& v);

private:
    T   m_data[2];
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename T>
Vector2D<T>::Vector2D()
{
    m_data[0] = m_data[1] = 0;
}

template <typename T>
Vector2D<T>::Vector2D(T x, T y)
{
    m_data[0] = x; m_data[1] = y;
}

template <typename T>
Vector2D<T>::Vector2D(const T* data)
{
    m_data[0] = data[0]; m_data[1] = data[1];
}

template <typename T>
Vector2D<T>::Vector2D(const Vector2D<T>& v)
{
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1];
}


template <typename T>
inline T Vector2D<T>::x() const
{
    return m_data[0];
}

template <typename T>
inline T Vector2D<T>::y() const
{
    return m_data[1];
}


template <typename T>
T Vector2D<T>::magnitude() const
{
    return std::sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1]);
}

template <typename T>
T Vector2D<T>::magnitude2() const
{
    return m_data[0] * m_data[0] + m_data[1] * m_data[1];
}


template <typename T>
Vector2D<T> Vector2D<T>::normalized() const
{
    const T length = magnitude2();

    if (!length)
        return *this;

    return *this / std::sqrt(length);
}


template <typename T>
inline void Vector2D<T>::set(T x, T y)
{
    m_data[0] = x; m_data[1] = y;
}

template <typename T>
inline void Vector2D<T>::set(const Vector2D<T>& v)
{
    set(v.x(), v.y());
}


template <typename T>
inline Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& v)
{
    m_data[0] += v.m_data[0]; m_data[1] += v.m_data[1];
    return *this;
}

template <typename T>
inline Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& v)
{
    m_data[0] -= v.m_data[0]; m_data[1] -= v.m_data[1];
    return *this;
}

template <typename T>
inline Vector2D<T>& Vector2D<T>::operator/=(T d)
{
    m_data[0] /= d; m_data[1] /= d;
    return *this;
}

template <typename T>
inline Vector2D<T>& Vector2D<T>::operator*=(T d)
{
    m_data[0] *= d; m_data[1] *= d;
    return *this;
}

// ------------------------------------------------------

//template <typename T>
//Vector2D<T> operator+(const Vector2D<T>& v1, const Vector2D<T>& v2)
//{
//    Vector2D<T> res(v1);
//    res += v2;
//    return res;
//}
//
//template <typename T>
//Vector2D<T> operator-(const Vector2D<T>& v1, const Vector2D<T>& v2)
//{
//    Vector2D<T> res(v1);
//    res -= v2;
//    return res;
//}
//
//template <typename T>
//Vector2D<T> operator/(const Vector2D<T>& v, T d)
//{
//    Vector2D<T> res(v);
//    res /= d;
//    return res;
//}
//
//template <typename T>
//Vector2D<T> operator*(const Vector2D<T>& v, T d)
//{
//    Vector2D<T><T> res(v);
//    res *= d;
//    return res;
//}
//
//template <typename T>
//Vector2D<T> operator*(T d, const Vector2D<T>& v)
//{
//    return v * d;
//}
//
//template <typename T>
//T operator*(const Vector2D<T>& v1, const Vector2D<T>& v2)
//{
//    return v1.m_data[0] * v2.m_data[0] + v1.m_data[1] * v2.m_data[1];
//}
//
//
//template <typename T>
//Vector2D<T> operator-(const Vector2D<T>& v)
//{
//    return Vector2D<T>(-v.m_data[0], -v.m_data[1]);
//}

// ------------------------------------------------------

template <typename T>
Vector2D<T>& Vector2D<T>::operator=(const Vector2D<T>& v)
{
    if (&v != this)
        Vector2D(v).swap(*this);
    return *this;
}

template <typename T>
void Vector2D<T>::swap(Vector2D<T>& v)
{
    T x = m_data[0]; T y = m_data[1];
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1];
    v.m_data[0] = x; v.m_data[1] = y;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef Vector2D<int>   Vector2DInt;
typedef Vector2D<float> Vector2DFloat;

} // namespace phys

#endif // PHYS_VECTOR2D_H_