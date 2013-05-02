#ifndef GESTURES_GESTURE_H_
#define GESTURES_GESTURE_H_


namespace core
{

class Gesture
{
protected:
    Gesture(long gestureType);

public:
    bool isType(long gestureType) const;

private:
    long        m_gestureType;
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

class PaperGesture
    : public Gesture
{
public:
    static const long GestureType;

public:
    PaperGesture();
};

// ----------------------------------------------------------------------------

class ScissorsGesture
    : public Gesture
{
public:
    static const long GestureType;

public:
    ScissorsGesture();
};

// ----------------------------------------------------------------------------

class StoneGesture
    : public Gesture
{
public:
    static const long GestureType;

public:
    StoneGesture();
};

} // namespace core

#endif // GESTURES_GESTURE_H_