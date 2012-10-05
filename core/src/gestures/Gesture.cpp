#include "../../include/core/gestures/Gesture.h"

namespace core
{

Gesture::Gesture(long gestureType)
    : m_gestureType(gestureType)
{
}

bool Gesture::isType(long gestureType) const
{
    return (gestureType == m_gestureType);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

const long PaperGesture::GestureType = (long)(void*)"Paper Gesture";

PaperGesture::PaperGesture()
    : Gesture(GestureType)
{
}

// ----------------------------------------------------------------------------

const long ScissorsGesture::GestureType = (long)(void*)"Scissors Gesture";

ScissorsGesture::ScissorsGesture()
    : Gesture(GestureType)
{
}

// ----------------------------------------------------------------------------

const long StoneGesture::GestureType = (long)(void*)"Stone Gesture";

StoneGesture::StoneGesture()
    : Gesture(GestureType)
{
}


} // namespace core