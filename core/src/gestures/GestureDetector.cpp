#include "../../include/core/gestures/GestureDetector.h"

namespace core
{

GestureDetector::GestureDetector(long detectorType)
    : m_detectorType(detectorType)
{}


long GestureDetector::type() const
{
    return m_detectorType;
}

bool GestureDetector::isType(long detectorType) const
{
    return (detectorType == m_detectorType);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

const long PaperScissorsStoneDetector::DetectorType = (long)(void*)"Paper/Scossors/Stone detector";

PaperScissorsStoneDetector::PaperScissorsStoneDetector()
    : GestureDetector(DetectorType)
{
}

} // namespace core