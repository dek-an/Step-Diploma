#ifndef GEOMETRY_VECTOR3D_H_
#define GEOMETRY_VECTOR3D_H_

namespace core
{

template <typename T = float>
class Vector3D
{
public:
    Vector3D();
    Vector3D(T x, T y, T z);
    explicit Vector3D(const T* data);
    Vector3D(const Vector3D& v);

public:
    inline T x() const;
    inline T y() const;
    inline T z() const;

    Vector3D& operator=(const Vector3D& v);

    inline Vector3D&    operator+=(const Vector3D& v);
    inline Vector3D&    operator-=(const Vector3D& v);
    inline Vector3D&    operator*=(T d);
    inline Vector3D&    operator/=(T d);

    friend inline Vector3D  operator+(const Vector3D& v1, const Vector3D& v2);
    friend inline Vector3D  operator-(const Vector3D& v1, const Vector3D& v2);
    friend inline Vector3D  operator/(const Vector3D& v, T d);
    friend inline Vector3D  operator*(const Vector3D& v, T d);
    friend inline Vector3D  operator*(T d, const Vector3D& v2);
    friend inline T     operator*(const Vector3D& v1, const Vector3D& v2);

private:
    void swap(Vector3D& v);

private:
    T   m_data[3];
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename T>
Vector3D<T>::Vector3D()
{
    m_data[0] = m_data[1] = m_data[2] = 0.0f;
}

template <typename T>
Vector3D<T>::Vector3D(T x, T y, T z)
{
    m_data[0] = x; m_data[1] = y; m_data[2] = z;
}

template <typename T>
Vector3D<T>::Vector3D(const T* data)
{
    m_data[0] = data[0]; m_data[1] = data[1]; m_data[2] = data[2];
}

template <typename T>
Vector3D<T>::Vector3D(const Vector3D& v)
{
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1]; m_data[2] = v.m_data[2];
}


template <typename T>
inline T Vector3D<T>::x() const
{
    return m_data[0];
}

template <typename T>
inline T Vector3D<T>::y() const
{
    return m_data[1];
}

template <typename T>
inline T Vector3D<T>::z() const
{
    return m_data[2];
}


template <typename T>
inline Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& v)
{
    m_data[0] += v.m_data[0]; m_data[1] += v.m_data[1]; m_data[2] += v.m_data[2];
    return *this;
}

template <typename T>
inline Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& v)
{
    m_data[0] -= v.m_data[0]; m_data[1] -= v.m_data[1]; m_data[2] -= v.m_data[2];
    return *this;
}

template <typename T>
inline Vector3D<T>& Vector3D<T>::operator/=(T d)
{
    m_data[0] /= d; m_data[1] /= d; m_data[2] /= d;
    return *this;
}

template <typename T>
inline Vector3D<T>& Vector3D<T>::operator*=(T d)
{
    m_data[0] *= d; m_data[1] *= d; m_data[2] *= d;
    return *this;
}


template <typename T>
inline Vector3D<T> operator+(const Vector3D<T>& v1, const Vector3D<T>& v2)
{
    Vector3D res(v1);
    res += v2;
    return res;
}

template <typename T>
inline Vector3D<T> operator-(const Vector3D<T>& v1, const Vector3D<T>& v2)
{
    Vector3D res(v1);
    res -= v2;
    return res;
}

template <typename T>
inline Vector3D<T> operator/(const Vector3D<T>& v, T d)
{
    Vector3D res(v);
    res /= d;
    return res;
}

template <typename T>
inline Vector3D<T> operator*(const Vector3D<T>& v, T d)
{
    Vector3D res(v);
    res *= d;
    return res;
}

template <typename T>
inline Vector3D<T> operator*(T d, const Vector3D<T>& v)
{
    return v * d;
}

template <typename T>
inline T operator*(const Vector3D<T>& v1, const Vector3D<T>& v2)
{
    return v1.m_data[0] * v2.m_data[0] + v1.m_data[1] * v2.m_data[1] + v1.m_data[2] * v2.m_data[2];
}

// ------------------------------------------------------

template <typename T>
Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& v)
{
    if (&v != this)
        Vector3D(v).swap(*this);
    return *this;
}

template <typename T>
void Vector3D<T>::swap(Vector3D<T>& v)
{
    T x = m_data[0]; T y = m_data[1]; T z = m_data[2];
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1]; m_data[2] = v.m_data[2];
    v.m_data[0] = x; v.m_data[1] = y; v.m_data[2] = z; 
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef Vector3D<int>   Vector3DInt;
typedef Vector3D<float> Vector3DFloat;

} // namespace core

#endif // GEOMETRY_VECTOR3D_H_