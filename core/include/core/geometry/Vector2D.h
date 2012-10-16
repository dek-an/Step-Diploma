#ifndef GEOMETRY_VECTOR2D_H_
#define GEOMETRY_VECTOR2D_H_

namespace core
{

template <typename T = float>
class Vector2D
{
public:
    inline Vector2D<T>();
    inline Vector2D<T>(T x, T y);
    inline Vector2D<T>(const T* data);
    inline Vector2D<T>(const Vector2D<T>& v);

public:
    inline T x() const;
    inline T y() const;

    Vector2D& operator=(const Vector2D<T>& v);

    inline Vector2D<T>&    operator+=(const Vector2D& v);
    inline Vector2D<T>&    operator-=(const Vector2D& v);
    inline Vector2D<T>&    operator*=(T d);
    inline Vector2D<T>&    operator/=(T d);

    friend inline Vector2D<T>   operator+(const Vector2D<T>& v1, const Vector2D<T>& v2)
    {
        Vector2D<T> res(v1);
        res += v2;
        return res;
    }
    friend inline Vector2D<T>   operator-(const Vector2D<T>& v1, const Vector2D<T>& v2);
    friend inline Vector2D<T>   operator/(const Vector2D<T>& v, T d);
    friend inline Vector2D<T>   operator*(const Vector2D<T>& v, T d);
    friend inline Vector2D<T>   operator*(T d, const Vector2D<T>& v2);
    friend inline T             operator*(const Vector2D<T>& v1, const Vector2D<T>& v2);

private:
    void swap(Vector2D& v);

private:
    T   m_data[2];
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename T>
inline Vector2D<T>::Vector2D()
{
    m_data[0] = m_data[1] = 0;
}

template <typename T>
inline Vector2D<T>::Vector2D(T x, T y)
{
    m_data[0] = x; m_data[1] = y;
}

template <typename T>
inline Vector2D<T>::Vector2D(const T* data)
{
    m_data[0] = data[0]; m_data[1] = data[1];
}

template <typename T>
inline Vector2D<T>::Vector2D(const Vector2D<T>& v)
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


//template <typename T>
//inline Vector2D<T> operator+(const Vector2D<T>& v1, const Vector2D<T>& v2)
//{
//    Vector2D<T> res(v1);
//    res += v2;
//    return res;
//}

template <typename T>
inline Vector2D<T> operator-(const Vector2D<T>& v1, const Vector2D<T>& v2)
{
    Vector2D<T> res(v1);
    res -= v2;
    return res;
}

template <typename T>
inline Vector2D<T> operator/(const Vector2D<T>& v, float d)
{
    Vector2D<T> res(v);
    res /= d;
    return res;
}

template <typename T>
inline Vector2D<T> operator*(const Vector2D<T>& v, float d)
{
    Vector2D<T><T> res(v);
    res *= d;
    return res;
}

template <typename T>
inline Vector2D<T> operator*(float d, const Vector2D<T>& v)
{
    return v * d;
}

template <typename T>
inline float operator*(const Vector2D<T>& v1, const Vector2D<T>& v2)
{
    return v1.m_data[0] * v2.m_data[0] + v1.m_data[1] * v2.m_data[1];
}

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
    float x = m_data[0]; float y = m_data[1];
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1];
    v.m_data[0] = x; v.m_data[1] = y;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef Vector2D<int> Vector2DInt;
typedef Vector2D<float> Vector2DFloat;

} // namespace core

#endif // GEOMETRY_VECTOR2D_H_