#include "../include/core/Contour.h"

namespace core
{

Contour::Contour()
{}

Contour::~Contour()
{}


void Contour::push_back(Vector2D<int> point)
{
    m_contour.push_back(point);
}

void Contour::clear()
{
    m_contour.clear();
}

Contour::ContourIterator Contour::begin() const
{
    return m_contour.begin();
}

Contour::ContourIterator Contour::end() const
{
    return m_contour.end();
}

} // namespace core