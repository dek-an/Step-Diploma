#ifndef CORE_CONTOUR_H_
#define CORE_CONTOUR_H_

#include "geometry/Vector2D.h"

#include <vector>


namespace core
{

class Contour
{
public:
    typedef std::vector< Vector2D<int> > ContourContainer;
    typedef ContourContainer::const_iterator ContourIterator;

public:
    Contour();
    ~Contour();

public:
    void push_back(Vector2D<int> point);
    void clear();
    ContourIterator begin() const;
    ContourIterator end() const;

private:
    ContourContainer    m_contour;
};

} // namespace core

#endif // CORE_CONTOUR_H_