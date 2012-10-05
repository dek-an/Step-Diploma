#include "../../include/core/geometry/Vector3D.h"

#include <memory>

namespace core
{

Vector3D& Vector3D::operator=(const Vector3D& v)
{
    if (&v != this)
        Vector3D(v).swap(*this);
    return *this;
}

void Vector3D::swap(Vector3D& v)
{
    float x = m_data[0]; float y = m_data[1]; float z = m_data[2];
    m_data[0] = v.m_data[0]; m_data[1] = v.m_data[1]; m_data[2] = v.m_data[2];
    v.m_data[0] = x; v.m_data[1] = y; v.m_data[2] = z; 
}

} // namespace core