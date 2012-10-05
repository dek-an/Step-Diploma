#ifndef GEOMETRY_VECTOR3D_H_
#define GEOMETRY_VECTOR3D_H_

namespace core
{


class Vector3D
{
public:
    inline Vector3D();
    inline Vector3D(float x, float y, float z);
    inline Vector3D(const float* data);
    inline Vector3D(const Vector3D& v);

public:
    inline float x() const;
    inline float y() const;
    inline float z() const;

    Vector3D& operator=(const Vector3D& v);

    inline Vector3D&    operator+=(const Vector3D& v);
    inline Vector3D&    operator-=(const Vector3D& v);
    inline Vector3D&    operator*=(float d);
    inline Vector3D&    operator/=(float d);

    friend inline Vector3D  operator+(const Vector3D& v1, const Vector3D& v2);
    friend inline Vector3D  operator-(const Vector3D& v1, const Vector3D& v2);
    friend inline Vector3D  operator/(const Vector3D& v, float d);
    friend inline Vector3D  operator*(const Vector3D& v, float d);
    friend inline Vector3D  operator*(float d, const Vector3D& v2);
    friend inline float     operator*(const Vector3D& v1, const Vector3D& v2);

private:
    void swap(Vector3D& v);

private:
    float   m_data[3];
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

inline Vector3D::Vector3D()
{
    m_data[0] = m_data[1] = m_data[2] = 0.0f;
}

inline Vector3D::Vector3D(float x, float y, float z)
{
    m_data[0] = x; m_data[1] = y; m_data[2] = z;
}

inline Vector3D::Vector3D(const float* data)
{
    m_data[0] = data[0]; m_data[1] = data[1]; m_data[2] = data[2];
}

inline Vector3D::Vector3D(const Vector3D& v)
{
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1]; m_data[2] = v.m_data[2];
}


inline float Vector3D::x() const
{
    return m_data[0];
}

inline float Vector3D::y() const
{
    return m_data[1];
}

inline float Vector3D::z() const
{
    return m_data[2];
}


inline Vector3D& Vector3D::operator+=(const Vector3D& v)
{
    m_data[0] += v.m_data[0]; m_data[1] += v.m_data[1]; m_data[2] += v.m_data[2];
    return *this;
}

inline Vector3D& Vector3D::operator-=(const Vector3D& v)
{
    m_data[0] -= v.m_data[0]; m_data[1] -= v.m_data[1]; m_data[2] -= v.m_data[2];
    return *this;
}

inline Vector3D& Vector3D::operator/=(float d)
{
    m_data[0] /= d; m_data[1] /= d; m_data[2] /= d;
    return *this;
}

inline Vector3D& Vector3D::operator*=(float d)
{
    m_data[0] *= d; m_data[1] *= d; m_data[2] *= d;
    return *this;
}


inline Vector3D operator+(const Vector3D& v1, const Vector3D& v2)
{
    Vector3D res(v1);
    res += v2;
    return res;
}

inline Vector3D operator-(const Vector3D& v1, const Vector3D& v2)
{
    Vector3D res(v1);
    res -= v2;
    return res;
}

inline Vector3D operator/(const Vector3D& v, float d)
{
    Vector3D res(v);
    res /= d;
    return res;
}

inline Vector3D operator*(const Vector3D& v, float d)
{
    Vector3D res(v);
    res *= d;
    return res;
}

inline Vector3D operator*(float d, const Vector3D& v)
{
    return v * d;
}

inline float operator*(const Vector3D& v1, const Vector3D& v2)
{
    return v1.m_data[0] * v2.m_data[0] + v1.m_data[1] * v2.m_data[1] + v1.m_data[2] * v2.m_data[2];
}


} // namespace core

#endif // GEOMETRY_VECTOR3D_H_